/* ============================================================
    Nova Roleplay - Sistemas base
    Interfaz, registro de personaje, tiendas, telefono,
    amigos, administracion, hambre y niveles.
   ============================================================ */
#if defined _roleplay_inc_included
    #endinput
#endif
#define _roleplay_inc_included

#include <a_samp>
#include <zcmd>
#include <sscanf2>
#include <streamer>
#include "database.inc"
#include "accounts.inc"

#define DIALOG_PROFILE 200
#define DIALOG_HELP    201
#define DIALOG_SHOP    202
#define DIALOG_FRIENDS 203
#define DIALOG_ADMIN   204

#define COLOR_NOVA  0x8A2BE2FF
#define COLOR_INFO  0xF5F5F5FF
#define COLOR_WARN  0xFFAA00FF
#define COLOR_ERROR 0xFF4444FF
#define MAX_ZONE_NAME 32
#define MAX_FRIEND_SLOTS 10

new PlayerText:NovaHudBar[MAX_PLAYERS];
new PlayerText:NovaHudInfo[MAX_PLAYERS];
new PlayerText:NovaHudZone[MAX_PLAYERS];
new PlayerText:NovaHudNeeds[MAX_PLAYERS];
new PlayerText:NovaProfilePreview[MAX_PLAYERS];
new NovaFriends[MAX_PLAYERS][MAX_FRIEND_SLOTS];
new NovaTimer;

stock NovaGetZone(playerid, zone[], size)
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    if (x > 76.0 && x < 340.0 && y > -220.0 && y < 50.0)
        format(zone, size, "Blueberry");
    else if (x > 44.0 && x < 2997.0 && y > -2892.0 && y < -768.0)
        format(zone, size, "Los Santos");
    else if (x > -2997.0 && x < -1213.0 && y > -1115.0 && y < 1659.0)
        format(zone, size, "San Fierro");
    else if (x > 869.0 && x < 2997.0 && y > 596.0 && y < 2997.0)
        format(zone, size, "Las Venturas");
    else if (x > -1213.0 && x < 869.0 && y > -768.0 && y < 596.0)
        format(zone, size, "Red County");
    else
        format(zone, size, "San Andreas");

    return 1;
}

stock RoleplayOnGameModeInit()
{
    SetGameModeText("Nova Roleplay");
    SendRconCommand("hostname Nova Roleplay | Servidor RolePlay");

    NovaCreateMapPoint(1315.45, -898.13, 39.58, "24/7 Los Santos", "/comprar");
    NovaCreateMapPoint(-2442.61, 755.43, 35.17, "24/7 San Fierro", "/comprar");
    NovaCreateMapPoint(2194.38, 1991.05, 12.30, "24/7 Las Venturas", "/comprar");

    NovaCreateMapPoint(1128.92, -1458.11, 15.79, "Electronica Los Santos", "/telefono /radio");
    NovaCreateMapPoint(-1987.20, 285.76, 35.17, "Electronica San Fierro", "/telefono /radio");
    NovaCreateMapPoint(2020.40, 1007.84, 10.82, "Electronica Las Venturas", "/telefono /radio");

    NovaCreateMapPoint(542.52, -1293.64, 17.24, "Concesionario Los Santos", "/comprarveh");
    NovaCreateMapPoint(-1954.25, 300.25, 35.46, "Concesionario San Fierro", "/comprarveh");
    NovaCreateMapPoint(2128.52, 1408.30, 10.82, "Concesionario Las Venturas", "/comprarveh");

    NovaTimer = SetTimer("NovaMinuteTimer", 60000, true);
    return 1;
}

stock NovaCreateMapPoint(Float:x, Float:y, Float:z, title[], command[])
{
    new label[128];
    format(label, sizeof(label), "{FFFFFF}%s\n{8A2BE2}%s", title, command);
    CreatePickup(1239, 1, x, y, z, -1);
    CreateDynamic3DTextLabel(label, 0xFFFFFFFF, x, y, z, 20.0);
    return 1;
}

