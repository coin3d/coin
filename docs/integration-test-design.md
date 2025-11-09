
## 0. Final decisions & constraints

Lock these in first:

* **Context backend (for now)**:
    
    * Single backend: `GLFWBackend` with hidden window + FBO.
        
    * Use `GLFW_CLIENT_API = OPENGL_API` for `backend=gl`, `OPENGL_ES_API` for `backend=gles`.
        
    * On web, reuse the same logic via Emscripten’s GLFW.
        
* **Comparator**:
    
    * Core: small in-house C++ comparator (lives in the `CoinRenderTests compare` subcommand), doing:
        
        * Per-pixel abs diff per channel (0–255).
            
        * Count of pixels above threshold.
            
        * % differing pixels.
            
        * Max abs diff.
            
        * RMSE over RGBA.
            
        * Optional mask image.
            
        * Diff.png heatmap.
            
    * Optional later: integrate a perceptual metric (e.g., Butteraugli or external `idiff`) as an **extra** field in metrics, not the gatekeeper initially.
        
* **Specs**: YAML files describing scenes + camera + state + per-backend tolerances.
    
* **CI**:
    
    * First version: Linux CI with **GLFW + Xvfb/headless display**.
        
    * Future option: second “pure EGL headless” backend if you want to drop X/Wayland.
        

* * *

## 1. Project layout (files & dirs)

Create this structure (adjust to your actual repo):

```text
tools/
  CoinRenderTests/
    src/
      main.cpp                  # dispatches subcommands
      SnapshotCommand.cpp/.h    # CoinRenderTests snapshot
      CompareCommand.cpp/.h     # CoinRenderTests compare
      RunCommand.cpp/.h         # CoinRenderTests run / update
      RenderCore.cpp/.h         # scene loading + GL backend
      GLFWBackend.cpp/.h
      SpecLoader.cpp/.h
      CompareCore.cpp/.h
      TestRunner.cpp/.h

tests/
  render/
    scenes/
      cube.iv
      sphere.iv
      lines.iv
      transparent.iv
      textured_quad.iv
    specs/
      cube.yml
      sphere.yml
      ...
    baselines/
      gl/
        cube.png
        ...
      gles/
        ...
    masks/
      cube_mask.png   (optional)

thirdparty/
  stb/
  yaml-cpp/
  (optional later: butteraugli, etc.)
```

CMake top-level options:

* `RENDER_TESTS_ENABLED` (ON/OFF)
    
* `RENDER_TESTS_ENABLE_GLES` (ON/OFF)
    
* `RENDER_TESTS_ENABLE_WEB` (ON/OFF)
    
* `RENDER_TESTS_ENABLE_PERCEPTUAL_METRICS` (ON/OFF, later)
    

* * *

## 2. Core components (responsibilities)

We ship **one executable** named `CoinRenderTests`. It exposes git-style subcommands so CI, developers, and future WASM builds all call the same binary:

```text
CoinRenderTests snapshot   # render a single scene/spec to PNG
CoinRenderTests compare    # compare two images + metrics/diff
CoinRenderTests run        # iterate specs, render + compare
CoinRenderTests update     # optional helper that regenerates baselines
```

Internally each subcommand still lives in its own translation unit so the code stays modular (`RenderCore`, `CompareCore`, `TestRunner`, etc.).

### 2.1 `CoinRenderTests snapshot`

* Inputs: scene/spec/backend/paths.
* Automatically loads specs from the canonical `testsuite/render/specs` directory and accepts the spec `id` as a positional argument (e.g., `CoinRenderTests snapshot cube_basic …`); the command still supports `--spec <path>` for backward compatibility.
* Defaults the output file (`--out`) when omitted (e.g., `coin_basic.png` next to CWD) so the CLI is easier to use interactively.

* Creates GL context (via GLFWBackend), FBO.

* Loads Coin scene (`.iv`), sets camera + state.
  * Removes any cameras shipped inside the `.iv` content when a spec is loaded so only the YAML camera is rendered, which keeps the WASD handler in control.

* Renders to FBO.

* `glReadPixels` → RGBA8 buffer → PNG output.

