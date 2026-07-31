/* ============================================================
    database.inc
    Maneja la conexión MySQL (plugin BlueG mysql R41-4)
    Requiere: a_mysql.inc (viene con el plugin)
   ============================================================ */
#if defined _database_inc_included
    #endinput
#endif
#define _database_inc_included

#include <a_mysql>

// -------- Datos de conexión: EDITAR con tus credenciales --------
#define DB_HOST     "127.0.0.1"
#define DB_USER     "root"
#define DB_PASS     ""
#define DB_NAME     "samp_rp"
#define DB_PORT     3306
// ------------------------------------------------------------------

new MySQL:g_SQL;

stock ConnectDatabase()
{
    new MySQLOpt: option_id = mysql_init_options();
    mysql_set_option(option_id, AUTO_RECONNECT, true);

    g_SQL = mysql_connect(DB_HOST, DB_USER, DB_PASS, DB_NAME, option_id);

    if (mysql_errno(g_SQL) != 0)
    {
        print("=====================================================");
        print("[DATABASE] ERROR: No se pudo conectar a MySQL.");
        print("[DATABASE] Revisa DB_HOST / DB_USER / DB_PASS / DB_NAME en database.inc");
        print("=====================================================");
        return 0;
    }

    print("=====================================================");
    print("[DATABASE] Conexión a MySQL establecida correctamente.");
    print("=====================================================");
    return 1;
}

stock DisconnectDatabase()
{
    mysql_close(g_SQL);
}
