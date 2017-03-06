-- add the new serverstatus table and add uname to server table
-- 
create table dm_serverstatus(
serverid	integer not null,
nameresolution	char(1),
ping		char(1),
connection	char(1),
basedir		char(1),
ipaddr		char(16),
pingtime	integer,
lasterror	varchar(1024),
lasttime	integer
);

alter table dm_server add uname varchar(256);

