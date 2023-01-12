DROP TABLE dm_componentdeps;

CREATE TABLE dm_componentdeps
(
    compid integer NOT NULL,
    packagename character varying(1024),
    packageversion character varying(256),
    deptype character varying(20),
    name character varying(80),
    url character varying(1024),
    summary character varying(8096),
    CONSTRAINT dm_componentdeps_pkey PRIMARY KEY (compid, packagename, packageversion)
);
