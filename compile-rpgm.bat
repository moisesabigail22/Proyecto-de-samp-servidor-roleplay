@echo off
setlocal EnableExtensions

rem ============================================================
rem Nova Roleplay - Compilador del gamemode RPGM
rem ============================================================
rem Usa este archivo desde la carpeta principal del servidor.
rem No abras pawno\pawncc.exe directo.
rem
rem Esta version usa rutas RELATIVAS y cortas porque el pawncc viejo
rem de SA-MP puede quedarse pegado con rutas absolutas muy largas.
rem ============================================================

cd /d "%~dp0"
set "LOG=compile-rpgm.log"

echo ============================================================
echo Nova Roleplay - Compilando gamemodes\rpgm.pwn
echo ============================================================
echo.

if not exist "pawno\pawncc.exe" (
    echo ERROR: No existe pawno\pawncc.exe
    echo Debe estar dentro de la carpeta pawno del servidor.
    pause
    exit /b 1
)

if not exist "gamemodes\rpgm.pwn" (
    echo ERROR: No existe gamemodes\rpgm.pwn
    pause
    exit /b 1
)

if exist "%LOG%" del /f /q "%LOG%" >nul 2>nul

echo Usando rutas cortas para evitar que pawncc se quede pegado.
echo Compilador: pawno\pawncc.exe
echo Fuente:     gamemodes\rpgm.pwn
echo Salida:     gamemodes\rpgm.amx
echo Log:        %LOG%
echo.
echo Si aparece un error, quedara guardado en %LOG%.
echo.

pushd pawno
pawncc.exe "..\gamemodes\rpgm.pwn" "-i..\include" "-iinclude" "-i..\includes\core" "-o..\gamemodes\rpgm.amx" > "..\%LOG%" 2>&1
set "COMPILE_RESULT=%ERRORLEVEL%"
popd

echo -------------------- compile-rpgm.log --------------------
if exist "%LOG%" (
    type "%LOG%"
) else (
    echo No se genero log.
)
echo ----------------------------------------------------------
echo.

if not "%COMPILE_RESULT%"=="0" (
    echo ERROR: pawncc fallo con codigo %COMPILE_RESULT%.
    pause
    exit /b %COMPILE_RESULT%
)

if not exist "gamemodes\rpgm.amx" (
    echo ERROR: No se genero gamemodes\rpgm.amx.
    pause
    exit /b 1
)

echo OK: gamemodes\rpgm.amx compilado correctamente.
echo Ahora inicia samp-server.exe.
echo.
pause
exit /b 0
