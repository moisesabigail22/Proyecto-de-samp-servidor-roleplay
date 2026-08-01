/* ============================================================
    Nova Roleplay - Sistemas de interfaz, perfil, tiendas,
    telefono, amigos, administracion, hambre y niveles.
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

#define DIALOG_PROFILE       200
#define DIALOG_HELP          201
#define DIALOG_FRIENDS       202
#define DIALOG_SHOP          203
#define DIALOG_ADMIN         204
#define DIALOG_FRIEND_ACTION 205
#define MAX_ZONE_NAME 32

#define SHOP_247             1
#define SHOP_ELECTRONICS     2
#define SHOP_DEALER          3

#define COLOR_NOVA           0x8A2BE2FF
#define COLOR_INFO           0xF5F5F5FF
#define COLOR_WARN           0xFFAA00FF
#define COLOR_ERROR          0xFF4444FF

new PlayerText:NovaHudBar[MAX_PLAYERS];
new PlayerText:NovaHudInfo[MAX_PLAYERS];
new PlayerText:NovaHudLocation[MAX_PLAYERS];
new PlayerText:NovaHudNeeds[MAX_PLAYERS];
new PlayerText:NovaRegisterSkin[MAX_PLAYERS];

new pSelectedFriend[MAX_PLAYERS];
new bool:pFriends[MAX_PLAYERS][MAX_PLAYERS];
new pRoleplayTimer;

stock GetPlayer2DZone(playerid, zone[], len)
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    if(x > 76.0 && x < 340.0 && y > -220.0 && y < 50.0) format(zone, len, "Blueberry");
    else if(x > 44.0 && x < 2997.0 && y > -2892.0 && y < -768.0) format(zone, len, "Los Santos");
    else if(x > -2997.0 && x < -1213.0 && y > -1115.0 && y < 1659.0) format(zone, len, "San Fierro");
    else if(x > 869.0 && x < 2997.0 && y > 596.0 && y < 2997.0) format(zone, len, "Las Venturas");
    else if(x > -1213.0 && x < 869.0 && y > -768.0 && y < 596.0) format(zone, len, "Red County");
    else format(zone, len, "San Andreas");
    return 1;
}

stock RoleplayOnGameModeInit()
{
    SetGameModeText("Nova Roleplay");
    SendRconCommand("hostname Nova Roleplay | Servidor RolePlay");

    CreatePickup(1274, 1, 1315.45, -898.13, 39.58, -1); // 24/7 LS
    CreateDynamic3DTextLabel("{FFFFFF}24/7 Los Santos\n{8A2BE2}/comprar", 0xFFFFFFFF, 1315.45, -898.13, 39.58, 20.0);
    CreatePickup(1274, 1, -2442.61, 755.43, 35.17, -1); // 24/7 SF
    CreateDynamic3DTextLabel("{FFFFFF}24/7 San Fierro\n{8A2BE2}/comprar", 0xFFFFFFFF, -2442.61, 755.43, 35.17, 20.0);
    CreatePickup(1274, 1, 2194.38, 1991.05, 12.30, -1); // 24/7 LV
    CreateDynamic3DTextLabel("{FFFFFF}24/7 Las Venturas\n{8A2BE2}/comprar", 0xFFFFFFFF, 2194.38, 1991.05, 12.30, 20.0);

    CreatePickup(1239, 1, 1128.92, -1458.11, 15.79, -1);
    CreateDynamic3DTextLabel("{FFFFFF}Electronica LS\nTelefono, radio y accesorios", 0xFFFFFFFF, 1128.92, -1458.11, 15.79, 20.0);
    CreatePickup(1239, 1, -1987.20, 285.76, 35.17, -1);
    CreateDynamic3DTextLabel("{FFFFFF}Electronica SF\nTelefono, radio y accesorios", 0xFFFFFFFF, -1987.20, 285.76, 35.17, 20.0);
    CreatePickup(1239, 1, 2020.40, 1007.84, 10.82, -1);
    CreateDynamic3DTextLabel("{FFFFFF}Electronica LV\nTelefono, radio y accesorios", 0xFFFFFFFF, 2020.40, 1007.84, 10.82, 20.0);

    CreatePickup(1239, 1, 542.52, -1293.64, 17.24, -1);
    CreateDynamic3DTextLabel("{FFFFFF}Concesionario Los Santos\n{8A2BE2}/comprarveh", 0xFFFFFFFF, 542.52, -1293.64, 17.24, 25.0);
    CreatePickup(1239, 1, -1954.25, 300.25, 35.46, -1);
    CreateDynamic3DTextLabel("{FFFFFF}Concesionario San Fierro\n{8A2BE2}/comprarveh", 0xFFFFFFFF, -1954.25, 300.25, 35.46, 25.0);
    CreatePickup(1239, 1, 2128.52, 1408.30, 10.82, -1);
    CreateDynamic3DTextLabel("{FFFFFF}Concesionario Las Venturas\n{8A2BE2}/comprarveh", 0xFFFFFFFF, 2128.52, 1408.30, 10.82, 25.0);

    pRoleplayTimer = SetTimer("RoleplayMinute", 60000, true);
    return 1;
}

stock RoleplayOnGameModeExit() { KillTimer(pRoleplayTimer); return 1; }

stock RoleplayCreateHud(playerid)
{
    NovaHudBar[playerid] = CreatePlayerTextDraw(playerid, 0.0, 426.0, "_");
    PlayerTextDrawUseBox(playerid, NovaHudBar[playerid], 1);
    PlayerTextDrawBoxColor(playerid, NovaHudBar[playerid], 0x000000DD);
    PlayerTextDrawTextSize(playerid, NovaHudBar[playerid], 640.0, 0.0);
    PlayerTextDrawLetterSize(playerid, NovaHudBar[playerid], 0.5, 2.0);

    NovaHudInfo[playerid] = CreatePlayerTextDraw(playerid, 8.0, 431.0, "Nova Roleplay | Nombre (ID:0)");
    PlayerTextDrawFont(playerid, NovaHudInfo[playerid], 2);
    PlayerTextDrawLetterSize(playerid, NovaHudInfo[playerid], 0.20, 1.0);
    PlayerTextDrawColor(playerid, NovaHudInfo[playerid], 0xFFFFFFFF);

    NovaHudLocation[playerid] = CreatePlayerTextDraw(playerid, 270.0, 431.0, "Ubicacion: San Andreas");
    PlayerTextDrawFont(playerid, NovaHudLocation[playerid], 2);
    PlayerTextDrawLetterSize(playerid, NovaHudLocation[playerid], 0.20, 1.0);
    PlayerTextDrawColor(playerid, NovaHudLocation[playerid], 0xFFFFFFFF);

    NovaHudNeeds[playerid] = CreatePlayerTextDraw(playerid, 520.0, 431.0, "Hambre: 100 | Nv. 1");
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
    new name[MAX_PLAYER_NAME], zone[MAX_ZONE_NAME], msg[128];
    GetPlayerName(playerid, name, sizeof(name));
    GetPlayer2DZone(playerid, zone, sizeof(zone));
    format(msg, sizeof(msg), "Nova Roleplay | %s (ID:%d)", name, playerid); PlayerTextDrawSetString(playerid, NovaHudInfo[playerid], msg);
    format(msg, sizeof(msg), "Ubicacion: %s", zone); PlayerTextDrawSetString(playerid, NovaHudLocation[playerid], msg);
    format(msg, sizeof(msg), "Hambre: %d | Nv. %d | EXP %d", pHunger[playerid], PlayerData[playerid][pLevel], PlayerData[playerid][pExp]); PlayerTextDrawSetString(playerid, NovaHudNeeds[playerid], msg);
    PlayerTextDrawShow(playerid, NovaHudBar[playerid]); PlayerTextDrawShow(playerid, NovaHudInfo[playerid]); PlayerTextDrawShow(playerid, NovaHudLocation[playerid]); PlayerTextDrawShow(playerid, NovaHudNeeds[playerid]);
    return 1;
}

stock RoleplayOnPlayerConnect(playerid)
{
    pAge[playerid] = 18; pSex[playerid] = 1; pCity[playerid] = 1; pPhone[playerid] = 0; pHunger[playerid] = 100; pHasPhone[playerid] = false; pHasRadio[playerid] = false;
    for(new i = 0; i < MAX_PLAYERS; i++) pFriends[playerid][i] = false;
    RoleplayCreateHud(playerid);
    return 1;
}

stock RoleplayOnPlayerDisconnect(playerid)
{
    PlayerTextDrawDestroy(playerid, NovaHudBar[playerid]); PlayerTextDrawDestroy(playerid, NovaHudInfo[playerid]); PlayerTextDrawDestroy(playerid, NovaHudLocation[playerid]); PlayerTextDrawDestroy(playerid, NovaHudNeeds[playerid]);
    return 1;
}

forward RoleplayShowProfileDialog(playerid);
public RoleplayShowProfileDialog(playerid)
{
    NovaRegisterSkin[playerid] = CreatePlayerTextDraw(playerid, 500.0, 135.0, "_");
    PlayerTextDrawFont(playerid, NovaRegisterSkin[playerid], 5);
    PlayerTextDrawTextSize(playerid, NovaRegisterSkin[playerid], 90.0, 120.0);
    PlayerTextDrawSetPreviewModel(playerid, NovaRegisterSkin[playerid], 60);
    PlayerTextDrawSetPreviewRot(playerid, NovaRegisterSkin[playerid], 0.0, 0.0, 180.0, 1.0);
    PlayerTextDrawShow(playerid, NovaRegisterSkin[playerid]);
    ShowPlayerDialog(playerid, DIALOG_PROFILE, DIALOG_STYLE_INPUT, "Nova Roleplay - Datos de personaje", "Completa: edad sexo ciudad\nEjemplo: 21 hombre Blueberry\nCiudades: Los Santos, San Fierro, Las Venturas, Blueberry", "Crear", "Salir");
    return 1;
}

stock RoleplayOnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_PROFILE)
    {
        if(!response) return Kick(playerid), 1;
        new edad, sexo[16], ciudad[32];
        if(sscanf(inputtext, "ds[16]s[32]", edad, sexo, ciudad) || edad < 14 || edad > 90)
            return ShowPlayerDialog(playerid, DIALOG_PROFILE, DIALOG_STYLE_INPUT, "Nova Roleplay - Error", "Formato invalido. Usa: 21 hombre Blueberry", "Crear", "Salir"), 1;
        pAge[playerid] = edad; pSex[playerid] = (!strcmp(sexo, "mujer", true) || !strcmp(sexo, "femenino", true)) ? 2 : 1;
        if(strfind(ciudad, "fierro", true) != -1) pCity[playerid] = 2; else if(strfind(ciudad, "venturas", true) != -1) pCity[playerid] = 3; else if(strfind(ciudad, "blue", true) != -1) pCity[playerid] = 4; else pCity[playerid] = 1;
        PlayerTextDrawDestroy(playerid, NovaRegisterSkin[playerid]);
        RoleplayFinishRegistration(playerid);
        return 1;
    }
    if(dialogid == DIALOG_HELP || dialogid == DIALOG_FRIENDS || dialogid == DIALOG_ADMIN || dialogid == DIALOG_SHOP) return 1;
    return 0;
}

stock RoleplayFinishRegistration(playerid)
{
    PlayerData[playerid][pLoggedIn] = true; PlayerData[playerid][pLevel] = 1; PlayerData[playerid][pExp] = 0; PlayerData[playerid][pMoney] = 500; pHunger[playerid] = 100;
    new Float:x, Float:y, Float:z;
    if(pCity[playerid] == 2) { x = -1983.0; y = 138.0; z = 27.7; }
    else if(pCity[playerid] == 3) { x = 2025.0; y = 1343.0; z = 10.8; }
    else if(pCity[playerid] == 4) { x = 203.7; y = -107.0; z = 1.5; }
    else { x = 1481.0; y = -1771.0; z = 18.8; }
    SetPlayerPos(playerid, x, y, z); SetPlayerSkin(playerid, pSex[playerid] == 2 ? 12 : 60); GivePlayerMoney(playerid, 500);
    TogglePlayerSpectating(playerid, false); SpawnPlayer(playerid); SetPlayerPos(playerid, x, y, z); RoleplayShowHud(playerid);
    SendClientMessage(playerid, COLOR_NOVA, "Bienvenido a Nova Roleplay. Usa /ayuda para ver los comandos.");
    return 1;
}

forward RoleplayMinute();
public RoleplayMinute()
{
    for(new i = 0; i < MAX_PLAYERS; i++) if(IsPlayerConnected(i) && PlayerData[i][pLoggedIn])
    {
        if(pHunger[i] > 0) pHunger[i]--;
        PlayerData[i][pExp]++;
        if(PlayerData[i][pExp] >= PlayerData[i][pLevel] * 60) { PlayerData[i][pExp] = 0; PlayerData[i][pLevel]++; SendClientMessage(i, COLOR_NOVA, "Subiste de nivel por tu actividad en el servidor."); }
        if(pHunger[i] <= 10) SendClientMessage(i, COLOR_WARN, "Tienes mucha hambre. Compra comida en un 24/7.");
        RoleplayShowHud(i);
    }
    return 1;
}

CMD:ayuda(playerid, params[])
{
    ShowPlayerDialog(playerid, DIALOG_HELP, DIALOG_STYLE_MSGBOX, "Nova Roleplay - Ayuda", "/stats /ayuda /comprar /comprarveh\n/telefono /sms /llamar /ubi /amigos\n/banco /depositar /retirar /pagar\nAdmin: /admin /daradmin /kick /ir /traer", "Cerrar", "");
    return 1;
}

CMD:stats(playerid, params[])
{
    new msg[180]; format(msg, sizeof(msg), "Nivel: %d | EXP: %d | Hambre: %d | Dinero: $%d | Telefono: %d | Admin: %d", PlayerData[playerid][pLevel], PlayerData[playerid][pExp], pHunger[playerid], GetPlayerMoney(playerid), pPhone[playerid], PlayerData[playerid][pAdminLevel]);
    SendClientMessage(playerid, COLOR_INFO, msg); return 1;
}

CMD:comprar(playerid, params[])
{
    ShowPlayerDialog(playerid, DIALOG_SHOP, DIALOG_STYLE_MSGBOX, "Nova Roleplay - Tiendas", "24/7: comida ($50), numero telefonico aleatorio ($250).\nElectronica: telefono ($500) y radio ($300).\nUsa /comida, /numero, /telefono o /radio cerca del negocio.", "Cerrar", "");
    return 1;
}
CMD:comida(playerid, params[]) { if(GetPlayerMoney(playerid) < 50) return SendClientMessage(playerid, COLOR_ERROR, "No tienes $50."), 1; GivePlayerMoney(playerid, -50); pHunger[playerid] = 100; RoleplayShowHud(playerid); return SendClientMessage(playerid, COLOR_NOVA, "Compraste comida y llenaste tu hambre."), 1; }
CMD:telefono(playerid, params[]) { if(pHasPhone[playerid]) return SendClientMessage(playerid, COLOR_WARN, "Ya tienes telefono."), 1; if(GetPlayerMoney(playerid) < 500) return SendClientMessage(playerid, COLOR_ERROR, "Necesitas $500."), 1; GivePlayerMoney(playerid, -500); pHasPhone[playerid] = true; return SendClientMessage(playerid, COLOR_NOVA, "Compraste un telefono."), 1; }
CMD:radio(playerid, params[]) { if(GetPlayerMoney(playerid) < 300) return SendClientMessage(playerid, COLOR_ERROR, "Necesitas $300."), 1; GivePlayerMoney(playerid, -300); pHasRadio[playerid] = true; return SendClientMessage(playerid, COLOR_NOVA, "Compraste una radio."), 1; }
CMD:numero(playerid, params[]) { if(!pHasPhone[playerid]) return SendClientMessage(playerid, COLOR_ERROR, "Primero compra un telefono en electronica."), 1; if(pPhone[playerid]) return SendClientMessage(playerid, COLOR_WARN, "Ya tienes numero."), 1; if(GetPlayerMoney(playerid) < 250) return SendClientMessage(playerid, COLOR_ERROR, "Necesitas $250."), 1; GivePlayerMoney(playerid, -250); pPhone[playerid] = 100000 + random(899999); new m[80]; format(m, sizeof(m), "Tu numero telefonico aleatorio es %d.", pPhone[playerid]); return SendClientMessage(playerid, COLOR_NOVA, m), 1; }
CMD:comprarveh(playerid, params[]) { if(GetPlayerMoney(playerid) < 12000) return SendClientMessage(playerid, COLOR_ERROR, "Necesitas $12000 para un vehiculo basico."), 1; new Float:x, Float:y, Float:z, Float:a; GetPlayerPos(playerid, x, y, z); GetPlayerFacingAngle(playerid, a); GivePlayerMoney(playerid, -12000); CreateVehicle(411, x+3.0, y, z, a, random(126), random(126), -1); return SendClientMessage(playerid, COLOR_NOVA, "Compraste un vehiculo basico del concesionario."), 1; }

stock FindPlayerByPhone(number) { for(new i=0;i<MAX_PLAYERS;i++) if(IsPlayerConnected(i) && pPhone[i] == number) return i; return INVALID_PLAYER_ID; }
CMD:sms(playerid, params[]) { new number, text[96]; if(sscanf(params, "ds[96]", number, text)) return SendClientMessage(playerid, COLOR_INFO, "Uso: /sms numero mensaje"), 1; new target = FindPlayerByPhone(number); if(target == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_ERROR, "Numero no disponible."), 1; new name[MAX_PLAYER_NAME], msg[144]; GetPlayerName(playerid, name, sizeof(name)); format(msg, sizeof(msg), "SMS de %s (%d): %s", name, pPhone[playerid], text); SendClientMessage(target, COLOR_NOVA, msg); return SendClientMessage(playerid, COLOR_NOVA, "SMS enviado."), 1; }
CMD:llamar(playerid, params[]) { new number; if(sscanf(params, "d", number)) return SendClientMessage(playerid, COLOR_INFO, "Uso: /llamar numero"), 1; new target = FindPlayerByPhone(number); if(target == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_ERROR, "Numero no disponible."), 1; SendClientMessage(target, COLOR_NOVA, "Te estan llamando. Usa /sms para responder por ahora."); return SendClientMessage(playerid, COLOR_NOVA, "Llamada enviada."), 1; }
CMD:ubi(playerid, params[]) { new number; if(sscanf(params, "d", number)) return SendClientMessage(playerid, COLOR_INFO, "Uso: /ubi numero"), 1; new target = FindPlayerByPhone(number); if(target == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_ERROR, "Numero no disponible."), 1; new Float:x, Float:y, Float:z, zone[MAX_ZONE_NAME], msg[128]; GetPlayerPos(playerid, x, y, z); GetPlayer2DZone(playerid, zone, sizeof(zone)); SetPlayerCheckpoint(target, x, y, z, 4.0); format(msg, sizeof(msg), "Ubicacion compartida: %s. Se marco un checkpoint.", zone); SendClientMessage(target, COLOR_NOVA, msg); return SendClientMessage(playerid, COLOR_NOVA, "Compartiste tu ubicacion actual."), 1; }
CMD:amigos(playerid, params[])
{
    new list[768] = "Nombre / ID / Estado / Acciones\n";
    new name[MAX_PLAYER_NAME], line[96], count;
    for(new i = 0; i < MAX_PLAYERS; i++) if(pFriends[playerid][i])
    {
        if(IsPlayerConnected(i)) { GetPlayerName(i, name, sizeof(name)); format(line, sizeof(line), "%s / ID:%d / Conectado / /sms /llamar /ubi %d\n", name, i, pPhone[i]); }
        else format(line, sizeof(line), "Amigo slot %d / Desconectado\n", i);
        strcat(list, line); count++;
    }
    if(!count) strcat(list, "No tienes amigos agregados. Usa /addamigo id.\n");
    ShowPlayerDialog(playerid, DIALOG_FRIENDS, DIALOG_STYLE_MSGBOX, "Nova Roleplay - Amigos", list, "Cerrar", "");
    return 1;
}
CMD:addamigo(playerid, params[])
{
    new id; if(sscanf(params, "u", id) || !IsPlayerConnected(id)) return SendClientMessage(playerid, COLOR_INFO, "Uso: /addamigo id"), 1;
    pFriends[playerid][id] = true; pFriends[id][playerid] = true;
    SendClientMessage(playerid, COLOR_NOVA, "Amigo agregado. Abre /amigos para ver estado y acciones.");
    SendClientMessage(id, COLOR_NOVA, "Te agregaron como amigo. Abre /amigos para ver estado y acciones.");
    return 1;
}

CMD:admin(playerid, params[]) { if(PlayerData[playerid][pAdminLevel] < 1) return SendClientMessage(playerid, COLOR_ERROR, "No eres administrador."), 1; ShowPlayerDialog(playerid, DIALOG_ADMIN, DIALOG_STYLE_MSGBOX, "Nova Roleplay - Admin", "/kick id razon\n/daradmin id nivel\n/ir id\n/traer id\n/admin para este panel", "Cerrar", ""); return 1; }
CMD:kick(playerid, params[]) { if(PlayerData[playerid][pAdminLevel] < 1) return SendClientMessage(playerid, COLOR_ERROR, "No eres administrador."), 1; new id, reason[64]; if(sscanf(params, "us[64]", id, reason)) return SendClientMessage(playerid, COLOR_INFO, "Uso: /kick id razon"), 1; SendClientMessage(id, COLOR_ERROR, reason); Kick(id); return 1; }
CMD:daradmin(playerid, params[]) { if(PlayerData[playerid][pAdminLevel] < 5) return SendClientMessage(playerid, COLOR_ERROR, "Necesitas admin nivel 5."), 1; new id, level; if(sscanf(params, "ud", id, level)) return SendClientMessage(playerid, COLOR_INFO, "Uso: /daradmin id nivel"), 1; PlayerData[id][pAdminLevel] = level; return SendClientMessage(playerid, COLOR_NOVA, "Nivel admin actualizado."), 1; }
CMD:ir(playerid, params[]) { if(PlayerData[playerid][pAdminLevel] < 1) return SendClientMessage(playerid, COLOR_ERROR, "No eres administrador."), 1; new id, Float:x, Float:y, Float:z; if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_INFO, "Uso: /ir id"), 1; GetPlayerPos(id, x, y, z); SetPlayerPos(playerid, x+1.0, y, z); return 1; }
CMD:traer(playerid, params[]) { if(PlayerData[playerid][pAdminLevel] < 1) return SendClientMessage(playerid, COLOR_ERROR, "No eres administrador."), 1; new id, Float:x, Float:y, Float:z; if(sscanf(params, "u", id)) return SendClientMessage(playerid, COLOR_INFO, "Uso: /traer id"), 1; GetPlayerPos(playerid, x, y, z); SetPlayerPos(id, x+1.0, y, z); return 1; }
