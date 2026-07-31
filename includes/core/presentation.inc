/* ============================================================
    presentation.inc
    Pantalla inicial, camara y bienvenida del servidor
   ============================================================ */
#if defined _presentation_inc_included
    #endinput
#endif
#define _presentation_inc_included

#include <a_samp>

#define DIALOG_SPLASH       299
#define DIALOG_PRESENTATION 300
#define INVALID_PLAYER_TD   PlayerText:INVALID_TEXT_DRAW

new PlayerText:gSplashTD[MAX_PLAYERS][8];

stock PresentationResetPlayerTextdraws(playerid)
{
    for (new i = 0; i < sizeof(gSplashTD[]); i++)
    {
        gSplashTD[playerid][i] = INVALID_PLAYER_TD;
    }
    return 1;
}

stock PresentationOnPlayerConnect(playerid)
{
    PresentationResetPlayerTextdraws(playerid);

    SendClientMessage(playerid, 0x9B30FFFF, "Cargando presentacion de Gran The Auto San Andreas...");
    GameTextForPlayer(playerid, "~p~GRAN THE AUTO~n~~w~SAN ANDREAS", 5000, 3);
    InterpolateCameraPos(playerid, 2040.5790, 1343.7682, 60.0000, 1958.3783, 1343.1572, 28.0000, 7000, CAMERA_MOVE);
    InterpolateCameraLookAt(playerid, 1958.3783, 1343.1572, 15.3746, 1958.3783, 1343.1572, 15.3746, 7000, CAMERA_MOVE);
    ShowSplashPresentation(playerid);
    return 1;
}

