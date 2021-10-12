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

CREATE OR REPLACE FUNCTION dm.queue_event() RETURNS TRIGGER AS $$ DECLARE data json; notification json; BEGIN IF (TG_OP = 'DELETE') THEN data = row_to_json(OLD); ELSE data = row_to_json(NEW); END IF; notification = json_build_object('table',TG_TABLE_NAME,'action', TG_OP,'data', data);  PERFORM pg_notify('q_event',notification::text); RETURN NULL;  END; $$ LANGUAGE plpgsql;