stock RoleplayOnGameModeExit()
{
    if (NovaTimer) KillTimer(NovaTimer);
    return 1;
}

stock RoleplayOnPlayerConnect(playerid)
{
    pAge[playerid] = 18;
    pSex[playerid] = 1;
    pCity[playerid] = 1;
    pPhone[playerid] = 0;
    pHunger[playerid] = 100;
    pHasPhone[playerid] = false;
    pHasRadio[playerid] = false;

    for (new i = 0; i < MAX_FRIEND_SLOTS; i++)
        NovaFriends[playerid][i] = INVALID_PLAYER_ID;

    NovaCreateHud(playerid);
    return 1;
}

stock RoleplayOnPlayerDisconnect(playerid)
{
    PlayerTextDrawDestroy(playerid, NovaHudBar[playerid]);
    PlayerTextDrawDestroy(playerid, NovaHudInfo[playerid]);
    PlayerTextDrawDestroy(playerid, NovaHudZone[playerid]);
    PlayerTextDrawDestroy(playerid, NovaHudNeeds[playerid]);
    return 1;
}

stock NovaCreateHud(playerid)
{
    NovaHudBar[playerid] = CreatePlayerTextDraw(playerid, 0.0, 426.0, "_");
    PlayerTextDrawUseBox(playerid, NovaHudBar[playerid], 1);
    PlayerTextDrawBoxColor(playerid, NovaHudBar[playerid], 0x000000DD);
    PlayerTextDrawTextSize(playerid, NovaHudBar[playerid], 640.0, 0.0);
    PlayerTextDrawLetterSize(playerid, NovaHudBar[playerid], 0.5, 2.0);

    NovaHudInfo[playerid] = CreatePlayerTextDraw(playerid, 8.0, 431.0, "Nova Roleplay");
    PlayerTextDrawFont(playerid, NovaHudInfo[playerid], 2);
    PlayerTextDrawLetterSize(playerid, NovaHudInfo[playerid], 0.20, 1.0);
    PlayerTextDrawColor(playerid, NovaHudInfo[playerid], 0xFFFFFFFF);

    NovaHudZone[playerid] = CreatePlayerTextDraw(playerid, 270.0, 431.0, "Ubicacion");
    PlayerTextDrawFont(playerid, NovaHudZone[playerid], 2);
    PlayerTextDrawLetterSize(playerid, NovaHudZone[playerid], 0.20, 1.0);
    PlayerTextDrawColor(playerid, NovaHudZone[playerid], 0xFFFFFFFF);

    NovaHudNeeds[playerid] = CreatePlayerTextDraw(playerid, 520.0, 431.0, "Hambre");
    PlayerTextDrawFont(playerid, NovaHudNeeds[playerid], 2);
    PlayerTextDrawLetterSize(playerid, NovaHudNeeds[playerid], 0.20, 1.0);
    PlayerTextDrawColor(playerid, NovaHudNeeds[playerid], 0xFFFFFFFF);
    return 1;
}

forward RoleplayShowHudPublic(playerid);
public RoleplayShowHudPublic(playerid)
{
    return RoleplayShowHud(playerid);
}

stock RoleplayShowHud(playerid)
{
    new name[MAX_PLAYER_NAME];
    new zone[MAX_ZONE_NAME];
    new text[128];

    GetPlayerName(playerid, name, sizeof(name));
    NovaGetZone(playerid, zone, sizeof(zone));

    format(text, sizeof(text), "Nova Roleplay | %s (ID:%d)", name, playerid);
    PlayerTextDrawSetString(playerid, NovaHudInfo[playerid], text);

    format(text, sizeof(text), "Ubicacion: %s", zone);
    PlayerTextDrawSetString(playerid, NovaHudZone[playerid], text);

    format(text, sizeof(text), "Hambre: %d | Nv. %d | EXP %d",
        pHunger[playerid], PlayerData[playerid][pLevel], PlayerData[playerid][pExp]);
    PlayerTextDrawSetString(playerid, NovaHudNeeds[playerid], text);

    PlayerTextDrawShow(playerid, NovaHudBar[playerid]);
    PlayerTextDrawShow(playerid, NovaHudInfo[playerid]);
    PlayerTextDrawShow(playerid, NovaHudZone[playerid]);
    PlayerTextDrawShow(playerid, NovaHudNeeds[playerid]);
    return 1;
}

