alter table dm.dm_user add IF NOT EXISTS endpointfilter character varying(1024);
alter table dm.dm_historyattachment add IF NOT EXISTS lineno integer;
