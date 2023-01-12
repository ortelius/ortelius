CREATE TABLE dm_helm (
    appid integer NOT NULL,
    deployid integer,
    filename character varying(1024),
    lineno integer,
    line character varying(4096)
);
