-- add the new servertype table to associate component type to a server
-- 

delete from dm.dm_fragmenttext where fragmentid = 270;
delete from dm.dm_fragments where id = 270;
delete from dm.dm_fragmentattrs where typeid = 270;