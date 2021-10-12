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

CREATE OR REPLACE FUNCTION dm.create_language_plpgsql() RETURNS BOOLEAN AS $$ CREATE LANGUAGE plpgsql; SELECT TRUE; $$ LANGUAGE SQL; SELECT CASE WHEN NOT (SELECT  TRUE AS exists FROM    pg_language WHERE   lanname = 'plpgsql' UNION SELECT  FALSE AS exists ORDER BY exists DESC LIMIT 1) THEN dm.create_language_plpgsql() ELSE FALSE END AS plpgsql_created; DROP FUNCTION dm.create_language_plpgsql();