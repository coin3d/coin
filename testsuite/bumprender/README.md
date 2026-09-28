# Bump program regression tests

`BumpProgramFailuresAndLifetime` runs without a GPU. It includes the production
implementation under a distinct private class name and adapts only GL calls and
context/state bookkeeping. Successful registrations and destruction use the
real `SoContextHandler` dispatcher; a wrapper injects registration failures
before entering that provider.
Coverage includes each upload stage, partial rollback, failure caching and
context-id reuse, pending initialization cancellation, diffuse/specular sets,
both resource destruction orders, stale callback snapshots, concurrent cache
access, and reentrant error handlers. Allocation-failure tests verify upload
rollback, bounded diagnostics, context cleanup, allocation-free destructor
metadata, failed queue submission, cache object/control-block/registry creation,
context-map insertion, and transactional sensor creation/control-block/insertion.
Boundary tests check local constructor rollback and propagation of provider and
diagnostic-handler exceptions. Token-limit tests cover the last valid
registration, controlled exhaustion, null-cache operations, stale callbacks and
exhaustion across SoDB reinitialization. Checks remain enabled in Release builds.

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

Exhaustion of callback tokens refuses new program caches without explicitly
throwing or recycling identities. Existing caches remain usable; an unavailable
cache makes program requests fail and redraw scheduling/destruction become safe
no-ops. The creation diagnostic runs once for the registry lifetime, outside its
mutex, including when its handler reenters cache creation. This does not promise
that dependency or application-handler exceptions cannot propagate.

Driver diagnostics use a fixed 512-byte buffer (511 text bytes plus terminator),
marking truncated messages with an ellipsis. Capturing/copying a diagnostic
cannot interrupt upload rollback or context cleanup through a host allocation.
If initialization cannot be queued, its family becomes failed rather than
remaining pending without a callback; context destruction allows another try.
Destructor metadata does not allocate. If a deletion callback cannot be queued,
the existing registry/context-destruction registration retains the GL names
until context destruction instead of unwinding the destructor. This is local
cache recovery, not a guarantee that the underlying Coin scheduler, glue or user
callback APIs preserve all their own invariants under arbitrary exceptions.

## Failure ownership and scope

The new cache handles its own expected allocation failures with fixed-storage
failure state. Diagnostic capture is local; diagnostic dispatch and application
handlers remain external dependency calls:

| Owned by this change | Local outcome |
| --- | --- |
| Callback identity exhaustion | Refuse new caches; never reuse an identity. |
| Cache object, shared control block or registry-node allocation | Return an unavailable cache, without publishing resources or callbacks for it. |
| Context-map insertion | Return failure before any GL query, name allocation, upload or enqueue; leave output arguments untouched. |
| Redraw sensor, shared control block or redraw-map insertion | Leave no empty entry, attachment or queued sensor; preserve other viewers' existing sensors. |
| GL errors during save/generation/bind/upload/validation/restore | Publish only a fully validated set; roll back names and cache the failed family. |
| Diagnostic capture, cleanup copies and destructor metadata | Use fixed storage or allocation-free ownership transfers. |

Context metadata exhaustion latches refusal of new contexts for that cache, with
one warning outside its mutex. Existing context records remain usable. Any
context-destruction notification clears this latch, including an id whose
insertion failed. This is deliberately conservative: it needs no emergency map
allocation and avoids repeating allocation/diagnostic work on every traversal.

Redraw allocation is best-effort. If it fails, deferred initialization still
keeps its own callback, but the affected viewer has no guaranteed automatic
next frame from this attempt. A later application traversal can retry the
notification or use the initialized programs. Sensor ownership is established
before map publication; triggering uses a nonthrowing weak lock rather than
`shared_from_this()`. Root-reference rollback also runs if notification throws.

Coin's providers retain responsibility for their own exception safety:
`coin_atexit` and `SoContextHandler` registration/removal, the GL callback queue,
sensor attachment/queues/notifications, GL glue, diagnostic formatting/dispatch,
and error/application handlers.
Constructor registration failure rolls back only our registry entry and
rethrows; a callback retained by a partially failing provider sees an absent,
non-reused token. No removal is attempted for an unconfirmed registration.
Queue-submission recovery assumes a safely rejected enqueue, not a provider
left with a locked mutex or partially mutated queue. The adapted queue tests
check that local response, not the real scheduler's allocation-failure atomicity.
In particular, an exception from a queue-change handler is not swallowed as a
sensor-allocation failure: insertion may already have happened. Handlers invoked
by these Coin services must not throw. Pre-existing rendering/array paths and
general Coin exception safety are outside this change; no blanket `noexcept`
contract or `-fno-exceptions` support is introduced.

The existing Coin context contract still applies: notify context destruction
with that context current, and use distinct ids for incompatible contexts.
Coordination of physical shared contexts using the same cache id is not changed.
