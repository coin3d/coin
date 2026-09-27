# Windows GDI fonts without OS version queries (issue #136)

Issue: https://github.com/coin3d/coin/issues/136

The OS query existed solely to distinguish Windows 95/98/Me from NT-family
Windows in two font operations:

- `GetTextFace(hdc, 0, NULL)` returning zero on Windows 9x was replaced with a
  guessed 1024-character allocation. On NT-family Windows the API reports
  the required name length, including its terminating null character.
- Closing a glyph contour added an extra endpoint only for Windows 9x. The
  existing NT path closes directly at the starting point; inserting the
  additional point there was documented as causing a gap in extruded text.

Removing the two legacy alternatives makes `GetVersionEx`, its wrapper and
its function-table entry unnecessary. The changed table is private and its
header is not installed. Public declarations are unchanged. Error diagnostics
for failing GDI calls remain in place. Historical comments about Win9x and
local glyph transformation matrices were removed without changing the matrices.

## Historical origin

The compatibility paths were deliberate fixes for then-supported Windows 9x,
not unreachable leftovers. The surviving Git history contains these changes:

| Date (UTC) | Commit | Reason and effect |
| --- | --- | --- |
| 2003-11-13 18:40:19 | [7a6948f5af](https://github.com/coin3d/coin/commit/7a6948f5afe8562e429a3d560af03feff0568319) | Handegard adds the `GetVersionEx` binding and a workaround for `GetTextFace(hdc, ..., NULL)` returning zero on Win95/98/Me. The wrapper checks `dwPlatformId` and supplies a guessed buffer size of 1024. |
| 2003-11-13 18:41:13 | [6e220994de](https://github.com/coin3d/coin/commit/6e220994deaf35ddc13f35297d217f8259fd6caf) | Handegard adds a font-name truncation diagnostic and changes two identity matrices from static to local after reporting Win9x failures with static storage. |
| 2003-11-14 09:17:05 | [8b2d2d3ce9](https://github.com/coin3d/coin/commit/8b2d2d3ce95cdee032d8b1a797e91b8e254d54da) | Morten Eriksen simplifies the version wrapper from `BOOL` to `void`, moves error handling inside it and restricts it to `OSVERSIONINFO`. Comments explain that `VerifyVersionInfo` cannot serve the Win9x compatibility purpose. |
| 2003-11-17 17:59:27 | [8352e40eff](https://github.com/coin3d/coin/commit/8352e40eff5cf9eef7ac47581ca71399ee92bb9e) | Handegard adds the startup Win9x flag and an extra vertex when closing glyph contours. The commit reports a missing vertex on Win95/98/Me; its comment says adding that vertex on NT/2000/XP causes a gap in extruded glyphs. |
| 2003-11-17 18:11:05 | [cee2d3dde7](https://github.com/coin3d/coin/commit/cee2d3dde78206244fba1a68cec2015e378732c9) | Corrects the font initializer to call the now-void wrapper without testing its return value. |
| 2003-11-18 | [c36482b06e](https://github.com/coin3d/coin/commit/c36482b06e6c6adcd02db07941da0c1c35b62d96) | Adds a FIXME questioning why matrix storage duration would affect Win9x, and cleans up contour processing. This records uncertainty about the matrix cause even at the time. |
| 2007-12-14 | [7f2e6d1f38](https://github.com/coin3d/coin/commit/7f2e6d1f382d81591de1e8750036c5c6258fc0a8) | Renames the implementations from `.c` to `.cpp` and moves private headers from `include/Inventor/C` to `src`. The version query predates this migration. |
| 2017-09-26 | [Issue #136](https://github.com/coin3d/coin/issues/136) | Bastiaan Veelo reports deprecation warnings with MSVC 2013+, changed version-query behavior on Windows 8.1+, and proposes ending Win9x compatibility after the next major release. |
| 2026-09-27 | `c0212b4d8d` | Removes the obsolete alternatives, the version-query wrapper and its private table entry; retains the existing NT-family contour behavior and GDI error diagnostics. |

The old author name is stored in Git with an encoding artifact as
`?ystein Handegard`; the commit links above identify the original records.
The repository also contains a January 2004 recovery commit mentioning history
lost in a disk crash. This account describes the surviving evidence rather
than claiming a complete external bug-report archive.

### What the evidence establishes

The first version query had a specific purpose: distinguish the Win9x family
(`VER_PLATFORM_WIN32_WINDOWS`) when a name-length query failed. Four days
later, the same distinction was reused for glyph contour closure. Neither
operation needed to discriminate Windows 8.1 from Windows 10 or other recent
NT releases.

The current Microsoft documentation says `GetVersionEx` version values depend
on application manifests starting with Windows 8.1. That motivates avoiding
version-based policy, but it does not demonstrate that these particular
`dwPlatformId` checks malfunctioned on modern Windows. The confirmed problem
is an obsolete API dependency and retained compatibility machinery for Win9x.
`VerifyVersionInfo` appeared in an explanatory comment, not as another live
query in these wrappers.

No archived Win9x environment was executed during this investigation. Claims
about its GDI failures and extrusion gaps come from contemporaneous commit
messages and code comments. Native testing in the dual-boot Windows session
can validate the retained behavior on the installed Windows version; it would
not reproduce Windows 9x behavior.

To inspect the original changes, include the old `.c` paths:

```sh
git show 7a6948f5af -- src/glue/win32api.c
git show 8b2d2d3ce9 -- src/glue/win32api.c
git show 8352e40eff -- src/fonts/win32.c
git show cee2d3dde7 -- src/fonts/win32.c
git log --follow -S 'coin_GetVersionEx' -- src/glue/win32api.cpp
```

## Validation

`Win32FontApiTest.cpp` is a Windows-only CTest executable. It compiles the
private GDI wrapper locally, then queries a memory DC with two stock fonts.
It compares sizing and name retrieval against the native GDI API, verifies
null termination and a canary beyond the requested buffer, restores the
selected font and releases the DC. It requires no OpenGL context.

On Windows, with a configured test build:

```sh
cmake --build build --target CoinWin32FontApiTest
ctest --test-dir build -C Release -R '^Win32FontApi$' --output-on-failure
```

Local validation on Linux used Zig 0.15.2 to generate Windows x64 COFF
objects with real Windows SDK headers:

- `src/glue/win32api.cpp`, ANSI;
- `src/fonts/win32.cpp`, ANSI;
- `testsuite/Win32FontApiTest.cpp`, ANSI;
- `src/glue/win32api.cpp` and the test, Unicode with DLL import declarations.

The backend's normal ANSI configuration was compiled; Unicode validation
covers the wrapper and test only. Generated public headers were supplied
from an existing configured Coin build, with `HAVE_WIN32_API` and
`HAVE_WINDOWS_H` explicitly enabled and without importing Linux config.h.

All five object compilations passed. Object symbol inspection confirms that
`win32api.cpp` references `GetTextFaceA` but no version-query API. A source
search finds no `GetVersionEx`, `VerifyVersionInfo`, `OSVERSIONINFO`, or Win9x
flag in the three modified Windows files. `git diff --check` passed.

Native GDI runtime execution and visual glyph extrusion validation were not
performed in this Linux session. Windows is available through dual boot,
but was not booted for this validation; Wine is not installed. The Windows
CTest entry is available for that session and the existing Windows CI build.

## References

- [GetTextFaceA contract](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-gettextfacea)
- [GetVersionExA behavior and manifest dependence](https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getversionexa)
- Personal study: `Estudo coin/estudos/So/SoFonts.md`, native font backend and fallback architecture.
