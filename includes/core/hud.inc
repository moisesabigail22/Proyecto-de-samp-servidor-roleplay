/* ============================================================
    hud.inc
    Barra inferior con datos del jugador y servidor
   ============================================================ */
#if defined _hud_inc_included
    #endinput
#endif
#define _hud_inc_included

#include <a_samp>

#define SERVER_DISPLAY_NAME "Gran de Auto San Andreas"
#define INVALID_HUD_TD PlayerText:INVALID_TEXT_DRAW

new PlayerText:gHudBar[MAX_PLAYERS];
new PlayerText:gHudLeft[MAX_PLAYERS];
new PlayerText:gHudCenter[MAX_PLAYERS];
new PlayerText:gHudRight[MAX_PLAYERS];

stock HudResetPlayer(playerid)
{
    gHudBar[playerid] = INVALID_HUD_TD;
    gHudLeft[playerid] = INVALID_HUD_TD;
    gHudCenter[playerid] = INVALID_HUD_TD;
    gHudRight[playerid] = INVALID_HUD_TD;
    return 1;
}

stock ShowPlayerHud(playerid)
{
    DestroyPlayerHud(playerid);

    gHudBar[playerid] = CreatePlayerTextDraw(playerid, 320.0, 430.0, "_");
    PlayerTextDrawLetterSize(playerid, gHudBar[playerid], 0.0, 2.2);
    PlayerTextDrawTextSize(playerid, gHudBar[playerid], 640.0, 0.0);
    PlayerTextDrawAlignment(playerid, gHudBar[playerid], 2);
    PlayerTextDrawUseBox(playerid, gHudBar[playerid], 1);
    PlayerTextDrawBoxColor(playerid, gHudBar[playerid], 0x000000CC);
    PlayerTextDrawShow(playerid, gHudBar[playerid]);

    gHudLeft[playerid] = CreatePlayerTextDraw(playerid, 12.0, 432.0, "_");
    PlayerTextDrawLetterSize(playerid, gHudLeft[playerid], 0.25, 1.2);
    PlayerTextDrawAlignment(playerid, gHudLeft[playerid], 1);
    PlayerTextDrawColor(playerid, gHudLeft[playerid], 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, gHudLeft[playerid], 1);
    PlayerTextDrawFont(playerid, gHudLeft[playerid], 2);
    PlayerTextDrawShow(playerid, gHudLeft[playerid]);

    gHudCenter[playerid] = CreatePlayerTextDraw(playerid, 320.0, 432.0, "_");
    PlayerTextDrawLetterSize(playerid, gHudCenter[playerid], 0.28, 1.2);
    PlayerTextDrawAlignment(playerid, gHudCenter[playerid], 2);
    PlayerTextDrawColor(playerid, gHudCenter[playerid], 0xB33CFFFF);
    PlayerTextDrawSetOutline(playerid, gHudCenter[playerid], 1);
    PlayerTextDrawFont(playerid, gHudCenter[playerid], 2);
    PlayerTextDrawShow(playerid, gHudCenter[playerid]);

    gHudRight[playerid] = CreatePlayerTextDraw(playerid, 628.0, 432.0, "_");
    PlayerTextDrawLetterSize(playerid, gHudRight[playerid], 0.25, 1.2);
    PlayerTextDrawAlignment(playerid, gHudRight[playerid], 3);
    PlayerTextDrawColor(playerid, gHudRight[playerid], 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, gHudRight[playerid], 1);
    PlayerTextDrawFont(playerid, gHudRight[playerid], 2);
    PlayerTextDrawShow(playerid, gHudRight[playerid]);

    UpdatePlayerHud(playerid);
    return 1;
}

stock DestroyPlayerHud(playerid)
{
    if (gHudBar[playerid] != INVALID_HUD_TD) PlayerTextDrawDestroy(playerid, gHudBar[playerid]);
    if (gHudLeft[playerid] != INVALID_HUD_TD) PlayerTextDrawDestroy(playerid, gHudLeft[playerid]);
    if (gHudCenter[playerid] != INVALID_HUD_TD) PlayerTextDrawDestroy(playerid, gHudCenter[playerid]);
    if (gHudRight[playerid] != INVALID_HUD_TD) PlayerTextDrawDestroy(playerid, gHudRight[playerid]);
    HudResetPlayer(playerid);
    return 1;
}

stock GetPlayerSimpleZone(playerid, dest[], size = sizeof(dest))
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    if (x > 44.0 && x < 2997.0 && y > -2892.0 && y < -768.0)
        format(dest, size, "Los Santos");
    else if (x > 869.0 && x < 2997.0 && y > 596.0 && y < 2993.0)
        format(dest, size, "Las Venturas");
    else if (x > -2997.0 && x < -1213.0 && y > -1115.0 && y < 1659.0)
        format(dest, size, "San Fierro");
    else
        format(dest, size, "San Andreas");
    return 1;
}

stock UpdatePlayerHud(playerid)
{
    if (!IsPlayerConnected(playerid) || !PlayerData[playerid][pLoggedIn]) return 1;
    if (gHudBar[playerid] == INVALID_HUD_TD) return 1;

    new name[MAX_PLAYER_NAME], zone[32], left[96], center[96], right[96];
    new hour, minute, second;
    GetPlayerName(playerid, name, sizeof(name));
    GetPlayerSimpleZone(playerid, zone, sizeof(zone));
    gettime(hour, minute, second);

    format(left, sizeof(left), "%s | ID:%d", name, playerid);
    format(center, sizeof(center), "%s | %s", SERVER_DISPLAY_NAME, zone);
    format(right, sizeof(right), "Nivel:%d | %02d:%02d", PlayerData[playerid][pLevel], hour, minute);

    PlayerTextDrawSetString(playerid, gHudLeft[playerid], left);
    PlayerTextDrawSetString(playerid, gHudCenter[playerid], center);
    PlayerTextDrawSetString(playerid, gHudRight[playerid], right);
    return 1;
}

forward UpdateServerHud();
public UpdateServerHud()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsPlayerConnected(i)) UpdatePlayerHud(i);
    }
    return 1;
}
