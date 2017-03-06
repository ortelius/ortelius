CREATE TABLE dm.dm_discovery
(
  id integer NOT NULL,
  serverid integer NOT NULL,
  servername character varying(64) NOT NULL,
  targetfilename character varying(2048),
  md5 character varying(256),
  discovery_time integer
)  
WITH (
  OIDS=FALSE
);