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
performed locally: this Linux host has neither Windows nor Wine. The Windows
CTest entry is available to the existing Windows CI build.

## References

- [GetTextFaceA contract](https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-gettextfacea)
- [GetVersionExA behavior and manifest dependence](https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getversionexa)
- Personal study: `Estudo coin/estudos/So/SoFonts.md`, native font backend and fallback architecture.
