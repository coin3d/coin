# CoinRenderTests Spec Expansion Plan

Goal: cover key Coin3D features with focused specs and reusable scenes. For each group below, the table lists the `.iv` scene we need (new or existing), the spec IDs to add, backends/profiles to run, and any special notes (tolerances, masks, toggles).

## Scenes to (Re)use or Add
- Existing: `testsuite/render/scenes/cube.iv`, `sphere.iv`.
- New simple helpers (add under `testsuite/render/scenes/`):
  - `text.iv` (simple 3D text with a transform separator).
  - `quad_textured.iv` (unit quad with a bound texture coordinate node).
  - `lights.iv` (separator containing multiple lights and a neutral mesh, e.g., cube).
  - `transparency_stack.iv` (stacked quads with alpha).
  - `lod_switch.iv` (LOD/Switch with two child meshes).
  - `billboard.iv` (Billboard node with a quad child).
  - `normals_missing.iv` (mesh without normals to exercise auto-generation).
  - `degenerate.iv` (geometry with some zero-area triangles).

## Spec Matrix (per feature)

| Feature | Scene | Spec IDs (new) | Backend/Profile | Notes |
| --- | --- | --- | --- | --- |
| Cameras: perspective/orthographic, FOV extremes, off-axis | `cube.iv` | `camera_persp_basic`, `camera_ortho_basic`, `camera_fov_extreme` | gl compat + core | Vary `fov_deg`, target offsets; include near/far edge cases. |
| Viewport sizing/backgrounds | `cube.iv` | `viewport_small_bg`, `viewport_large_bg` | gl compat | Use different viewport sizes and background colors. |
| Lighting: directional, point, spot | `lights.iv` | `lighting_directional`, `lighting_point`, `lighting_spot` | gl compat + core | Toggle `toggles.lighting`; adjust attenuation in point/spot. |
| Materials/colors (diffuse/specular/emissive, two-sided, sRGB) | `cube.iv` | `materials_shiny`, `materials_two_sided`, `materials_srgb_on` | gl compat + core | Switch `state.srgb` on/off; two-sided material on cube. |
| Geometry variety & culling | `cube.iv`, `sphere.iv`, `normals_missing.iv`, `degenerate.iv` | `cull_on_off_cube`, `normals_autogen`, `degenerate_faces` | gl compat + core | Flip `state.cull_face`; expect masks/tolerances for degenerate case. |
| Transparency & blending | `transparency_stack.iv` | `transparency_blend`, `transparency_msaa` | gl compat + core | Set `state.blend=true`, test with `msaa_samples=4`; add mask/tolerance. |
| Textures: wrap/filter/mipmap/NPOT | `quad_textured.iv` | `texture_wrap_repeat`, `texture_wrap_clamp`, `texture_mipmap`, `texture_npot` | gl compat + core | Swap sampler params via scene or spec toggles; ensure texture assets exist. |
| Normal maps (if supported) | `quad_textured.iv` | `texture_normalmap` | gl compat + core | Add normal map texture; adjust material. |
| State toggles: depth/cull/blend/msaa | Any simple scene (cube) | `state_depth_off`, `state_cull_off`, `state_blend_on`, `state_msaa_4x` | gl compat + core | Explicitly flip one state per spec. |
| Background overrides | `cube.iv` | `background_override` | gl compat | Use `viewport.background` with alpha, confirm clear color. |
| GL profiles/backends | Any above | duplicate specs with `*_core` | gl core | Add outputs for core profile to ensure “no legacy” path. |
| Advanced nodes: text, transforms, separators | `text.iv` | `text_basic`, `transform_stack` | gl compat + core | Include nested separators with local state to ensure isolation. |
| LOD/Switch nodes | `lod_switch.iv` | `lod_switch_basic` | gl compat + core | Ensure correct child selection at given camera distance. |
| Billboards | `billboard.iv` | `billboard_basic` | gl compat + core | Verify billboard faces camera. |
| Edge/error cases | `normals_missing.iv`, `degenerate.iv` | `normals_missing_auto`, `degenerate_geometry` | gl compat + core | Expect relaxed tolerances or masks. |

## Tolerances, Masks, Outputs
- Add masks/tolerances for noisy cases: transparency/MSAA, degenerate geometry, normal-map lighting variance.
- For each spec, add `outputs` entries for both `backend: gl` with `gl-profile compat` and `gl-profile core` (by duplicating outputs with different IDs or by adding a second output entry).

## Grouping & Naming
- Place new `.iv` scenes under `testsuite/render/scenes/`.
- Add specs under `testsuite/render/specs/` with clear prefixes: `camera_*`, `lighting_*`, `materials_*`, `textures_*`, `state_*`, `profile_*`, `advanced_*`, `edge_*`.
- Reuse existing assets where possible; keep new textures small (PNG) and store under `testsuite/render/scenes/textures/`.

## Implementation Steps (for Codex)
1) Add missing scenes (`text.iv`, `quad_textured.iv`, `lights.iv`, `transparency_stack.iv`, `lod_switch.iv`, `billboard.iv`, `normals_missing.iv`, `degenerate.iv`).
2) Author specs per the matrix above, reusing cube/sphere where possible.
3) Add baseline outputs for both compat/core where relevant; apply masks/tolerances for noisy specs.
4) Wire textures/normal maps into `quad_textured.iv` scenes and reference them from specs.
5) Keep specs small and focused; one feature toggle per spec.
