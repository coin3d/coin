# `soshape_bumprender` context/lifecycle reproducer

This manual regression test renders one Coin shape through two real,
non-shared GLX contexts and distinct Coin cache-context IDs.

It verifies three contracts:

1. `A -> B -> A` selects a complete ARB program set owned by the current
   cache context. Before B renders, A's numeric names are deliberately made
   into empty programs in B. This turns the historical silent failure into a
   deterministic `GL_PROGRAM_LENGTH_ARB == 0` failure.
2. If context A dies while the shape is alive, calling
   `SoContextHandler::destructingContext(idA)` with A current deletes A's three
   programs and leaves B usable.
3. If the shape dies while B is alive, deletion is deferred until Coin next
   enters B's cache context, then all three names disappear.

The test uses only public Coin APIs. It intentionally does not inspect the
private hash table or depend on test-only instrumentation.

## Requirements

- X11 and GLX development headers/libraries;
- a built shared `libCoin`;
- an OpenGL compatibility context with `ARB_vertex_program`,
  `ARB_fragment_program`, and Coin's bump-mapping feature set.

This is not registered in the default CTest suite because ordinary CI runners
cannot be assumed to provide those graphics facilities. Missing display or
extensions produce exit status 2 (`INCONCLUSIVE`), never a false pass.

## Run

```sh
testsuite/reproducers/soshape-bumprender-context-lifecycle/run.sh \
  /absolute/path/to/build/lib
```

For a software driver, run the same command under a working Xvfb server with
`LIBGL_ALWAYS_SOFTWARE=1`. A local desktop X server is also sufficient.

On the historical implementation the B program lengths are zero, the A
context-first names remain live, and the B owner-first names are not drained.
With the fix all checks pass.
