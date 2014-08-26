@setlocal

@set TMPPRJ=compress
@set TMPSRC=..
@set TMPOPTS=
@set TMPLOG=bldlog-1.txt

call chkmsvc %TMPPRJ%
@echo Bgn %DATE% %TIME% > %TMPLOG%

@echo Doing 'cmake %TMPSRC% %TMPOPTS%' to %TMPLOG%
@echo Doing 'cmake %TMPSRC% %TMPOPTS%' >> %TMPLOG%
@cmake %TMPSRC% %TMPOPTS% >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR1

@echo Doing 'cmake --build . --config debug'
@echo Doing 'cmake --build . --config debug' >> %TMPLOG%
@cmake --build . --config debug >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR2

@echo Doing: 'cmake --build . --config release'
@echo Doing: 'cmake --build . --config release' >> %TMPLOG%
@cmake --build . --config release >> %TMPLOG% 2>&1
@if ERRORLEVEL 1 goto ERR3

@echo.
@echo Appears a successful build...
@echo.
@echo No install at this time...
@echo.

@goto END

:ERR1
@echo cmake config, generation error
@goto ISERR

:ERR2
@echo debug build error
@goto ISERR

:ERR3
@echo release build error
@goto ISERR

:ISERR
@endlocal
@exit /b 1

:END
@endlocal
@exit /b 0

@REM eof