* Optionally writes simple render metrics JSON (render time, backend info).
* Additional flags:
  * `--gl-profile compat|core` toggles the requested OpenGL profile (compatibility is still the default).
  * `--visible-window --wait` keeps the GLFW window open and runs a mini render loop so WASD movement is visible while `CoinRenderTests snapshot` waits for you to close the window.
  * The interactive loop reuses `renderScene(false)` so re-renders happen without triggering another PNG capture; log output prints the deltas whenever WASD moves the camera.

* Exit code 0 on success, non-zero on failures (context, scene, GL, file).
    

### 2.2 `CoinRenderTests compare`

* Inputs: expected.png, actual.png, optional mask.png, tolerance args, diff output path, metrics output path.
    
* Loads PNGs, applies mask, computes metrics.
    
* Writes:
    
    * `diff.png` heatmap (greyscale or colored).
        
    * `metrics.json`.
        
* Exit code:
    
    * 0 if within tolerance.
        
    * 1 if outside tolerance.
        
    * 2+ for IO/other errors.
        

### 2.3 `CoinRenderTests run` (test orchestrator)

* Inputs: `--spec-dir`, `--backend`, `--artifacts-dir`, `[--only ids]`, `[--update-baselines]`, `[--retries N]`.

* Discovers `.yml` specs.
    
* For each spec + matching backend:
    
    * Runs `CoinRenderTests snapshot` → `actual.png`.
        
    * If `--update-baselines`: copies `actual` to baseline path.
        
    * Else: runs `CoinRenderTests compare` vs baseline.
        
* Aggregates failures and returns:

    * 0 if all tests passed.
        
    * 1 if any regression.

* Defaults:
  * `spec-dir` now resolves relative to the executable (so running from `bld/` still finds `testsuite/render/specs`), avoiding brittle `cwd` assumptions.
  * `--gl-profile` can be used here too and is forwarded to each child `snapshot` invocation so both compat and core paths get covered in the suite.

* * *

## 3. YAML spec schema (final)

Use this as the contract between tools & tests:

```yaml
id: cube_basic
scene: scenes/cube.iv

camera:
  type: perspective    # or orthographic
  position: [2, 2, 2]
  target: [0, 0, 0]
  up: [0, 1, 0]
  fov_deg: 45
  near: 0.1
  far: 10.0

viewport:
  width: 512
  height: 512
  background: [0.2, 0.2, 0.2, 1.0]

state:
  depth_test: true
  blend: false
  cull_face: true
  msaa_samples: 0
  srgb: false

toggles:
  lighting: true
  textures: false
  vbo: false

outputs:
  - backend: gl
    baseline: baselines/gl/cube_basic.png
    tolerance:
      per_channel: 2         # 0–255
      max_diff_pct: 0.5      # %
      rmse: 1.0              # optional, <= 0 means “ignore”
    mask: masks/cube_basic.png
  - backend: gles
    baseline: baselines/gles/cube_basic.png
    tolerance:
      per_channel: 3
      max_diff_pct: 1.0

notes: "Simple lit cube."
```

C++ structs mirror this (one `RenderTestSpec` per file, multiple `OutputSpec` entries for different backends).

* * *

## 4. Incremental development plan (phases & tasks)

### Phase 0 – Setup & scaffolding

**Goal:** Project wiring compiles; basic CLI skeletons exist.

Tasks:

1. Add CMake option `ENABLE_RENDER_TESTS`.
    
2. Add `thirdparty/stb` and `thirdparty/yaml-cpp` (via `FetchContent` or submodule).
    
3. Add the `CoinRenderTests` executable with stub subcommands:
    
    * `CoinRenderTests snapshot --help` prints usage.
        
    * `CoinRenderTests compare --help` prints usage.
        
    * `CoinRenderTests run --help` prints usage.
        
4. Add a `ctest` stub:
    
    * e.g. `add_test(NAME render_smoke COMMAND CoinRenderTests snapshot --help)`.
        

Acceptance:

* Running `ctest -R render` succeeds.
    
* All three subcommands build/run and show a usage message.
    

* * *

### Phase 1 – GLFW backend + raw snapshot (no specs yet)

**Goal:** You can render a single `.iv` scene to a PNG offscreen using GLFW.

Tasks:

