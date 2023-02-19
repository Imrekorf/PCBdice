@echo off

FOR /F "tokens=* USEBACKQ" %%F IN (`git rev-parse --short HEAD`) DO (
SET GIT_V=%%F
)

FOR /F "tokens=* USEBACKQ" %%F IN (`git describe --all --dirty`) DO (
SET GIT_DIRTY=%%F
)

set str1=%GIT_DIRTY
if not x%str1:dirty=%==x%str1% goto dirty
set GIT_DIRTY=0
goto clean
:dirty
set GIT_DIRTY=1
:clean

(
@echo #ifndef __GIT_VERSION_HEADER__
@echo #define __GIT_VERSION_HEADER__
@echo.
@echo #define GIT_VERSION "%GIT_V%"
@echo #define GIT_DIRTY   %GIT_DIRTY%
@echo.
@echo #endif
)> "version_git.h.tmp"

fc "version_git.h.tmp" "..\version_git.h" > nul
if errorlevel 1 goto error
if errorlevel 2 goto error

echo "No revision changes"
del "version_git.h.tmp"
goto finish

:error
echo "New git revision"
move "version_git.h.tmp" "..\version_git.h" > nul

:finish