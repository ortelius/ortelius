ALTER TABLE IF EXISTS dm.dm_domain
    ADD COLUMN IF NOT EXISTS fullname character varying COLLATE pg_catalog."default";

UPDATE dm.dm_domain SET fullname = dm.fulldomain(id);
