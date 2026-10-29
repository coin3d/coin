# Callback regression checks

Portable coverage runs automatically in `CoinTests`: typed dispatch after copy,
assignment and owner destruction; mixed registrations and last-duplicate removal;
removal/addition/clear during dispatch and nested invocation of the cleared list.

From the repository root, with a shared Coin build:

```sh
sh testsuite/reproducers/soselection-protected-callbacklist-abi/run.sh /path/to/build/lib
sh testsuite/reproducers/soselection-protected-callbacklist-abi/run-old-header.sh /path/to/build/lib
sh testsuite/reproducers/sodragger-callback-function-pointer-ub/run.sh /path/to/build/lib
sh testsuite/reproducers/socallbacklist-second-wave/run.sh /path/to/build/lib
```

The scripts accept `CXX`, `CXXFLAGS` and `LDFLAGS`, resolve relative library
paths before changing directories, and clean temporary executables. The
second-wave driver needs X11 development headers/libraries and a reachable X
display for its rendering case. A printed SKIP is not rendering coverage.

The old-header client uses every public header from the fixed pre-change commit
`c27cf9a72d302c8c1ee3367a8865a603ea26622a` (optional second argument overrides it),
plus generated configuration headers from the selected build. That commit must
exist locally. Checks remain active with `-DNDEBUG`. This exercises a historical
subclass layout, mixed dispatch, copies and assignment against the new library;
it does not certify ABI compatibility on every platform.

For Clang sanitizer checks, instrument both the library and drivers with
`-fsanitize=address,undefined,function -fno-omit-frame-pointer` and pass the
sanitizer flags through `LDFLAGS` as well. Use `UBSAN_OPTIONS=halt_on_error=1`
for strict checks. Function-pointer identity casts used for removal are never
invoked by the test. The allocation churn check measures live bytes before and
after 100000 registrations (4096-byte tolerance); it is a bounded retention
check, not proof that every path is leak-free.

On the isolated PR branch, strict event-path sanitizer checks can encounter the
SoPath downcasts addressed separately by PR #714. GLX setup can encounter the
pbuffer bug addressed by PR #715. `COIN_GLXGLUE_NO_PBUFFERS=1` selects the pixmap
backend to exercise pre-render dispatch, but does not validate pbuffers. Report
these failures and any recovery/leak settings explicitly rather than treating
an eventual PASS line as a clean sanitizer result.
