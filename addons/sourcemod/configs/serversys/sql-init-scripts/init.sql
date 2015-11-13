SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";

CREATE TABLE IF NOT EXISTS `servers` (
  `id` int(11) NOT NULL,
  `name` varchar(64) NOT NULL,
  `ip` varchar(64) NULL DEFAULT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `maps` (
  `id` int(11) NOT NULL,
  `game` int(11) NOT NULL,
  `name` varchar(64) NOT NULL,
  `lastplayed` bigint(20) NOT NULL DEFAULT 0
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

ALTER TABLE `maps`
  ADD UNIQUE KEY `name_game` (`game`, `name`);


CREATE TABLE IF NOT EXISTS `users` (
  `pid` int(11) NOT NULL,
  `auth` int(11) NOT NULL,
  `name` varchar(64) NOT NULL,
  `lastseen` bigint(20) NOT NULL DEFAULT 0
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS `playtime` (
  `row` int(11) NOT NULL,
  `pid` int(11) NOT NULL,
  `sid` int(11) NOT NULL,
  `time` bigint(20) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `maptime` (
  `row` int(11) NOT NULL,
  `mid` int(11) NOT NULL,
  `sid` int(11) NOT NULL,
  `time` bigint(20) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


ALTER TABLE `servers`
  ADD PRIMARY KEY (`id`), ADD KEY `id` (`id`), ADD KEY `name` (`name`), ADD KEY `ip` (`ip`), ADD INDEX(`id`), ADD INDEX(`name`), ADD INDEX(`ip`);

ALTER TABLE `users`
  ADD PRIMARY KEY (`pid`), ADD KEY `pid` (`pid`), ADD KEY `auth` (`auth`), ADD KEY `name` (`name`), ADD KEY `lastseen` (`lastseen`), ADD INDEX(`pid`), ADD INDEX(`auth`), ADD INDEX(`name`), ADD INDEX(`lastseen`);

ALTER TABLE `playtime`
  ADD PRIMARY KEY (`row`), ADD KEY `row` (`row`), ADD KEY `pid` (`pid`), ADD KEY `sid` (`sid`), ADD KEY `time` (`time`), ADD INDEX(`row`), ADD INDEX(`pid`), ADD INDEX(`sid`), ADD INDEX(`time`);

ALTER TABLE `maptime`
  ADD PRIMARY KEY (`row`), ADD UNIQUE KEY `mid_sid` (`mid`, `sid`), ADD KEY `row` (`row`), ADD KEY `mid` (`mid`), ADD KEY `sid` (`sid`), ADD KEY `time` (`time`), ADD INDEX(`row`), ADD INDEX(`mid`), ADD INDEX(`sid`), ADD INDEX(`time`);

ALTER TABLE `maps`
  ADD PRIMARY KEY (`id`), ADD KEY `game` (`game`), ADD KEY `name` (`name`), ADD KEY `lastplayed` (`lastplayed`), ADD INDEX(`id`), ADD INDEX(`game`), ADD INDEX(`name`);

ALTER TABLE `servers`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `users`
  MODIFY `pid` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `playtime`
  MODIFY `row` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `maps`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `maptime`
  MODIFY `row` int(11) NOT NULL AUTO_INCREMENT;
