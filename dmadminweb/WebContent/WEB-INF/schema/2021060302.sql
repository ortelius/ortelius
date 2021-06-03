alter table dm.dm_componentitem add IF NOT EXISTS serviceowner character varying(80);
alter table dm.dm_componentitem add IF NOT EXISTS serviceowneremail character varying(80);
alter table dm.dm_componentitem add IF NOT EXISTS serviceownerphone character varying(20);
alter table dm.dm_componentitem add IF NOT EXISTS slackchannel character varying(1024);
alter table dm.dm_componentitem add IF NOT EXISTS discordchannel character varying(1024);
alter table dm.dm_componentitem add IF NOT EXISTS hipchatchannel character varying(1024);
alter table dm.dm_componentitem add IF NOT EXISTS pagerdutyurl character varying(1024);
alter table dm.dm_componentitem add IF NOT EXISTS pagerdutybusinessurl character varying(1024);
