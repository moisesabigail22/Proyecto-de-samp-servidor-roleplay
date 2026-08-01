/* ============================================================
    accounts.inc
    Registro, login, carga y guardado de cuentas de jugador
    Requiere: database.inc, dialogs.inc, sscanf2
   ============================================================ */
#if defined _accounts_inc_included
    #endinput
#endif
#define _accounts_inc_included

#include <a_samp>
#include <sscanf2>
#include "../includes/core/database.inc"

#define DIALOG_LOGIN     100
#define DIALOG_REGISTER  101

#define MAX_LOGIN_ATTEMPTS 3

enum E_PLAYER_DATA
{
    pID,
    pAdminLevel,
    bool:pLoggedIn,
    pMoney,
    pBank,
    pLevel,
    pExp,
    pJob,
    pFaction,
    pGang,
    pLoginAttempts
};
new PlayerData[MAX_PLAYERS][E_PLAYER_DATA];
new pAge[MAX_PLAYERS], pSex[MAX_PLAYERS], pCity[MAX_PLAYERS], pPhone[MAX_PLAYERS], pHunger[MAX_PLAYERS];
new bool:pHasPhone[MAX_PLAYERS], bool:pHasRadio[MAX_PLAYERS];

// -------------------------------------------------------------
// Conexión de un jugador: revisamos si la cuenta existe
// -------------------------------------------------------------
stock AccountsOnPlayerConnect(playerid)
{
    PlayerData[playerid][pLoggedIn] = false;
    PlayerData[playerid][pLoginAttempts] = 0;

    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));

    new query[128];
    mysql_format(g_SQL, query, sizeof(query),
        "SELECT `id` FROM `accounts` WHERE `username` = '%e' LIMIT 1", name);
    mysql_tquery(g_SQL, query, "OnAccountCheck", "d", playerid);
    return 1;
}

forward OnAccountCheck(playerid);
public OnAccountCheck(playerid)
{
    if (!IsPlayerConnected(playerid)) return 1;

    new rows = cache_num_rows();
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));

    if (rows > 0)
    {
        // Cuenta existe -> pedir contraseña (login)
        new title[64];
        format(title, sizeof(title), "Iniciar sesion - Bienvenido %s", name);
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, title,
            "Esta cuenta ya esta registrada.\nIngresa tu contraseña para continuar:",
            "Ingresar", "Salir");
    }
    else
    {
        // No existe -> registrar
        new title[64];
        format(title, sizeof(title), "Registro - Crea tu cuenta %s", name);
        ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, title,
            "Esta cuenta no esta registrada.\nElige una contraseña para crearla:",
            "Registrarse", "Salir");
    }
    return 1;
}

// -------------------------------------------------------------
// Manejo de los diálogos de login/registro
// Llamar desde el OnDialogResponse principal del gamemode
// -------------------------------------------------------------
stock AccountsOnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid == DIALOG_REGISTER)
    {
        if (!response) return Kick(playerid), 1;

        if (strlen(inputtext) < 5)
        {
            ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD,
                "Registro - Error",
                "La contraseña debe tener al menos 5 caracteres.\nIntenta de nuevo:",
                "Registrarse", "Salir");
            return 1;
        }

        new hashPass[129];
        WP_Hash(hashPass, sizeof(hashPass), inputtext);

        new name[MAX_PLAYER_NAME];
        GetPlayerName(playerid, name, sizeof(name));

        new query[256];
        mysql_format(g_SQL, query, sizeof(query),
            "INSERT INTO `accounts` (`username`, `password`) VALUES ('%e', '%e')",
            name, hashPass);
        mysql_tquery(g_SQL, query, "OnAccountRegistered", "d", playerid);
        return 1;
    }

    if (dialogid == DIALOG_LOGIN)
    {
        if (!response) return Kick(playerid), 1;

        PlayerData[playerid][pLoginAttempts]++;

        new hashPass[129];
        WP_Hash(hashPass, sizeof(hashPass), inputtext);

        new name[MAX_PLAYER_NAME];
        GetPlayerName(playerid, name, sizeof(name));

        new query[256];
        mysql_format(g_SQL, query, sizeof(query),
            "SELECT * FROM `accounts` WHERE `username` = '%e' AND `password` = '%e' LIMIT 1",
            name, hashPass);
        mysql_tquery(g_SQL, query, "OnAccountLogin", "d", playerid);
        return 1;
    }
    return 0;
}

forward OnAccountRegistered(playerid);
public OnAccountRegistered(playerid)
{
    if (!IsPlayerConnected(playerid)) return 1;
    PlayerData[playerid][pID] = cache_insert_id();
    SendClientMessage(playerid, 0x33CC33FF, "Tu cuenta fue creada con exito. Completa tu personaje.");
    CallLocalFunction("RoleplayShowProfileDialog", "d", playerid);
    return 1;
}

