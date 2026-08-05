-- ============================================================
--  ESQUEMA DE BASE DE DATOS - SERVIDOR ROLEPLAY SA-MP
--  Ejecutar este archivo UNA VEZ en tu servidor MySQL
--  (phpMyAdmin, HeidiSQL, DBeaver, o consola: mysql -u root -p < schema.sql)
-- ============================================================

CREATE DATABASE IF NOT EXISTS samp_rp CHARACTER SET utf8mb4;
USE samp_rp;

-- Cuentas de jugadores
CREATE TABLE IF NOT EXISTS `accounts` (
    `id`            INT(11) NOT NULL AUTO_INCREMENT,
    `username`      VARCHAR(24) NOT NULL,
    `password`      VARCHAR(129) NOT NULL,   -- hash whirlpool
    `admin_level`   TINYINT(2) NOT NULL DEFAULT 0,
    `money`         INT(11) NOT NULL DEFAULT 500,
    `bank`          INT(11) NOT NULL DEFAULT 0,
    `level`         INT(11) NOT NULL DEFAULT 1,
    `exp`           INT(11) NOT NULL DEFAULT 0,
    `job`           TINYINT(2) NOT NULL DEFAULT 0,
    `faction`       TINYINT(2) NOT NULL DEFAULT 0,
    `gang`          INT(11) NOT NULL DEFAULT 0,
    `pos_x`         FLOAT NOT NULL DEFAULT 1958.3783,
    `pos_y`         FLOAT NOT NULL DEFAULT 1343.1572,
    `pos_z`         FLOAT NOT NULL DEFAULT 15.3746,
    `interior`      INT(11) NOT NULL DEFAULT 0,
    `world`         INT(11) NOT NULL DEFAULT 0,
    `skin`          INT(11) NOT NULL DEFAULT 0,
    `age`           INT(11) NOT NULL DEFAULT 18,
    `sex`           TINYINT(1) NOT NULL DEFAULT 1,
    `city`          TINYINT(1) NOT NULL DEFAULT 1,
    `phone`         INT(11) NOT NULL DEFAULT 0,
    `has_phone`     TINYINT(1) NOT NULL DEFAULT 0,
    `has_radio`     TINYINT(1) NOT NULL DEFAULT 0,
    `hunger`        INT(11) NOT NULL DEFAULT 100,
    `health`        FLOAT NOT NULL DEFAULT 100,
    `armour`        FLOAT NOT NULL DEFAULT 0,
    `register_date` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    `last_login`    DATETIME NULL,
    `last_ip`       VARCHAR(45) NULL,
    `banned`        TINYINT(1) NOT NULL DEFAULT 0,
    `ban_reason`    VARCHAR(128) NULL,
    PRIMARY KEY (`id`),
    UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Inventario (uno-a-muchos con accounts)
CREATE TABLE IF NOT EXISTS `inventory` (
    `id`         INT(11) NOT NULL AUTO_INCREMENT,
    `account_id` INT(11) NOT NULL,
    `item_name`  VARCHAR(32) NOT NULL,
    `amount`     INT(11) NOT NULL DEFAULT 1,
    PRIMARY KEY (`id`),
    KEY `account_id` (`account_id`),
    CONSTRAINT `fk_inventory_account` FOREIGN KEY (`account_id`) REFERENCES `accounts`(`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Vehículos (personales, concesionario, facción)
CREATE TABLE IF NOT EXISTS `vehicles` (
    `id`         INT(11) NOT NULL AUTO_INCREMENT,
    `owner_id`   INT(11) NULL,           -- NULL = vehículo del sistema/facción
    `model`      INT(11) NOT NULL,
    `pos_x`      FLOAT NOT NULL,
    `pos_y`      FLOAT NOT NULL,
    `pos_z`      FLOAT NOT NULL,
    `pos_a`      FLOAT NOT NULL,
    `color1`     INT(11) NOT NULL DEFAULT 0,
    `color2`     INT(11) NOT NULL DEFAULT 0,
    `plate`      VARCHAR(16) NULL,
    `fuel`       INT(11) NOT NULL DEFAULT 100,
    `locked`     TINYINT(1) NOT NULL DEFAULT 1,
    `faction`    TINYINT(2) NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`),
    KEY `owner_id` (`owner_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Casas
CREATE TABLE IF NOT EXISTS `houses` (
    `id`          INT(11) NOT NULL AUTO_INCREMENT,
    `owner_id`    INT(11) NULL,
    `entrance_x`  FLOAT NOT NULL,
    `entrance_y`  FLOAT NOT NULL,
    `entrance_z`  FLOAT NOT NULL,
    `interior_id` INT(11) NOT NULL DEFAULT 1,
    `price`       INT(11) NOT NULL DEFAULT 10000,
    `locked`      TINYINT(1) NOT NULL DEFAULT 1,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Negocios
CREATE TABLE IF NOT EXISTS `businesses` (
    `id`       INT(11) NOT NULL AUTO_INCREMENT,
    `owner_id` INT(11) NULL,
    `name`     VARCHAR(32) NOT NULL,
    `type`     TINYINT(2) NOT NULL DEFAULT 0,
    `pos_x`    FLOAT NOT NULL,
    `pos_y`    FLOAT NOT NULL,
    `pos_z`    FLOAT NOT NULL,
    `price`    INT(11) NOT NULL DEFAULT 5000,
    `till`     INT(11) NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Bandas
CREATE TABLE IF NOT EXISTS `gangs` (
    `id`     INT(11) NOT NULL AUTO_INCREMENT,
    `name`   VARCHAR(32) NOT NULL,
    `leader_id` INT(11) NOT NULL,
    `color`  INT(11) NOT NULL DEFAULT 0,
    `territory` VARCHAR(64) NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Log de administración (baneos, kicks, warns)
CREATE TABLE IF NOT EXISTS `admin_logs` (
    `id`        INT(11) NOT NULL AUTO_INCREMENT,
    `admin`     VARCHAR(24) NOT NULL,
    `target`    VARCHAR(24) NOT NULL,
    `action`    VARCHAR(16) NOT NULL,
    `reason`    VARCHAR(128) NULL,
    `date`      DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


-- Amigos entre jugadores para /amigos
CREATE TABLE IF NOT EXISTS `friends` (
    `id` INT(11) NOT NULL AUTO_INCREMENT,
    `account_id` INT(11) NOT NULL,
    `friend_id` INT(11) NOT NULL,
    `status` TINYINT(1) NOT NULL DEFAULT 1,
    `created_at` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`),
    KEY `account_id` (`account_id`),
    KEY `friend_id` (`friend_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Puntos de tiendas y concesionarios usados por Nova Roleplay
CREATE TABLE IF NOT EXISTS `shop_points` (
    `id` INT(11) NOT NULL AUTO_INCREMENT,
    `name` VARCHAR(48) NOT NULL,
    `type` TINYINT(2) NOT NULL,
    `city` VARCHAR(32) NOT NULL,
    `pos_x` FLOAT NOT NULL,
    `pos_y` FLOAT NOT NULL,
    `pos_z` FLOAT NOT NULL,
    PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
