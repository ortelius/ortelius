CREATE TABLE dm.dm_discovery
(
  id integer NOT NULL,
  serverid integer NOT NULL,
  servername varchar2(64) NOT NULL,
  targetfilename varchar2(2048),
  md5 varchar2(256),
  discovery_time integer
);