forward OnAccountLogin(playerid);
public OnAccountLogin(playerid)
{
    if (!IsPlayerConnected(playerid)) return 1;

    if (cache_num_rows() > 0)
    {
        PlayerData[playerid][pLoggedIn] = true;

        new id, adminlvl, money, bank, level, exp, job, faction, gang;
        new age, sex, city, phone, hunger, has_phone, has_radio;
        cache_get_value_name_int(0, "id", id);
        cache_get_value_name_int(0, "admin_level", adminlvl);
        cache_get_value_name_int(0, "money", money);
        cache_get_value_name_int(0, "bank", bank);
        cache_get_value_name_int(0, "level", level);
        cache_get_value_name_int(0, "exp", exp);
        cache_get_value_name_int(0, "job", job);
        cache_get_value_name_int(0, "faction", faction);
        cache_get_value_name_int(0, "gang", gang);
        cache_get_value_name_int(0, "age", age);
        cache_get_value_name_int(0, "sex", sex);
        cache_get_value_name_int(0, "city", city);
        cache_get_value_name_int(0, "phone", phone);
        cache_get_value_name_int(0, "hunger", hunger);
        cache_get_value_name_int(0, "has_phone", has_phone);
        cache_get_value_name_int(0, "has_radio", has_radio);

        PlayerData[playerid][pID] = id;
        PlayerData[playerid][pAdminLevel] = adminlvl;
        PlayerData[playerid][pMoney] = money;
        PlayerData[playerid][pBank] = bank;
        PlayerData[playerid][pLevel] = level;
        PlayerData[playerid][pExp] = exp;
        PlayerData[playerid][pJob] = job;
        PlayerData[playerid][pFaction] = faction;
        PlayerData[playerid][pGang] = gang;
        pAge[playerid] = age;
        pSex[playerid] = sex;
        pCity[playerid] = city;
        pPhone[playerid] = phone;
        pHunger[playerid] = hunger;
        pHasPhone[playerid] = bool:has_phone;
        pHasRadio[playerid] = bool:has_radio;

        new posx, posy, posz, Float:fx, Float:fy, Float:fz, Float:facing, interior, world, skin;
        cache_get_value_name_float(0, "pos_x", fx);
        cache_get_value_name_float(0, "pos_y", fy);
        cache_get_value_name_float(0, "pos_z", fz);
        cache_get_value_name_int(0, "interior", interior);
        cache_get_value_name_int(0, "world", world);
        cache_get_value_name_int(0, "skin", skin);
        #pragma unused posx, posy, posz, facing

        SetPlayerPos(playerid, fx, fy, fz);
        SetPlayerInterior(playerid, interior);
        SetPlayerVirtualWorld(playerid, world);
        SetPlayerSkin(playerid, skin);
        GivePlayerMoney(playerid, PlayerData[playerid][pMoney]);

        SendClientMessage(playerid, 0x33CC33FF, "Has iniciado sesion correctamente. ¡Bienvenido de nuevo!");
        SetCameraBehindPlayer(playerid);
        TogglePlayerSpectating(playerid, false);
        CallLocalFunction("RoleplayShowHudPublic", "d", playerid);
    }
    else
    {
        if (PlayerData[playerid][pLoginAttempts] >= MAX_LOGIN_ATTEMPTS)
        {
            SendClientMessage(playerid, 0xFF0000FF, "Superaste el numero maximo de intentos. Has sido expulsado.");
            Kick(playerid);
            return 1;
        }

        new title[64], msg[128];
        format(title, sizeof(title), "Iniciar sesion - Intento %d/%d", PlayerData[playerid][pLoginAttempts], MAX_LOGIN_ATTEMPTS);
        format(msg, sizeof(msg), "Contraseña incorrecta.\nIntenta de nuevo:");
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, title, msg, "Ingresar", "Salir");
    }
    return 1;
}

// -------------------------------------------------------------
// Guardar / cargar stats adicionales (se llama tras crear cuenta)
// -------------------------------------------------------------
stock LoadPlayerStats(playerid)
{
    SetPlayerPos(playerid, 1958.3783, 1343.1572, 15.3746);
    SetPlayerSkin(playerid, 0);
    GivePlayerMoney(playerid, 500);
    return 1;
}

// Se llama en OnPlayerDisconnect
stock AccountsOnPlayerDisconnect(playerid)
{
    if (!PlayerData[playerid][pLoggedIn]) return 1;

    new Float:x, Float:y, Float:z, interior, world;
    GetPlayerPos(playerid, x, y, z);
    interior = GetPlayerInterior(playerid);
    world = GetPlayerVirtualWorld(playerid);

    new query[400];
    mysql_format(g_SQL, query, sizeof(query),
        "UPDATE `accounts` SET `money`=%d, `bank`=%d, `level`=%d, `exp`=%d, \
         `job`=%d, `faction`=%d, `gang`=%d, `pos_x`=%f, `pos_y`=%f, `pos_z`=%f, \
         `interior`=%d, `world`=%d, `age`=%d, `sex`=%d, `city`=%d, `phone`=%d, `hunger`=%d, `has_phone`=%d, `has_radio`=%d, `last_login`=NOW() WHERE `id`=%d",
        GetPlayerMoney(playerid), PlayerData[playerid][pBank],
        PlayerData[playerid][pLevel], PlayerData[playerid][pExp],
        PlayerData[playerid][pJob], PlayerData[playerid][pFaction],
        PlayerData[playerid][pGang], x, y, z, interior, world,
        pAge[playerid], pSex[playerid], pCity[playerid], pPhone[playerid],
        pHunger[playerid], pHasPhone[playerid], pHasRadio[playerid],
        PlayerData[playerid][pID]);
    mysql_tquery(g_SQL, query);
    return 1;
}
