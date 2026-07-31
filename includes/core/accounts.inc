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
#define DIALOG_REG_AGE   102
#define DIALOG_REG_SEX   103
#define DIALOG_REG_SPAWN 104
#define DIALOG_REG_SKIN  105

#define MAX_LOGIN_ATTEMPTS 3

#define DEFAULT_SPAWN_X 1958.3783
#define DEFAULT_SPAWN_Y 1343.1572
#define DEFAULT_SPAWN_Z 15.3746
#define DEFAULT_SPAWN_A 269.1425
#define DEFAULT_SKIN    0

#define SPAWN_CITY_LS    0
#define SPAWN_CITY_LV    1
#define SPAWN_CITY_SF    2

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
    pLoginAttempts,
    pAge,
    pSex,
    pSpawnCity,
    pSkin
};
new PlayerData[MAX_PLAYERS][E_PLAYER_DATA];
new RegisterPassword[MAX_PLAYERS][129];

// -------------------------------------------------------------
// Conexión de un jugador: revisamos si la cuenta existe
// -------------------------------------------------------------
stock AccountsOnPlayerConnect(playerid)
{
    PlayerData[playerid][pLoggedIn] = false;
    PlayerData[playerid][pLoginAttempts] = 0;
    RegisterPassword[playerid][0] = EOS;

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
        new title[64], body[512];
        format(title, sizeof(title), "RPGM RolePlay | Acceso de %s", name);
        format(body, sizeof(body),
            "{33CCFF}Bienvenido de nuevo, {FFFFFF}%s{33CCFF}.\n\n{FFFFFF}Tu personaje ya existe en nuestra base de datos.\n{C0C0C0}Para continuar, escribe tu contraseña.\n\n{33CC33}Sistema conectado correctamente.\n{FFCC00}Intentos disponibles: %d",
            name, MAX_LOGIN_ATTEMPTS);
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, title, body,
            "Ingresar", "Salir");
    }
    else
    {
        // No existe -> registrar
        new title[64], body[512];
        format(title, sizeof(title), "RPGM RolePlay | Registro de %s", name);
        format(body, sizeof(body),
            "{33CCFF}Bienvenido a {FFFFFF}RPGM RolePlay{33CCFF}.\n\n{FFFFFF}No encontramos una cuenta con tu nombre.\n{C0C0C0}Crea una contraseña para guardar tu personaje, dinero y progreso.\n\n{FFCC00}Minimo: 5 caracteres.\n{33CC33}Despues del registro apareceras en el spawn inicial.");
        ShowPlayerDialog(playerid, DIALOG_REGISTER, DIALOG_STYLE_PASSWORD, title, body,
            "Crear cuenta", "Salir");
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
                "RPGM RolePlay | Registro",
                "{FF0000}La contraseña es demasiado corta.\n\n{FFFFFF}Usa al menos 5 caracteres para proteger tu cuenta.",
                "Intentar", "Salir");
            return 1;
        }

        WP_Hash(RegisterPassword[playerid], sizeof(RegisterPassword[]), inputtext);
        ShowPlayerDialog(playerid, DIALOG_REG_AGE, DIALOG_STYLE_INPUT,
            "RPGM RolePlay | Edad",
            "{FFFFFF}Ingresa la edad de tu personaje.\n\n{C0C0C0}Edad permitida: 16 a 80 anos.",
            "Siguiente", "Salir");
        return 1;
    }

    if (dialogid == DIALOG_REG_AGE)
    {
        if (!response) return Kick(playerid), 1;

        new age;
        if (sscanf(inputtext, "d", age) || age < 16 || age > 80)
        {
            ShowPlayerDialog(playerid, DIALOG_REG_AGE, DIALOG_STYLE_INPUT,
                "RPGM RolePlay | Edad",
                "{FF0000}Edad invalida.\n\n{FFFFFF}Ingresa una edad entre 16 y 80 anos.",
                "Siguiente", "Salir");
            return 1;
        }

        PlayerData[playerid][pAge] = age;
        ShowPlayerDialog(playerid, DIALOG_REG_SEX, DIALOG_STYLE_LIST,
            "RPGM RolePlay | Sexo",
            "Masculino\nFemenino",
            "Siguiente", "Salir");
        return 1;
    }

    if (dialogid == DIALOG_REG_SEX)
    {
        if (!response) return Kick(playerid), 1;

        PlayerData[playerid][pSex] = listitem;
        ShowPlayerDialog(playerid, DIALOG_REG_SKIN, DIALOG_STYLE_LIST,
            "RPGM RolePlay | Ropa inicial",
            "Ropa urbana\nRopa casual\nRopa elegante\nRopa de barrio",
            "Siguiente", "Salir");
        return 1;
    }

    if (dialogid == DIALOG_REG_SKIN)
    {
        if (!response) return Kick(playerid), 1;

        switch (listitem)
        {
            case 1: PlayerData[playerid][pSkin] = 26;
            case 2: PlayerData[playerid][pSkin] = 60;
            case 3: PlayerData[playerid][pSkin] = 21;
            default: PlayerData[playerid][pSkin] = DEFAULT_SKIN;
        }

        ShowPlayerDialog(playerid, DIALOG_REG_SPAWN, DIALOG_STYLE_LIST,
            "RPGM RolePlay | Lugar de aparicion",
            "Los Santos\nLas Venturas\nSan Fierro",
            "Crear", "Salir");
        return 1;
    }

    if (dialogid == DIALOG_REG_SPAWN)
    {
        if (!response) return Kick(playerid), 1;

        PlayerData[playerid][pSpawnCity] = listitem;

        new name[MAX_PLAYER_NAME], query[420];
        GetPlayerName(playerid, name, sizeof(name));

        new Float:x, Float:y, Float:z, Float:a;
        GetSpawnByCity(PlayerData[playerid][pSpawnCity], x, y, z, a);

        mysql_format(g_SQL, query, sizeof(query),
            "INSERT INTO `accounts` (`username`, `password`, `age`, `sex`, `spawn_city`, `skin`, `pos_x`, `pos_y`, `pos_z`) VALUES ('%e', '%e', %d, %d, %d, %d, %f, %f, %f)",
            name, RegisterPassword[playerid], PlayerData[playerid][pAge], PlayerData[playerid][pSex], PlayerData[playerid][pSpawnCity], PlayerData[playerid][pSkin], x, y, z);
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
    SendClientMessage(playerid, 0x33CC33FF, "Tu cuenta fue creada con exito. Preparando tu personaje...");
    PlayerData[playerid][pID] = cache_insert_id();
    PlayerData[playerid][pLoggedIn] = true;
    LoadPlayerStats(playerid);

    new Float:x, Float:y, Float:z, Float:a;
    GetSpawnByCity(PlayerData[playerid][pSpawnCity], x, y, z, a);
    SpawnAuthenticatedPlayer(playerid, x, y, z, a, PlayerData[playerid][pSkin], 0, 0);
    ShowServerPresentation(playerid);
    return 1;
}

