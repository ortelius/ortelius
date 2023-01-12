DROP TABLE dm.dm_helm;

CREATE TABLE dm.dm_helm
(
    appid integer NOT NULL,
    deployid integer,
    filename character varying(1024),
    lineno integer,
    line character varying(4096),
    compid integer,
    envid integer,
    helmrepo character varying(256),
    helmrepourl character varying(512),
    chartname character varying(80),
    chartversion character varying(80),
    chartdigest character varying(256),
    appparentid integer
);

CREATE TABLE dm_helmimages (
    appid integer NOT NULL,
    deployid integer,
    compid integer,
    envid integer,
    chartdigest character varying(512),
    registry character varying(256),
    organization character varying(256),
    imagename character varying(256),
    imagetag character varying(256),
    imagedigest character varying(512)
);
