# Native Windows 3.1 Port

This builds the original Microsoft C modules with OpenWatcom's 16-bit Windows
SDK. It is not a new file manager and does not require Win32s.

From the repository root:

```sh
make -f win16/Makefile WATCOM=/opt/watcom core
make -f win16/Makefile WATCOM=/opt/watcom JWLINK=/path/to/jwlink core-test
```

Outputs default to `bld/win16/`. Override `OUT` for a parent-project build.
`WINFILE.LIB` contains the original filename canonicalization/rename masks,
linked directory entries and 64-bit size formatter. `WFCORE.EXE` executes 17
native tests and writes `C:\WFPORT.LOG`; it is not an application release.
The test uses test-only size-unit strings, not a complete localized UI.

## Platform Boundary

`CALMIRA_WIN16` selects ANSI Win16 declarations in `platform.h`. The build
requires a 16-bit large memory model: 16-bit integers and far data pointers.
`memory.c` supplies global-memory allocation for the original linked directory
blocks; near `PBYTE` arithmetic is replaced by `LPBYTE`. Original Microsoft
copyright/MIT notices and the algorithms remain in their existing source files.
Only the unused historical `ach83InvalidChars` warning is suppressed; other
compiler warnings remain errors.

## Incomplete Application

`make -f win16/Makefile application` attempts the original entry point, source
modules, resources and native NE link. It still fails on unported Win32 SDK
dependencies. `#define WIN31` in the historical header is not a usable target
selector. No successful Win16 application build or GUI/file-operation
acceptance is claimed yet. Do not replace Windows' WINFILE.EXE with the test
runner or with a separate drive-list demo.