1. Implement `GLFWBackend`:
    
    * `BackendConfig { width, height, msaa_samples, gles, gl_major, gl_minor }`.
        
    * `init()`:
        
        * `glfwInit()`.
            
        * Hints:
            
            * `GLFW_VISIBLE = GLFW_FALSE`.
                
            * `GLFW_CLIENT_API = gles ? GLFW_OPENGL_ES_API : GLFW_OPENGL_API`.
                
            * `GLFW_CONTEXT_VERSION_MAJOR/MINOR` from config.
                
            * `GLFW_CONTEXT_CREATION_API = GLFW_EGL_CONTEXT_API` if you want EGL under the hood.
                
        * Create window with size `width/height`.
            
        * `glfwMakeContextCurrent`.
            
        * `glfwSwapInterval(0)` (vsync off).
            
        * Create FBO + color & depth RBO/texture.
            
    * `bindFramebuffer()` binds the FBO and sets viewport.
        
    * `readPixels()` does `glReadPixels` RGBA8 into provided buffer.
        
2. Implement minimal Coin render path in `CoinRenderTests snapshot`:
    
    * Parse CLI:
        
        * `--scene file.iv`
            
        * `--backend gl`
            
        * `--out out.png`
            
        * `--width/--height` (optional).
            
    * Create `GLFWBackend` with config (default GL, 3.3 or what Coin likes).
        
    * Normalize GL state (disable smoothing, MSAA, etc.).
        
    * Load scene via Coin:
        
        * `SoDB::readAll(&in)`.
            
    * Create camera programmatically (simple fixed camera for now).
        
    * Use `SoGLRenderAction` to render into current context/FBO.
        
    * Read back pixels, write PNG via stb_image_write.
        
3. Add a **single** `.iv` example (`cube.iv`) under `testsuite/render/scenes`.
    

Acceptance:

* `CoinRenderTests snapshot --scene testsuite/render/scenes/cube.iv --backend gl --out cube.png` produces a plausible PNG locally.
    
* This works in your CI image (with Xvfb/headless display configured).
    

* * *

### Phase 2 – YAML specs + camera/state from file

**Goal:** `CoinRenderTests snapshot` can read a spec and honor camera, viewport, background, and basic state.

Tasks:

1. Define C++ structs:
    
    * `CameraSpec`, `ViewportSpec`, `StateSpec`, `Toggles`, `ToleranceSpec`, `OutputSpec`, `RenderTestSpec`.
        
2. Implement `spec_loader.cpp` using `yaml-cpp`:
    
    * `RenderTestSpec loadSpec(const std::string& path);`
        
    * Validate required fields (`id`, `scene`, `camera`, `viewport`, at least one `outputs` entry).
        
3. Update `CoinRenderTests snapshot`:
    
    * New flags:
        
        * `--spec spec.yml`
            
        * `--backend gl|gles|web`
            
        * `--out actual.png`
            
        * `--visible-window` (optional — renders to the GLFW window before still writing the PNG).
            
    * If `--spec` is given:
        
        * Load spec.
            
        * Pick `OutputSpec` matching backend.
            
        * Use spec’s `scene`, `camera`, `viewport`, `state`, `toggles`.
            
    * If only `--scene` is given (no `--spec`), keep Phase-1 behavior (debug mode).
        
4. Implement mapping from `CameraSpec` to Coin camera node:
    
    * Create `SoPerspectiveCamera` or `SoOrthographicCamera`.
        
    * Set position/orientation/near/far/fov.
        
5. Implement state mapping:
    
    * Depth, blend, cull, msaa, srgb flags → GL state overrides.
        
6. Create 2–3 basic `*.yml` specs (cube, sphere) and hook them up.
    

Acceptance:

* `CoinRenderTests snapshot --spec testsuite/render/specs/cube.yml --backend gl --out out/cube.png` uses the YAML camera & viewport.
    
* Changing YAML camera visibly changes the image.
    

* * *

### Phase 3 – `CoinRenderTests compare` (core C++ comparator)

**Goal:** Have a reliable, self-contained comparator: per-pixel diff, metrics, diff image, tolerances.

Tasks:

1. Implement PNG loader/writer using `stb_image`/`stb_image_write` in `compare_core.cpp`.
    
