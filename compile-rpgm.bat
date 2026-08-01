@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem ============================================================
rem Nova Roleplay - Compilar gamemode RPGM
rem ============================================================
rem Ejecuta este BAT desde la carpeta principal del servidor.
rem Muestra progreso, escribe compile-rpgm.log y corta si pawncc
rem queda trabado demasiado tiempo.
rem ============================================================

cd /d "%~dp0"
set "LOG=compile-rpgm.log"
set "DONE=%TEMP%\nova_rpgm_compile_done.txt"
set "RESULT=%TEMP%\nova_rpgm_compile_result.txt"
set "TIMEOUT_SECONDS=180"

if exist "%DONE%" del /f /q "%DONE%" >nul 2>nul
if exist "%RESULT%" del /f /q "%RESULT%" >nul 2>nul
if exist "%LOG%" del /f /q "%LOG%" >nul 2>nul

echo ============================================================
echo Nova Roleplay - Compilando gamemodes\rpgm.pwn
echo ============================================================
echo.

if not exist "pawno\pawncc.exe" (
    echo ERROR: falta pawno\pawncc.exe
    pause
    exit /b 1
)

if not exist "gamemodes\rpgm.pwn" (
    echo ERROR: falta gamemodes\rpgm.pwn
    pause
    exit /b 1
)

echo No se cambio el nombre del archivo del gamemode: sigue siendo rpgm.
echo Compilando con rutas cortas para evitar cuelgues por rutas largas.
echo Log: %LOG%
echo.

set "WORKER=%TEMP%\nova_rpgm_compile_worker.bat"
> "%WORKER%" echo @echo off
>> "%WORKER%" echo cd /d "%%CD%%\pawno"
>> "%WORKER%" echo pawncc.exe "..\gamemodes\rpgm.pwn" "-i..\include" "-iinclude" "-i..\includes\core" "-o..\gamemodes\rpgm.amx" ^> "..\%LOG%" 2^>^&1
>> "%WORKER%" echo echo %%ERRORLEVEL%% ^> "%RESULT%"
>> "%WORKER%" echo echo done ^> "%DONE%"

start "NovaPawnCompile" /min cmd /c ""%WORKER%""

set /a ELAPSED=0
:WAIT_COMPILE
if exist "%DONE%" goto COMPILE_FINISHED

set /a ELAPSED+=1
<nul set /p "=Compilando... !ELAPSED!/!TIMEOUT_SECONDS! segundos`r"
timeout /t 1 /nobreak >nul

if !ELAPSED! GEQ !TIMEOUT_SECONDS! (
    echo.
    echo ERROR: pawncc tardo mas de !TIMEOUT_SECONDS! segundos y fue detenido.
    taskkill /f /im pawncc.exe >nul 2>nul
    echo Revisa %LOG%. Si esta vacio, mueve la carpeta del servidor a una ruta corta como C:\NovaRP.
    pause
    exit /b 1
)
goto WAIT_COMPILE

:COMPILE_FINISHED
echo.
set /p COMPILE_RESULT=<"%RESULT%"

echo -------------------- compile-rpgm.log --------------------
if exist "%LOG%" type "%LOG%"
echo ----------------------------------------------------------
echo.

if not "%COMPILE_RESULT%"=="0" (
    echo ERROR: pawncc fallo con codigo %COMPILE_RESULT%.
    pause
    exit /b %COMPILE_RESULT%
)

if not exist "gamemodes\rpgm.amx" (
    echo ERROR: no se genero gamemodes\rpgm.amx.
    pause
    exit /b 1
)

echo OK: gamemodes\rpgm.amx compilado correctamente.
echo Ahora puedes iniciar samp-server.exe.
pause
exit /b 0
