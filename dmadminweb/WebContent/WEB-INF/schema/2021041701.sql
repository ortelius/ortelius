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

COMMENT ON TABLE dm_componentdeps IS 'Component Dependencies';

COMMENT ON COLUMN dm_componentdeps.compid IS 'Component Id';
COMMENT ON COLUMN dm_componentdeps.packagename IS 'Name of the package';
COMMENT ON COLUMN dm_componentdeps.packageversion IS 'Version of the package';
COMMENT ON COLUMN dm_componentdeps.cve IS 'Name of the CVE';
COMMENT ON COLUMN dm_componentdeps.cve_url IS 'Url to the CVE details in the CVE website';
COMMENT ON COLUMN dm_componentdeps.license IS 'Name of the License for the package';
COMMENT ON COLUMN dm_componentdeps.license_url IS 'Url to the License File';
