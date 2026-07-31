/* ============================================================
    economy.inc
    Comandos basicos de dinero y banco
    Requiere: accounts.inc, ZCMD (zcmd.inc) - descargar aparte
   ============================================================ */
#if defined _economy_inc_included
    #endinput
#endif
#define _economy_inc_included

#include "../includes/core/accounts.inc"

CMD:banco(playerid, params[])
{
    new msg[128];
    format(msg, sizeof(msg), "== BANCO ==\nEfectivo: $%d\nBanco: $%d",
        GetPlayerMoney(playerid), PlayerData[playerid][pBank]);
    ShowPlayerDialog(playerid, 200, DIALOG_STYLE_MSGBOX, "Banco", msg, "Cerrar", "");
    return 1;
}

CMD:depositar(playerid, params[])
{
    new amount;
    if (sscanf(params, "d", amount))
        return SendClientMessage(playerid, 0xFFFFFFFF, "USO: /depositar [cantidad]"), 1;

    if (amount <= 0 || amount > GetPlayerMoney(playerid))
        return SendClientMessage(playerid, 0xFF0000FF, "No tienes esa cantidad de dinero encima."), 1;

    GivePlayerMoney(playerid, -amount);
    PlayerData[playerid][pBank] += amount;

    new msg[64];
    format(msg, sizeof(msg), "Depositaste $%d. Saldo en banco: $%d", amount, PlayerData[playerid][pBank]);
    SendClientMessage(playerid, 0x33CC33FF, msg);
    return 1;
}

CMD:retirar(playerid, params[])
{
    new amount;
    if (sscanf(params, "d", amount))
        return SendClientMessage(playerid, 0xFFFFFFFF, "USO: /retirar [cantidad]"), 1;

    if (amount <= 0 || amount > PlayerData[playerid][pBank])
        return SendClientMessage(playerid, 0xFF0000FF, "No tienes esa cantidad de dinero en el banco."), 1;

    PlayerData[playerid][pBank] -= amount;
    GivePlayerMoney(playerid, amount);

    new msg[64];
    format(msg, sizeof(msg), "Retiraste $%d. Saldo en banco: $%d", amount, PlayerData[playerid][pBank]);
    SendClientMessage(playerid, 0x33CC33FF, msg);
    return 1;
}

CMD:pagar(playerid, params[])
{
    new targetid, amount;
    if (sscanf(params, "ud", targetid, amount))
        return SendClientMessage(playerid, 0xFFFFFFFF, "USO: /pagar [id] [cantidad]"), 1;

    if (targetid == INVALID_PLAYER_ID || !IsPlayerConnected(targetid))
        return SendClientMessage(playerid, 0xFF0000FF, "Ese jugador no esta conectado."), 1;

    if (amount <= 0 || amount > GetPlayerMoney(playerid))
        return SendClientMessage(playerid, 0xFF0000FF, "No tienes esa cantidad de dinero."), 1;

    GivePlayerMoney(playerid, -amount);
    GivePlayerMoney(targetid, amount);

    new msg[96], name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    format(msg, sizeof(msg), "Le pagaste $%d a %s.", amount, ReturnName(targetid));
    SendClientMessage(playerid, 0x33CC33FF, msg);
    format(msg, sizeof(msg), "%s te pago $%d.", name, amount);
    SendClientMessage(targetid, 0x33CC33FF, msg);
    return 1;
}

stock ReturnName(playerid)
{
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    return name;
}