forward RoleplayShowProfileDialog(playerid);
public RoleplayShowProfileDialog(playerid)
{
    NovaProfilePreview[playerid] = CreatePlayerTextDraw(playerid, 500.0, 135.0, "_");
    PlayerTextDrawFont(playerid, NovaProfilePreview[playerid], 5);
    PlayerTextDrawTextSize(playerid, NovaProfilePreview[playerid], 90.0, 120.0);
    PlayerTextDrawSetPreviewModel(playerid, NovaProfilePreview[playerid], 60);
    PlayerTextDrawSetPreviewRot(playerid, NovaProfilePreview[playerid], 0.0, 0.0, 180.0, 1.0);
    PlayerTextDrawShow(playerid, NovaProfilePreview[playerid]);

    ShowPlayerDialog(playerid, DIALOG_PROFILE, DIALOG_STYLE_INPUT,
        "Nova Roleplay - Crear personaje",
        "Escribe: edad sexo ciudad\nEjemplo: 21 hombre Blueberry\nCiudades: LS, SF, LV o Blueberry",
        "Crear", "Salir");
    return 1;
}

stock RoleplayOnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid == DIALOG_PROFILE)
    {
        return NovaHandleProfileDialog(playerid, response, inputtext);
    }

    if (dialogid == DIALOG_HELP || dialogid == DIALOG_SHOP ||
        dialogid == DIALOG_FRIENDS || dialogid == DIALOG_ADMIN)
    {
        return 1;
    }

    return 0;
}

stock NovaHandleProfileDialog(playerid, response, inputtext[])
{
    if (!response) return Kick(playerid), 1;

    new age;
    new sex[16];
    new city[24];

    if (sscanf(inputtext, "ds[16]s[24]", age, sex, city) || age < 14 || age > 90)
    {
        ShowPlayerDialog(playerid, DIALOG_PROFILE, DIALOG_STYLE_INPUT,
            "Nova Roleplay - Error",
            "Formato invalido. Usa: 21 hombre Blueberry",
            "Crear", "Salir");
        return 1;
    }

    pAge[playerid] = age;
    pSex[playerid] = NovaParseSex(sex);
    pCity[playerid] = NovaParseCity(city);

    PlayerTextDrawDestroy(playerid, NovaProfilePreview[playerid]);
    NovaFinishRegistration(playerid);
    return 1;
}

stock NovaParseSex(sex[])
{
    if (!strcmp(sex, "mujer", true) || !strcmp(sex, "femenino", true))
        return 2;
    return 1;
}

stock NovaParseCity(city[])
{
    if (!strcmp(city, "SF", true) || strfind(city, "fierro", true) != -1)
        return 2;
    if (!strcmp(city, "LV", true) || strfind(city, "venturas", true) != -1)
        return 3;
    if (strfind(city, "blue", true) != -1)
        return 4;
    return 1;
}

stock NovaFinishRegistration(playerid)
{
    new Float:x, Float:y, Float:z;

    PlayerData[playerid][pLoggedIn] = true;
    PlayerData[playerid][pLevel] = 1;
    PlayerData[playerid][pExp] = 0;
    PlayerData[playerid][pMoney] = 500;
    pHunger[playerid] = 100;

    NovaGetSpawnByCity(pCity[playerid], x, y, z);
    SetPlayerSkin(playerid, pSex[playerid] == 2 ? 12 : 60);
    GivePlayerMoney(playerid, 500);
    TogglePlayerSpectating(playerid, false);
    SpawnPlayer(playerid);
    SetPlayerPos(playerid, x, y, z);
    RoleplayShowHud(playerid);

    SendClientMessage(playerid, COLOR_NOVA,
        "Bienvenido a Nova Roleplay. Usa /ayuda para ver comandos.");
    return 1;
}

