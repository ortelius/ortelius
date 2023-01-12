-- Alter dm_discovery table to hold original and detected MD5 checksums
ALTER TABLE dm.dm_discovery RENAME md5 to detectedmd5;
ALTER TABLE dm.dm_discovery ADD deployedmd5 character varying(256);
