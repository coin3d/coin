# Bump program regression tests

`BumpProgramFailuresAndLifetime` runs without a GPU. It includes the production
implementation under a distinct private class name and adapts only GL calls and
context/state bookkeeping. It uses the real `SoContextHandler` dispatcher.
Coverage includes each upload stage, partial rollback, failure caching and
context-id reuse, pending initialization cancellation, diffuse/specular sets,
both resource destruction orders, stale callback snapshots, concurrent cache
access, and reentrant error handlers. Checks remain enabled in Release builds.

`BumpProgramGLX` runs when an X display and ARB vertex/fragment programs are
available; otherwise CTest reports it as skipped. It verifies the actual shaders
in two independent contexts, binding restoration, both GL destruction orders,
and deferred initialization with default and pre-existing bindings inside
`GL_COMPILE`. A real `SoShape` scene with `renderCaching=ON` also exercises Coin's
actual current-context scheduler and resource cleanup. It warms a cached scene
in context A, then verifies that the first visit to context B enters a display
list with uninitialized programs. A `SoSceneManager` notification must schedule
the next rendering automatically, restore the expected pixel color, and settle
without repeated redraws. The private test adapter uses exported cleanup APIs
and fails explicitly if an unadapted drawing helper is accidentally exercised.

Run both with:

```sh
ctest --test-dir build --output-on-failure -R BumpProgram
```

The private cache distinguishes pending initialization from failed uploads.
While a display list is open, it invalidates that incomplete cache and queues
initialization for the next current-context opportunity before cache traversal.
That first traversal can omit the specular contribution; the next one uses the
validated set, including with forced render caching. `SoShape` notifies the scene
root after releasing its render lock, scheduling that next frame in event-driven
viewers without requiring user interaction. A standalone render action still
needs the application to service the redraw notification or perform another
traversal. If a callback itself runs
inside a user display list, it does not upload and a later traversal can schedule
another attempt.

An upload failure disables that program set for the lifetime of the Coin cache
context id, including transient GL allocation failures. Context destruction
clears this failure state and permits a recycled id to try again. Deferred
callbacks never call user error handlers under the scheduler lock; a failure is
reported once on the next renderer request, outside the private cache mutex.

The existing Coin context contract still applies: notify context destruction
with that context current, and use distinct ids for incompatible contexts.
Coordination of physical shared contexts using the same cache id is not changed.
