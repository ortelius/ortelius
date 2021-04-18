CREATE TABLE IF NOT EXISTS dm_componentversion
(
    compid integer primary key,
    packagename varchar(1024),
    packageversion varchar(256),
    cve varchar(80),
    cve_url varchar(1024),
    license varchar(256),
    license_url varchar(1024)
);

COMMENT ON TABLE dm_componentversion IS 'Component Version';

COMMENT ON COLUMN dm_componentversion.compid IS 'Component Id';
COMMENT ON COLUMN dm_componentversion.packagename IS 'Name of the package';
COMMENT ON COLUMN dm_componentversion.packageversion IS 'Version of the package';
COMMENT ON COLUMN dm_componentversion.cve IS 'Name of the CVE';
COMMENT ON COLUMN dm_componentversion.cve_url IS 'Url to the CVE details in the CVE website';
COMMENT ON COLUMN dm_componentversion.license IS 'Name of the License for the package';
COMMENT ON COLUMN dm_componentversion.license_url IS 'Url to the License File';