stock ShowSplashPresentation(playerid)
{
    gSplashTD[playerid][0] = CreatePlayerTextDraw(playerid, 320.0, 60.0, "_");
    PlayerTextDrawLetterSize(playerid, gSplashTD[playerid][0], 0.0, 27.0);
    PlayerTextDrawTextSize(playerid, gSplashTD[playerid][0], 610.0, 0.0);
    PlayerTextDrawAlignment(playerid, gSplashTD[playerid][0], 2);
    PlayerTextDrawUseBox(playerid, gSplashTD[playerid][0], 1);
    PlayerTextDrawBoxColor(playerid, gSplashTD[playerid][0], 0x000000CC);
    PlayerTextDrawShow(playerid, gSplashTD[playerid][0]);

    gSplashTD[playerid][1] = CreatePlayerTextDraw(playerid, 320.0, 92.0, "GRAN");
    PlayerTextDrawLetterSize(playerid, gSplashTD[playerid][1], 1.4, 5.4);
    PlayerTextDrawAlignment(playerid, gSplashTD[playerid][1], 2);
    PlayerTextDrawColor(playerid, gSplashTD[playerid][1], 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, gSplashTD[playerid][1], 2);
    PlayerTextDrawBackgroundColor(playerid, gSplashTD[playerid][1], 0x000000FF);
    PlayerTextDrawFont(playerid, gSplashTD[playerid][1], 3);
    PlayerTextDrawShow(playerid, gSplashTD[playerid][1]);

    gSplashTD[playerid][2] = CreatePlayerTextDraw(playerid, 320.0, 150.0, "DE AUTO");
    PlayerTextDrawLetterSize(playerid, gSplashTD[playerid][2], 1.1, 4.4);
    PlayerTextDrawAlignment(playerid, gSplashTD[playerid][2], 2);
    PlayerTextDrawColor(playerid, gSplashTD[playerid][2], 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, gSplashTD[playerid][2], 2);
    PlayerTextDrawBackgroundColor(playerid, gSplashTD[playerid][2], 0x000000FF);
    PlayerTextDrawFont(playerid, gSplashTD[playerid][2], 3);
    PlayerTextDrawShow(playerid, gSplashTD[playerid][2]);

    gSplashTD[playerid][3] = CreatePlayerTextDraw(playerid, 320.0, 205.0, "San Andreas");
    PlayerTextDrawLetterSize(playerid, gSplashTD[playerid][3], 0.9, 4.0);
    PlayerTextDrawAlignment(playerid, gSplashTD[playerid][3], 2);
    PlayerTextDrawColor(playerid, gSplashTD[playerid][3], 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, gSplashTD[playerid][3], 2);
    PlayerTextDrawBackgroundColor(playerid, gSplashTD[playerid][3], 0x9B30FFFF);
    PlayerTextDrawFont(playerid, gSplashTD[playerid][3], 1);
    PlayerTextDrawShow(playerid, gSplashTD[playerid][3]);

    gSplashTD[playerid][4] = CreatePlayerTextDraw(playerid, 320.0, 275.0, "EL SERVIDOR MODERNO");
    PlayerTextDrawLetterSize(playerid, gSplashTD[playerid][4], 0.62, 2.5);
    PlayerTextDrawAlignment(playerid, gSplashTD[playerid][4], 2);
    PlayerTextDrawColor(playerid, gSplashTD[playerid][4], 0xB33CFFFF);
    PlayerTextDrawSetOutline(playerid, gSplashTD[playerid][4], 2);
    PlayerTextDrawFont(playerid, gSplashTD[playerid][4], 2);
    PlayerTextDrawShow(playerid, gSplashTD[playerid][4]);

    gSplashTD[playerid][5] = CreatePlayerTextDraw(playerid, 320.0, 330.0, "TODOS LOS ACTOS Y COSAS REALIZADAS EN EL SERVIDOR");
    PlayerTextDrawLetterSize(playerid, gSplashTD[playerid][5], 0.33, 1.55);
    PlayerTextDrawAlignment(playerid, gSplashTD[playerid][5], 2);
    PlayerTextDrawColor(playerid, gSplashTD[playerid][5], 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, gSplashTD[playerid][5], 1);
    PlayerTextDrawFont(playerid, gSplashTD[playerid][5], 2);
    PlayerTextDrawShow(playerid, gSplashTD[playerid][5]);

    gSplashTD[playerid][6] = CreatePlayerTextDraw(playerid, 320.0, 350.0, "NO IMPLICA NADA DE LA VIDA REAL");
    PlayerTextDrawLetterSize(playerid, gSplashTD[playerid][6], 0.42, 1.85);
    PlayerTextDrawAlignment(playerid, gSplashTD[playerid][6], 2);
    PlayerTextDrawColor(playerid, gSplashTD[playerid][6], 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, gSplashTD[playerid][6], 1);
    PlayerTextDrawFont(playerid, gSplashTD[playerid][6], 2);
    PlayerTextDrawShow(playerid, gSplashTD[playerid][6]);

    gSplashTD[playerid][7] = CreatePlayerTextDraw(playerid, 320.0, 385.0, "Presiona CONTINUAR para iniciar registro o login");
    PlayerTextDrawLetterSize(playerid, gSplashTD[playerid][7], 0.35, 1.45);
    PlayerTextDrawAlignment(playerid, gSplashTD[playerid][7], 2);
    PlayerTextDrawColor(playerid, gSplashTD[playerid][7], 0x9B30FFFF);
    PlayerTextDrawSetOutline(playerid, gSplashTD[playerid][7], 1);
    PlayerTextDrawFont(playerid, gSplashTD[playerid][7], 2);
    PlayerTextDrawShow(playerid, gSplashTD[playerid][7]);

    ShowPlayerDialog(playerid, DIALOG_SPLASH, DIALOG_STYLE_MSGBOX,
        "Gran The Auto San Andreas",
        "{B33CFF}GRAN THE AUTO SAN ANDREAS\n{FFFFFF}El servidor moderno\n\n{C0C0C0}Esta ventana negra aparece antes del registro/login.\n{FFFFFF}Presiona Continuar para verificar tu cuenta.",
        "Continuar", "Salir");
    return 1;
}

stock HideSplashPresentation(playerid)
{
    for (new i = 0; i < sizeof(gSplashTD[]); i++)
    {
        if (gSplashTD[playerid][i] != INVALID_PLAYER_TD)
        {
            PlayerTextDrawHide(playerid, gSplashTD[playerid][i]);
            PlayerTextDrawDestroy(playerid, gSplashTD[playerid][i]);
            gSplashTD[playerid][i] = INVALID_PLAYER_TD;
        }
    }
    return 1;
}

stock PresentationOnDialogResponse(playerid, dialogid, response)
{
    if (dialogid != DIALOG_SPLASH) return 0;

    HideSplashPresentation(playerid);
    if (!response) return Kick(playerid), 1;
    AccountsOnPlayerConnect(playerid);
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
