@setlocal

@set TMPEXE=release\compress.exe
@if NOT EXIST %TMPEXE% goto NOEXE

@set TMPFIL1=bldlog-1.txt
@if NOT EXIST %TMPFIL1% goto NOFIL1
@set TMPFIL2=%TMPFIL1%.Z

@set TMPCNT=0

:RPT

@set TMPSIZ1=

@call :SETFS %TMPFIL1%

@REM echo Size of %TMPFIL1% %TMPSIZ1% bytes
@%TMPEXE% -v %TMPFIL1% 

@if NOT EXIST %TMPFIL2% goto NOFIL2
@call :SETFS %TMPFIL2%

@REM echo Size of %TMPFIL2% %TMPSIZ1% bytes

@%TMPEXE% -vd %TMPFIL2%

@set /A TMPCNT+=1

@if %TMPCNT% LSS 20 goto RPT

@goto END

:NOEXE
@echo Can NOT find EXE %TMPEXE%
@goto END

:NOFIL1
@echo Can NOT find file 1 %TMPFIL1%
@goto END

:NOFIL2
@echo Can NOT find file 2 %TMPFIL2%
@goto END

:SETFS
@set TMPSIZ1=%~z1
@goto :EOF


:END