forward OnAccountLogin(playerid);
public OnAccountLogin(playerid)
{
    if (!IsPlayerConnected(playerid)) return 1;

    if (cache_num_rows() > 0)
    {
        PlayerData[playerid][pLoggedIn] = true;

        new id, adminlvl, money, bank, level, exp, job, faction, gang, age, sex, spawncity;
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
        cache_get_value_name_int(0, "spawn_city", spawncity);

        PlayerData[playerid][pID] = id;
        PlayerData[playerid][pAdminLevel] = adminlvl;
        PlayerData[playerid][pMoney] = money;
        PlayerData[playerid][pBank] = bank;
        PlayerData[playerid][pLevel] = level;
        PlayerData[playerid][pExp] = exp;
        PlayerData[playerid][pJob] = job;
        PlayerData[playerid][pFaction] = faction;
        PlayerData[playerid][pGang] = gang;
        PlayerData[playerid][pAge] = age;
        PlayerData[playerid][pSex] = sex;
        PlayerData[playerid][pSpawnCity] = spawncity;

        new posx, posy, posz, Float:fx, Float:fy, Float:fz, Float:facing, interior, world, skin;
        cache_get_value_name_float(0, "pos_x", fx);
        cache_get_value_name_float(0, "pos_y", fy);
        cache_get_value_name_float(0, "pos_z", fz);
        cache_get_value_name_int(0, "interior", interior);
        cache_get_value_name_int(0, "world", world);
        cache_get_value_name_int(0, "skin", skin);
        PlayerData[playerid][pSkin] = skin;
        #pragma unused posx, posy, posz, facing

        SpawnAuthenticatedPlayer(playerid, fx, fy, fz, DEFAULT_SPAWN_A, skin, interior, world);
        GivePlayerMoney(playerid, PlayerData[playerid][pMoney]);

        SendClientMessage(playerid, 0x33CC33FF, "Has iniciado sesion correctamente. Cargando tu personaje...");
        ShowServerPresentation(playerid);
    }
    else
    {
        if (PlayerData[playerid][pLoginAttempts] >= MAX_LOGIN_ATTEMPTS)
        {
            SendClientMessage(playerid, 0xFF0000FF, "Superaste el numero maximo de intentos. Has sido expulsado.");
            Kick(playerid);
            return 1;
        }

        new title[64], msg[256];
        format(title, sizeof(title), "Iniciar sesion - Intento %d/%d", PlayerData[playerid][pLoginAttempts], MAX_LOGIN_ATTEMPTS);
        format(msg, sizeof(msg), "{FF0000}Contraseña incorrecta.\n\n{FFFFFF}Revisa tus datos e intenta de nuevo.\n{FFCC00}Intentos restantes: %d", MAX_LOGIN_ATTEMPTS - PlayerData[playerid][pLoginAttempts]);
        ShowPlayerDialog(playerid, DIALOG_LOGIN, DIALOG_STYLE_PASSWORD, title, msg, "Ingresar", "Salir");
    }
    return 1;
}

