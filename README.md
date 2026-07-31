# RPGM - Servidor RolePlay Moderno para SA-MP

Base de servidor roleplay en Pawn para SA-MP, con conexión MySQL, registro/login, economía básica y una presentación inicial para orientar al jugador apenas entra al servidor.

## Estado actual

- Gamemode principal: `gamemodes/rpgm.pwn`.
- Conexión MySQL modular: `includes/core/database.inc`.
- Sistema de cuentas con registro/login: `includes/core/accounts.inc`.
- Economía inicial: `/banco`, `/depositar`, `/retirar`, `/pagar`.
- Presentación de bienvenida: mensajes iniciales, cámara de presentación y diálogo con reglas/comandos básicos.

## Requisitos

Colocá estos includes/plugins antes de compilar y ejecutar:

- SA-MP server 0.3.7.
- BlueG MySQL R41-4 (`a_mysql.inc` + plugin `mysql`).
- sscanf (`sscanf2.inc` + plugin `sscanf`).
- ZCMD (`zcmd.inc`).
- Whirlpool (`whirlpool.inc` + plugin `whirlpool`).
- Streamer y CrashDetect si los vas a mantener en `server.cfg`.

## Instalación rápida

1. Importá la base de datos:

   ```sql
   SOURCE scriptfiles/database/schema.sql;
   ```

2. Editá las credenciales en `includes/core/database.inc`:

   ```pawn
   #define DB_HOST     "127.0.0.1"
   #define DB_USER     "root"
   #define DB_PASS     ""
   #define DB_NAME     "samp_rp"
   #define DB_PORT     3306
   ```

3. Verificá `server.cfg`:

   ```cfg
   hostname [ES] Servidor RolePlay Moderno
   gamemode0 rpgm 1
   plugins mysql streamer sscanf crashdetect whirlpool
   ```

4. Compilá `gamemodes/rpgm.pwn` con Pawno o `pawncc`.
5. Iniciá el servidor y entrá con tu cliente SA-MP.

## Flujo de entrada del jugador

1. El jugador conecta y queda en modo espectador con una cámara de presentación.
2. El sistema revisa MySQL para saber si la cuenta existe.
3. Si existe, muestra login. Si no existe, muestra registro.
4. Al iniciar sesión o registrarse, se muestra la presentación con reglas, comandos y primeros pasos.
5. El jugador puede usar `/ayuda`, `/stats`, `/banco`, `/depositar`, `/retirar` y `/pagar`.

## Próximas mejoras sugeridas

- Selector de skin al registrarse.
- Spawn por facción/trabajo.
- Sistema de tutorial por checkpoints.
- Guardado de vida, chaleco y skin al desconectar.
- Panel administrativo básico.
