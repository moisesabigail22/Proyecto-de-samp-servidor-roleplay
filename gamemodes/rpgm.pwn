/* ============================================================
    RPGM - Gamemode RolePlay Moderno
    FASE 1: Base + MySQL + Registro/Login + Economia basica

    Requiere compilar con estas librerias en /pawno/include:
      - a_samp.inc          (viene con SA-MP)
      - a_mysql.inc         (plugin BlueG mysql R41-4)
      - sscanf2.inc         (plugin sscanf)
      - zcmd.inc            (sistema de comandos, .inc suelto)
      - whirlpool.inc       (hash de contraseñas, .inc suelto)

    Ver README.md para los links de descarga.
   ============================================================ */

#define SSCANF_NO_NICE_FEATURES
#include <a_samp>
#include <zcmd>
#include <whirlpool>

#include "../includes/core/database.inc"
#include "../includes/core/presentation.inc"
#include "../includes/core/accounts.inc"
#include "../includes/core/economy.inc"

#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_GREEN 0x33CC33FF
#define COLOR_RED   0xFF0000FF

main()
{
    print("\n----------------------------------");
    print(" RPGM - Servidor RolePlay Moderno ");
    print("----------------------------------\n");
}

public OnGameModeInit()
{
    SetGameModeText("RPGM v1.0 - RolePlay");
    UsePlayerPedAnims();
    ShowPlayerMarkers(PLAYER_MARKERS_MODE_GLOBAL);
    ShowNameTags(1);
    DisableInteriorEnterExits();

    AddPlayerClass(0, 1958.3783, 1343.1572, 15.3746, 269.1425, 0,0,0,0,0,0);

    ConnectDatabase();
    return 1;
}

public OnGameModeExit()
{
    DisconnectDatabase();
    return 1;
}

public OnPlayerConnect(playerid)
{
    // Congelamos y ponemos en modo espectador visual hasta loguear
    TogglePlayerSpectating(playerid, true);
    PresentationOnPlayerConnect(playerid);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    AccountsOnPlayerDisconnect(playerid);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    if (!PlayerData[playerid][pLoggedIn]) return 0;

    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 0.0);
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (PresentationOnDialogResponse(playerid, dialogid, response))
        return 1;

    if (AccountsOnDialogResponse(playerid, dialogid, response, listitem, inputtext))
        return 1;

    return 0;
}

// -------------------------------------------------------------
// Comandos basicos de ejemplo (mas se agregan en fases siguientes)
// -------------------------------------------------------------
CMD:stats(playerid, params[])
{
    new msg[160];
    format(msg, sizeof(msg), "Nivel: %d | EXP: %d | Admin: %d",
        PlayerData[playerid][pLevel], PlayerData[playerid][pExp], PlayerData[playerid][pAdminLevel]);
    SendClientMessage(playerid, COLOR_WHITE, msg);
    return 1;
}

CMD:ayuda(playerid, params[])
{
    SendClientMessage(playerid, COLOR_WHITE, "== Comandos disponibles ==");
    SendClientMessage(playerid, COLOR_WHITE, "/banco /depositar /retirar /pagar /stats");
    return 1;
}