// -------------------------------------------------------------
// Guardar / cargar stats adicionales (se llama tras crear cuenta)
// -------------------------------------------------------------
stock LoadPlayerStats(playerid)
{
    PlayerData[playerid][pMoney] = 500;
    PlayerData[playerid][pBank] = 0;
    PlayerData[playerid][pLevel] = 1;
    PlayerData[playerid][pExp] = 0;
    PlayerData[playerid][pJob] = 0;
    PlayerData[playerid][pFaction] = 0;
    PlayerData[playerid][pGang] = 0;
    if (PlayerData[playerid][pSkin] <= 0) PlayerData[playerid][pSkin] = DEFAULT_SKIN;
    if (PlayerData[playerid][pAge] < 16) PlayerData[playerid][pAge] = 18;
    return 1;
}

stock GetSpawnByCity(spawncity, &Float:x, &Float:y, &Float:z, &Float:a)
{
    switch (spawncity)
    {
        case SPAWN_CITY_LV:
        {
            x = 2022.6012; y = 1343.1572; z = 10.8203; a = 269.1425;
        }
        case SPAWN_CITY_SF:
        {
            x = -1985.4635; y = 137.7074; z = 27.6875; a = 90.0000;
        }
        default:
        {
            x = DEFAULT_SPAWN_X; y = DEFAULT_SPAWN_Y; z = DEFAULT_SPAWN_Z; a = DEFAULT_SPAWN_A;
        }
    }
    return 1;
}

stock SpawnAuthenticatedPlayer(playerid, Float:x, Float:y, Float:z, Float:angle, skin, interior, world)
{
    ResetPlayerMoney(playerid);
    SetPlayerInterior(playerid, interior);
    SetPlayerVirtualWorld(playerid, world);
    SetSpawnInfo(playerid, NO_TEAM, skin, x, y, z, angle, 0, 0, 0, 0, 0, 0);
    TogglePlayerSpectating(playerid, false);
    SpawnPlayer(playerid);
    SetPlayerPos(playerid, x, y, z);
    SetPlayerFacingAngle(playerid, angle);
    SetPlayerSkin(playerid, skin);
    SetCameraBehindPlayer(playerid);
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

    new query[520];
    mysql_format(g_SQL, query, sizeof(query),
        "UPDATE `accounts` SET `money`=%d, `bank`=%d, `level`=%d, `exp`=%d, \
         `job`=%d, `faction`=%d, `gang`=%d, `age`=%d, `sex`=%d, `spawn_city`=%d, `pos_x`=%f, `pos_y`=%f, `pos_z`=%f, \
         `interior`=%d, `world`=%d, `last_login`=NOW() WHERE `id`=%d",
        GetPlayerMoney(playerid), PlayerData[playerid][pBank],
        PlayerData[playerid][pLevel], PlayerData[playerid][pExp],
        PlayerData[playerid][pJob], PlayerData[playerid][pFaction],
        PlayerData[playerid][pGang], PlayerData[playerid][pAge], PlayerData[playerid][pSex],
        PlayerData[playerid][pSpawnCity], x, y, z, interior, world,
        PlayerData[playerid][pID]);
    mysql_tquery(g_SQL, query);
    return 1;
}
