@echo off

git describe --always --dirty --abbrev=7 > version_git.h.tmp


set dirty=
findstr dirty version_git.h.tmp > NUL

if %errorlevel% == 0 (
	echo WARNING! Local modification detected!
    set dirty=D
)

set /p hash= < version_git.h.tmp
set hash=%hash:~0,7%

(
@echo #ifndef __GIT_VERSION_HEADER__
@echo #define __GIT_VERSION_HEADER__
@echo.
@echo #define GIT_VERSION 0x%dirty%%hash%
@echo.
@echo #endif
)> "version_git.h.tmp"

fc "version_git.h.tmp" "..\version_git.h" > nul
if errorlevel 1 goto error
if errorlevel 2 goto error

del "version_git.h.tmp"
goto finish

:error
move "version_git.h.tmp" "..\version_git.h" > nul

:finish