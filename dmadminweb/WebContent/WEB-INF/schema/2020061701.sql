alter table dm.dm_historyattachment add IF NOT EXISTS line character varying(4096);
alter table dm.dm_historyattachment add IF NOT EXISTS lineno integer;
