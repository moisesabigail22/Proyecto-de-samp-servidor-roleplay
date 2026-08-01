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
#include "../includes/core/accounts.inc"
#include "../includes/core/economy.inc"
#include "../includes/core/roleplay.inc"

#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_GREEN 0x33CC33FF
#define COLOR_RED   0xFF0000FF

main()
{
    print("\n----------------------------------");
    print(" Nova Roleplay - Servidor RolePlay ");
    print("----------------------------------\n");
}

public OnGameModeInit()
{
    SetGameModeText("Nova Roleplay");
    UsePlayerPedAnims();
    ShowPlayerMarkers(PLAYER_MARKERS_MODE_GLOBAL);
    ShowNameTags(1);
    DisableInteriorEnterExits();

    AddPlayerClass(0, 1958.3783, 1343.1572, 15.3746, 269.1425, 0,0,0,0,0,0);

    ConnectDatabase();
    RoleplayOnGameModeInit();
    return 1;
}

public OnGameModeExit()
{
    RoleplayOnGameModeExit();
    DisconnectDatabase();
    return 1;
}

public OnPlayerConnect(playerid)
{
    // Congelamos y ponemos en modo espectador visual hasta loguear
    TogglePlayerSpectating(playerid, true);
    RoleplayOnPlayerConnect(playerid);
    AccountsOnPlayerConnect(playerid);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    RoleplayOnPlayerDisconnect(playerid);
    AccountsOnPlayerDisconnect(playerid);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    SetPlayerHealth(playerid, 100.0);
    SetPlayerArmour(playerid, 0.0);
    if (PlayerData[playerid][pLoggedIn]) RoleplayShowHud(playerid);
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (AccountsOnDialogResponse(playerid, dialogid, response, listitem, inputtext))
        return 1;

    if (RoleplayOnDialogResponse(playerid, dialogid, response, listitem, inputtext))
        return 1;

    return 0;
}
