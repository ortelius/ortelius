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

COMMENT ON TABLE dm_componentdeps IS 'Component Dependencies';
COMMENT ON COLUMN dm_componentdeps.compid IS 'Component Id';
COMMENT ON COLUMN dm_componentdeps.packagename IS 'Name of the package';
COMMENT ON COLUMN dm_componentdeps.packageversion IS 'Version of the package';
COMMENT ON COLUMN dm_componentdeps.deptype IS 'Type of dep (cve/license)';
COMMENT ON COLUMN dm_componentdeps.name IS 'Name of the CVE or License';
COMMENT ON COLUMN dm_componentdeps.url IS 'Url to the CVE or License details';
COMMENT ON COLUMN dm_componentdeps.summary IS 'Summary of the CVE or license';

