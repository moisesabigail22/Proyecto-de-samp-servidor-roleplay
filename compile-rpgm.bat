@echo off
rem Compile Nova Roleplay with the SA-MP/Pawno compiler that ships in this repo.
rem Run this on Windows before starting samp-server.exe so gamemodes\rpgm.amx
rem is regenerated from gamemodes\rpgm.pwn and does not throw runtime error 17.

cd /d "%~dp0"
pawno\pawncc.exe gamemodes\rpgm.pwn -;+ -(+ -iinclude -ipawno\include -iincludes\core -ogamemodes\rpgm.amx
if errorlevel 1 (
    echo.
    echo ERROR: No se pudo compilar gamemodes\rpgm.pwn. Revisa los mensajes de pawncc.
    exit /b 1
)
python tools\check_amx.py gamemodes\rpgm.amx 2>nul
if errorlevel 1 (
    echo.
    echo ADVERTENCIA: no se pudo validar con Python, pero pawncc termino correctamente.
)
echo.
echo OK: gamemodes\rpgm.amx actualizado. Ahora puedes iniciar samp-server.exe.
