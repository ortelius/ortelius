-- add the new servertype table to associate component type to a server
--

insert into dm.dm_servercomptype select distinct c.serverid, a.id from dm.dm_type a, dm.dm_component b, dm.dm_compsallowedonserv c where a.id = b.comptypeid and b.id = c.compid
