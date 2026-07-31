-- ============================================================
-- MIGRACION 002 - Datos de creacion de personaje
-- Ejecutar si ya tenias creada la tabla accounts antes de esta actualizacion.
-- ============================================================

USE samp_rp;

ALTER TABLE `accounts`
    ADD COLUMN `age` TINYINT(3) NOT NULL DEFAULT 18 AFTER `gang`,
    ADD COLUMN `sex` TINYINT(1) NOT NULL DEFAULT 0 AFTER `age`,
    ADD COLUMN `spawn_city` TINYINT(1) NOT NULL DEFAULT 0 AFTER `sex`;
