-- add the new servertype table to associate component type to a server
-- 
create table dm_servercomptype(
serverid	integer not null,
comptypeid  integer not null
);

