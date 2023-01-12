-- Alter dm_discovery table to hold original and detected MD5 checksums
ALTER TABLE dm.dm_discovery RENAME COLUMN md5 to detectedmd5;
ALTER TABLE dm.dm_discovery ADD deployedmd5 varchar2(256);
