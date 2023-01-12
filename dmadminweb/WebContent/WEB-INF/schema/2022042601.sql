CREATE TABLE dm_vulns
(
    packagename character varying(1024) NOT NULL,
    packageversion character varying(256) NOT NULL,
    id character varying(80) NOT NULL,
    purl character varying(1024),
    summary character varying(8096),
    cvss character varying(256),
    risklevel character varying(256),
    CONSTRAINT dm_vulns_pkey PRIMARY KEY (packagename, packageversion, id)
);
