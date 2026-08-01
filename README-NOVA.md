# Nova Roleplay - despliegue del gamemode

Si el servidor muestra:

```text
Script[gamemodes/rpgm.amx]: Run time error 17: "Invalid/unsupported P-code file format"
Failed to load 'gamemodes/rpgm.amx' script.
```

significa que `gamemodes/rpgm.amx` no fue compilado con un compilador compatible
o quedó corrupto al copiarlo/subirlo. El servidor SA-MP carga archivos `.amx`, no
`.pwn`; por eso después de cambiar el código hay que recompilar.

## Windows

Ejecuta **este archivo**, no `pawno\pawncc.exe` directamente:

```bat
compile-rpgm.bat
```

El compilador escribira detalles en `compile-rpgm.log`. Si la ventana se queda solo en `Pawn compiler 3.2.3664`, normalmente abriste `pawno\pawncc.exe` sin pasarle `gamemodes\rpgm.pwn`; cierra esa ventana y ejecuta `compile-rpgm.bat` desde la carpeta principal.

Luego inicia:

```bat
samp-server.exe
```

## Validar el AMX antes de subirlo

```bash
python3 tools/check_amx.py gamemodes/rpgm.amx
```

Debe imprimir `OK` con `magic=0xF1E0` y versiones `8/8`. Si imprime `FAIL`, vuelve
a compilar con `compile-rpgm.bat` y sube el `.amx` nuevo junto con el `.pwn`.