stock NovaGetSpawnByCity(city, &Float:x, &Float:y, &Float:z)
{
    if (city == 2)
    {
        x = -1983.0; y = 138.0; z = 27.7;
    }
    else if (city == 3)
    {
        x = 2025.0; y = 1343.0; z = 10.8;
    }
    else if (city == 4)
    {
        x = 203.7; y = -107.0; z = 1.5;
    }
    else
    {
        x = 1481.0; y = -1771.0; z = 18.8;
    }
    return 1;
}

forward NovaMinuteTimer();
public NovaMinuteTimer()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i) || !PlayerData[i][pLoggedIn]) continue;

        if (pHunger[i] > 0) pHunger[i]--;
        PlayerData[i][pExp]++;

        if (PlayerData[i][pExp] >= PlayerData[i][pLevel] * 60)
        {
            PlayerData[i][pExp] = 0;
            PlayerData[i][pLevel]++;
            SendClientMessage(i, COLOR_NOVA, "Subiste de nivel por actividad.");
        }

        if (pHunger[i] <= 10)
            SendClientMessage(i, COLOR_WARN, "Tienes hambre. Compra comida en un 24/7.");

        RoleplayShowHud(i);
    }
    return 1;
}

CMD:ayuda(playerid, params[])
{
    ShowPlayerDialog(playerid, DIALOG_HELP, DIALOG_STYLE_MSGBOX,
        "Nova Roleplay - Ayuda",
        "/stats /comprar /comida /telefono /radio /numero\n/sms /llamar /ubi /amigos /addamigo\n/banco /depositar /retirar /pagar\nAdmin: /admin /kick /daradmin /ir /traer",
        "Cerrar", "");
    return 1;
}

CMD:stats(playerid, params[])
{
    new text[160];
    format(text, sizeof(text),
        "Nivel: %d | EXP: %d | Hambre: %d | Tel: %d | Admin: %d",
        PlayerData[playerid][pLevel], PlayerData[playerid][pExp],
        pHunger[playerid], pPhone[playerid], PlayerData[playerid][pAdminLevel]);
    SendClientMessage(playerid, COLOR_INFO, text);
    return 1;
}

CMD:comprar(playerid, params[])
{
    ShowPlayerDialog(playerid, DIALOG_SHOP, DIALOG_STYLE_MSGBOX,
        "Nova Roleplay - Tiendas",
        "24/7: /comida y /numero\nElectronica: /telefono y /radio\nConcesionario: /comprarveh",
        "Cerrar", "");
    return 1;
}

CMD:comida(playerid, params[])
{
    if (GetPlayerMoney(playerid) < 50)
        return SendClientMessage(playerid, COLOR_ERROR, "Necesitas $50."), 1;

    GivePlayerMoney(playerid, -50);
    pHunger[playerid] = 100;
    RoleplayShowHud(playerid);
    SendClientMessage(playerid, COLOR_NOVA, "Compraste comida.");
    return 1;
}

CMD:telefono(playerid, params[])
{
    if (pHasPhone[playerid])
        return SendClientMessage(playerid, COLOR_WARN, "Ya tienes telefono."), 1;
    if (GetPlayerMoney(playerid) < 500)
        return SendClientMessage(playerid, COLOR_ERROR, "Necesitas $500."), 1;

    GivePlayerMoney(playerid, -500);
    pHasPhone[playerid] = true;
    SendClientMessage(playerid, COLOR_NOVA, "Compraste un telefono.");
    return 1;
}

CMD:radio(playerid, params[])
{
    if (GetPlayerMoney(playerid) < 300)
        return SendClientMessage(playerid, COLOR_ERROR, "Necesitas $300."), 1;

    GivePlayerMoney(playerid, -300);
    pHasRadio[playerid] = true;
    SendClientMessage(playerid, COLOR_NOVA, "Compraste una radio.");
    return 1;
}