2. Implement comparison core:
    
    ```cpp
    struct DiffMetrics {
        int width, height;
        int total_pixels;
        int compared_pixels;
        int differing_pixels;
        double diff_pct;
        int max_abs_diff;
        double mean_abs_diff;
        double rmse;
        bool pass;
    };
    
    DiffMetrics compare(const Image& expected,
                        const Image& actual,
                        const Mask* mask,
                        const ToleranceSpec& tol);
    ```
    
    * Per pixel:
        
        * If mask provided and mask alpha == 0 → skip.
            
        * For RGBA channels:
            
            * `d = abs(e - a)`.
                
            * Track `max_abs_diff`.
                
            * Accumulate sum of abs diffs & squared diffs.
                
            * If any channel’s `d > tol.per_channel`, mark pixel differing.
                
    * After loop:
        
        * `diff_pct = 100.0 * differing_pixels / compared_pixels`.
            
        * `rmse = sqrt(sum_sq_diff / (4 * compared_pixels))`.
            
        * `pass = (diff_pct <= tol.max_diff_pct) && (tol.rmse <= 0 || rmse <= tol.rmse)`.
            
3. Implement diff heatmap generation:
    
    * For each pixel, map max per-channel diff to intensity `[0..255]`.
        
    * Write grayscale or simple colored heatmap to `diff.png`.
        
4. Implement `CoinRenderTests compare` CLI:
    
    * Args:
        
        * `--expected <file>`
            
        * `--actual <file>`
            
        * `--mask <file>` (optional)
            
        * `--tolerance-per-channel N`
            
        * `--max-diff-pct P`
            
        * `--rmse R` (optional)
            
        * `--diff <file>` (optional)
            
        * `--metrics <file>` (optional)
            
    * Prints a one-line result + optional verbose stats.
        
    * Writes `metrics.json` if requested.
        
5. Define exit codes:
    
    * 0: pass.
        
    * 1: comparison failed tolerance.
        
    * 2+: errors (file IO, size mismatch, etc.).
        

Acceptance:

* Comparing an image to itself passes with zero diffs.
    
* Comparing to a modified image yields expected metrics.
    
* A CTest or simple script runs a couple of known comparisons.
    

* * *

### Phase 4 – `CoinRenderTests run` + CTest integration + baselines (GL only)

**Goal:** Full end-to-end: CTest calls a single test runner which renders & compares using specs and baselines.

Tasks:

1. Implement `CoinRenderTests run`:
    
    * CLI args:
        
        ```text
        --spec-dir <path>
        --backend <gl|gles|web>
        --artifacts-dir <path>
        [--only id1,id2,...]
        [--update-baselines]
        [--retries N]
        ```
        
    * Logic:
        
  * Parse every `.yml` in `spec-dir` up front (the spec directory is the authoritative catalog).
            
        * For each `RenderTestSpec`:
            
            * For each `OutputSpec` whose backend matches `--backend`:
                
                * If `--only` is set and `id` not in list → skip.
                    
                * Compute:
                    
                    * `actual = artifacts-dir/backend/id.actual.png`
                        
                    * `diff = artifacts-dir/backend/id.diff.png`
                        
                    * `metrics = artifacts-dir/backend/id.metrics.json`
                        
                * On `--update-baselines`:
                    
                    * Run `CoinRenderTests snapshot --spec spec.yml --backend ... --out tmp.png`.
                        
                    * Copy `tmp.png` to spec’s `baseline` path.
                        
                * Otherwise:
                    
                    * Run `CoinRenderTests snapshot` → `actual`.
                        
                    * Run `CoinRenderTests compare` with tolerance & mask from `OutputSpec`.
                        
                    * Respect `--retries` (if fail, rerun once before giving up).
                        
        * Aggregate pass/fail across tests and exit accordingly.
            
2. Seed initial baselines for backend `gl`:
    
    * Scenes: cube, sphere lit, lines/polyline, transparency overlap, textured quad.
        
    * Use `CoinRenderTests run --backend gl --update-baselines --only ...` to generate baseline PNGs under `testsuite/render/baselines/gl/`.
        
