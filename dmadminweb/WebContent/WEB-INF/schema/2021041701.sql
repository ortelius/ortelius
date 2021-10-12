-- Copyright (c) 2021 Linux Foundation
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.

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