CMD:numero(playerid, params[])
{
    if (!pHasPhone[playerid])
        return SendClientMessage(playerid, COLOR_ERROR, "Primero compra un telefono."), 1;
    if (pPhone[playerid] != 0)
        return SendClientMessage(playerid, COLOR_WARN, "Ya tienes numero."), 1;
    if (GetPlayerMoney(playerid) < 250)
        return SendClientMessage(playerid, COLOR_ERROR, "Necesitas $250."), 1;

    GivePlayerMoney(playerid, -250);
    pPhone[playerid] = 100000 + random(899999);

    new text[80];
    format(text, sizeof(text), "Tu numero telefonico es %d.", pPhone[playerid]);
    SendClientMessage(playerid, COLOR_NOVA, text);
    return 1;
}

stock NovaFindPhone(number)
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsPlayerConnected(i) && pPhone[i] == number)
            return i;
    }
    return INVALID_PLAYER_ID;
}

CMD:sms(playerid, params[])
{
    new number;
    new message[96];
    if (sscanf(params, "ds[96]", number, message))
        return SendClientMessage(playerid, COLOR_INFO, "Uso: /sms numero mensaje"), 1;

    new target = NovaFindPhone(number);
    if (target == INVALID_PLAYER_ID)
        return SendClientMessage(playerid, COLOR_ERROR, "Numero no disponible."), 1;

    new name[MAX_PLAYER_NAME];
    new text[144];
    GetPlayerName(playerid, name, sizeof(name));
    format(text, sizeof(text), "SMS de %s (%d): %s", name, pPhone[playerid], message);
    SendClientMessage(target, COLOR_NOVA, text);
    SendClientMessage(playerid, COLOR_NOVA, "SMS enviado.");
    return 1;
}

CMD:llamar(playerid, params[])
{
    new number;
    if (sscanf(params, "d", number))
        return SendClientMessage(playerid, COLOR_INFO, "Uso: /llamar numero"), 1;

    new target = NovaFindPhone(number);
    if (target == INVALID_PLAYER_ID)
        return SendClientMessage(playerid, COLOR_ERROR, "Numero no disponible."), 1;

    SendClientMessage(target, COLOR_NOVA, "Te estan llamando. Responde con /sms.");
    SendClientMessage(playerid, COLOR_NOVA, "Llamada enviada.");
    return 1;
}

CMD:ubi(playerid, params[])
{
    new number;
    if (sscanf(params, "d", number))
        return SendClientMessage(playerid, COLOR_INFO, "Uso: /ubi numero"), 1;

    new target = NovaFindPhone(number);
    if (target == INVALID_PLAYER_ID)
        return SendClientMessage(playerid, COLOR_ERROR, "Numero no disponible."), 1;

    new Float:x, Float:y, Float:z;
    new zone[MAX_ZONE_NAME];
    new text[128];

    GetPlayerPos(playerid, x, y, z);
    NovaGetZone(playerid, zone, sizeof(zone));
    SetPlayerCheckpoint(target, x, y, z, 4.0);

    format(text, sizeof(text), "Ubicacion compartida: %s. Checkpoint marcado.", zone);
    SendClientMessage(target, COLOR_NOVA, text);
    SendClientMessage(playerid, COLOR_NOVA, "Compartiste tu ubicacion.");
    return 1;
}

CMD:comprarveh(playerid, params[])
{
    if (GetPlayerMoney(playerid) < 12000)
        return SendClientMessage(playerid, COLOR_ERROR, "Necesitas $12000."), 1;

    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    GivePlayerMoney(playerid, -12000);
    CreateVehicle(411, x + 3.0, y, z, a, random(126), random(126), -1);
    SendClientMessage(playerid, COLOR_NOVA, "Compraste un vehiculo basico.");
    return 1;
}

CMD:addamigo(playerid, params[])
{
    new target;
    if (sscanf(params, "u", target) || !IsPlayerConnected(target))
        return SendClientMessage(playerid, COLOR_INFO, "Uso: /addamigo id"), 1;

    for (new i = 0; i < MAX_FRIEND_SLOTS; i++)
    {
        if (NovaFriends[playerid][i] == INVALID_PLAYER_ID)
        {
            NovaFriends[playerid][i] = target;
            SendClientMessage(playerid, COLOR_NOVA, "Amigo agregado. Usa /amigos.");
            return 1;
        }
    }

    SendClientMessage(playerid, COLOR_ERROR, "Lista de amigos llena.");
    return 1;
}

