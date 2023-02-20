CREATE TABLE IF NOT EXISTS dm_componentdeps
(
    compid integer primary key,
    packagename varchar(1024),
    packageversion varchar(256),
    cve varchar(80),
    cve_url varchar(1024),
    license varchar(256),
    license_url varchar(1024)
);
