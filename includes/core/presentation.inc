/* ============================================================
    presentation.inc
    Presentacion visual y bienvenida para el jugador tras login
   ============================================================ */
#if defined _presentation_inc_included
    #endinput
#endif
#define _presentation_inc_included

#include <a_samp>

#define DIALOG_PRESENTATION 300

stock PresentationOnPlayerConnect(playerid)
{
    SendClientMessage(playerid, 0x33CC33FF, "Bienvenido a RPGM RolePlay. Estamos verificando tu cuenta...");
    InterpolateCameraPos(playerid, 2040.5790, 1343.7682, 60.0000, 1958.3783, 1343.1572, 28.0000, 7000, CAMERA_MOVE);
    InterpolateCameraLookAt(playerid, 1958.3783, 1343.1572, 15.3746, 1958.3783, 1343.1572, 15.3746, 7000, CAMERA_MOVE);
    return 1;
}

stock ShowServerPresentation(playerid)
{
    SendClientMessage(playerid, 0x33CC33FF, "========================================");
    SendClientMessage(playerid, 0xFFFFFFFF, "RPGM RolePlay: crea tu historia, respeta el rol y progresa legal o ilegalmente.");
    SendClientMessage(playerid, 0xFFFFFFFF, "Usa /ayuda para ver comandos principales y /stats para revisar tu personaje.");
    SendClientMessage(playerid, 0x33CC33FF, "========================================");

    ShowPlayerDialog(playerid, DIALOG_PRESENTATION, DIALOG_STYLE_MSGBOX,
        "Bienvenido a RPGM RolePlay",
        "Objetivo:\nVivi una experiencia roleplay moderna con economia, banco y progreso de personaje.\n\nReglas basicas:\n- Respeta el rol y a los demas jugadores.\n- No uses cheats, bugs ni programas externos.\n- Segui las indicaciones del staff.\n\nPrimeros comandos:\n/ayuda - Lista de comandos\n/stats - Datos de tu personaje\n/banco - Ver efectivo y saldo bancario",
        "Empezar", "");
    return 1;
}