CMD:amigos(playerid, params[])
{
    new list[768];
    new line[96];
    new name[MAX_PLAYER_NAME];
    new count;

    strcat(list, "Nombre / ID / Estado / Acciones\n");

    for (new i = 0; i < MAX_FRIEND_SLOTS; i++)
    {
        new target = NovaFriends[playerid][i];
        if (target == INVALID_PLAYER_ID) continue;

        if (IsPlayerConnected(target))
        {
            GetPlayerName(target, name, sizeof(name));
            format(line, sizeof(line), "%s / ID:%d / Conectado / /sms /llamar /ubi %d\n",
                name, target, pPhone[target]);
        }
        else
        {
            format(line, sizeof(line), "Slot %d / Desconectado\n", i);
        }

        strcat(list, line);
        count++;
    }

    if (!count) strcat(list, "No tienes amigos. Usa /addamigo id.\n");

    ShowPlayerDialog(playerid, DIALOG_FRIENDS, DIALOG_STYLE_MSGBOX,
        "Nova Roleplay - Amigos", list, "Cerrar", "");
    return 1;
}

CMD:admin(playerid, params[])
{
    if (PlayerData[playerid][pAdminLevel] < 1)
        return SendClientMessage(playerid, COLOR_ERROR, "No eres administrador."), 1;

    ShowPlayerDialog(playerid, DIALOG_ADMIN, DIALOG_STYLE_MSGBOX,
        "Nova Roleplay - Admin",
        "/kick id razon\n/daradmin id nivel\n/ir id\n/traer id",
        "Cerrar", "");
    return 1;
}

CMD:kick(playerid, params[])
{
    if (PlayerData[playerid][pAdminLevel] < 1)
        return SendClientMessage(playerid, COLOR_ERROR, "No eres administrador."), 1;

    new target;
    new reason[64];
    if (sscanf(params, "us[64]", target, reason))
        return SendClientMessage(playerid, COLOR_INFO, "Uso: /kick id razon"), 1;

    SendClientMessage(target, COLOR_ERROR, reason);
    Kick(target);
    return 1;
}

CMD:daradmin(playerid, params[])
{
    if (PlayerData[playerid][pAdminLevel] < 5)
        return SendClientMessage(playerid, COLOR_ERROR, "Necesitas admin nivel 5."), 1;

    new target, level;
    if (sscanf(params, "ud", target, level))
        return SendClientMessage(playerid, COLOR_INFO, "Uso: /daradmin id nivel"), 1;

    PlayerData[target][pAdminLevel] = level;
    SendClientMessage(playerid, COLOR_NOVA, "Nivel admin actualizado.");
    return 1;
}

CMD:ir(playerid, params[])
{
    if (PlayerData[playerid][pAdminLevel] < 1)
        return SendClientMessage(playerid, COLOR_ERROR, "No eres administrador."), 1;

    new target;
    new Float:x, Float:y, Float:z;
    if (sscanf(params, "u", target))
        return SendClientMessage(playerid, COLOR_INFO, "Uso: /ir id"), 1;

    GetPlayerPos(target, x, y, z);
    SetPlayerPos(playerid, x + 1.0, y, z);
    return 1;
}

CMD:traer(playerid, params[])
{
    if (PlayerData[playerid][pAdminLevel] < 1)
        return SendClientMessage(playerid, COLOR_ERROR, "No eres administrador."), 1;

    new target;
    new Float:x, Float:y, Float:z;
    if (sscanf(params, "u", target))
        return SendClientMessage(playerid, COLOR_INFO, "Uso: /traer id"), 1;

    GetPlayerPos(playerid, x, y, z);
    SetPlayerPos(target, x + 1.0, y, z);
    return 1;
}
