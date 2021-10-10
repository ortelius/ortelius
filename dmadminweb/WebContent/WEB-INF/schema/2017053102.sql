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

CREATE OR REPLACE FUNCTION dm.queue_event() RETURNS TRIGGER AS $$ DECLARE data xml; BEGIN data = xmlelement(name row,xmlattributes(NEW.id,NEW.clientid,NEW.credentials,NEW.stdin,NEW.command)); PERFORM pg_notify('q_event_'||NEW.clientid,CAST(data AS character varying)); RETURN NULL; END; $$ LANGUAGE plpgsql;