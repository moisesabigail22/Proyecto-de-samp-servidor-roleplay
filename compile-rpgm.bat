@echo off
setlocal EnableExtensions

rem ============================================================
rem Nova Roleplay - Compilador del gamemode RPGM
rem ============================================================
rem IMPORTANTE:
rem  - Ejecuta este archivo desde la carpeta principal del servidor.
rem  - NO abras pawno\pawncc.exe directamente, porque se queda mostrando
rem    solo el encabezado del compilador si no recibe el .pwn correcto.
rem  - Este BAT genera gamemodes\rpgm.amx desde gamemodes\rpgm.pwn.
rem ============================================================

cd /d "%~dp0"
set "ROOT=%CD%"
set "LOG=%ROOT%\compile-rpgm.log"
set "PAWNCC=%ROOT%\pawno\pawncc.exe"
set "SOURCE=%ROOT%\gamemodes\rpgm.pwn"
set "OUTPUT=%ROOT%\gamemodes\rpgm.amx"

echo ============================================================
echo Nova Roleplay - Compilando gamemodes\rpgm.pwn
echo ============================================================
echo.

if not exist "%PAWNCC%" (
    echo ERROR: No existe "%PAWNCC%".
    echo Revisa que la carpeta pawno este dentro del servidor.
    pause
    exit /b 1
)

if not exist "%SOURCE%" (
    echo ERROR: No existe "%SOURCE%".
    echo Revisa que gamemodes\rpgm.pwn este dentro del servidor.
    pause
    exit /b 1
)

if exist "%LOG%" del /f /q "%LOG%" >nul 2>nul

echo Usando: "%PAWNCC%"
echo Fuente: "%SOURCE%"
echo Salida: "%OUTPUT%"
echo Log:    "%LOG%"
echo.
echo Espera... si falla, se mostrara el contenido de compile-rpgm.log.
echo.

rem Se ejecuta desde pawno para que pawncc encuentre sus archivos internos.
pushd "%ROOT%\pawno"
"%PAWNCC%" "%SOURCE%" "-i%ROOT%\include" "-i%ROOT%\pawno\include" "-i%ROOT%\includes\core" "-o%OUTPUT%" > "%LOG%" 2>&1
set "COMPILE_RESULT=%ERRORLEVEL%"
popd

if not "%COMPILE_RESULT%"=="0" (
    echo ERROR: pawncc no pudo compilar. Codigo: %COMPILE_RESULT%
    echo -------------------- compile-rpgm.log --------------------
    type "%LOG%"
    echo ----------------------------------------------------------
    pause
    exit /b %COMPILE_RESULT%
)

if not exist "%OUTPUT%" (
    echo ERROR: pawncc termino pero no se genero "%OUTPUT%".
    echo -------------------- compile-rpgm.log --------------------
    type "%LOG%"
    echo ----------------------------------------------------------
    pause
    exit /b 1
)

echo OK: gamemodes\rpgm.amx fue compilado correctamente.
echo Ahora inicia samp-server.exe.
echo.
pause
exit /b 0