3. Wire CTest:
    
    ```cmake
    if (ENABLE_RENDER_TESTS)
      add_test(
        NAME render_gl
        COMMAND CoinRenderTests run
          --spec-dir ${CMAKE_CURRENT_SOURCE_DIR}/testsuite/render/specs
          --backend gl
          --artifacts-dir ${CMAKE_BINARY_DIR}/render_artifacts/gl
      )
    endif()
    ```
    
4. Add a second test for a small subset (fast local run) if desired.
    

Acceptance:

* `ctest -R render_gl` renders all GL tests, compares them, and passes.
    
* On deliberate change to cube scene, CTest fails and artifacts show expected/actual/diff.
    

* * *

### Phase 5 – GLES backend via GLFW

**Goal:** Same pipeline running with OpenGL ES contexts.

Tasks:

1. Extend `GLFWBackend`:
    
    * If backend is `gles`:
        
        * `GLFW_CLIENT_API = GLFW_OPENGL_ES_API`.
            
        * Choose ES version (2 or 3) via hints.
            
    * Ensure FBO creation is ES-compatible (no forbidden enums).
        
2. In spec YAML, add `backend: gles` entries for a subset of tests with slightly looser tolerances.
    
3. Update the render backend factory (used by snapshot/run modes):
    
    * `--backend gles` → `GLFWBackend` with `gles = true`.
        
4. Add CTest:
    
    ```cmake
    add_test(
      NAME render_gles
      COMMAND CoinRenderTests run
        --spec-dir ...
        --backend gles
        --artifacts-dir ${CMAKE_BINARY_DIR}/render_artifacts/gles
    )
    ```
    
5. Adjust shaders or Coin configuration if it uses desktop-only GL features.
    

Acceptance:

* `ctest -R render_gles` passes on CI environment that supports GLES via GLFW+EGL (or on dev machine).
    
* GLES images look reasonable; diffs are within tolerances.
    

* * *

### Phase 6 – Web / WASM (initial subset)

**Goal:** A few tests run in WebAssembly/WebGL and produce PNGs that can be compared.

Tasks (high level):

1. Emscripten build:
    
    * Add a CMake target that compiles `CoinRenderTests` with Emscripten.
        
    * Use the same GLFW code path.
        
2. WASM harness:
    
    * JS: load wasm, call `coin_render_run_snapshot(id, backend="web")` (or similar helper that wraps the snapshot mode).
        
    * Ensure `CoinRenderTests snapshot` inside wasm writes PNG to virtual FS (e.g. `/tmp/id.web.png`).
        
3. Node/headless browser script:
    
    * Runs the wasm module for each selected spec.
        
    * Copies produced PNGs out of virtual FS into `artifacts-dir/web`.
        
4. Compare with native:
    
    * On host, run `CoinRenderTests compare` vs `baselines/web/id.png`.
        
5. Add a small CTest or CI job that executes this harness for 3–5 simple scenes.
    

Acceptance:

* Web job produces snapshots and diff metrics.
    
* You have first `backend: web` entries in YAML (probably with higher tolerances).
    

* * *

### Phase 7 – Perceptual metrics & HTML report (optional, Python ok here)

**Goal:** Make regressions easier to triage; not critical for CI correctness.

Tasks:

1. Extend `metrics.json` (from `CoinRenderTests compare`) with optional fields:
    
    * Placeholder for `perceptual_score`, `perceptual_diff_image` path, etc.
        
2. Add Python script `tools/scripts/report.py`:
    
    * Walk `artifacts-dir/**/metrics.json`.
        
    * Build an HTML page:
        
        * Table per backend: id, pass/fail, diff_pct, rmse, max_abs_diff, link to expected/actual/diff.
            
    * Optionally compute SSIM or a perceptual metric using Python libs and add to report.
        
3. CI integration:
    
    * If Python and dependencies are available:
        
        * Run `report.py` after render tests.
            
        * Upload the HTML as an artifact.
            
4. (Optional) Integrate C++ perceptual metric:
    
    * E.g. Butteraugli: compute `butteraugli_score` and dump into metrics.
        
    * Use it as a warning threshold, not the main pass/fail criterion at first.
        

Acceptance:

* For a failing test, the HTML report clearly shows what changed and by how much.
    
* CI artifacts include expected/actual/diff + HTML dashboard.
    
