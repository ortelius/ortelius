CREATE SEQUENCE IF NOT EXISTS dm.dm_compitem_id_seq;
SELECT setval('dm_compitem_id_seq', max(id)) FROM dm.dm_componentitem;
