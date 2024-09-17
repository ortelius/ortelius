ALTER TABLE dm.dm_componentitem
    ADD COLUMN IF NOT EXISTS purl character varying(1024);
