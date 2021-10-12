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

alter table users add(dob date);

update users set dob=to_date('1955-10-28','YYYY-MM-DD') where surname='Gates';
update users set dob=to_date('1944-08-17','YYYY-MM-DD') where surname='Ellison';
update users set dob=to_date('1954-11-13','YYYY-MM-DD') where surname='McNealy';
update users set dob=to_date('1969-12-28','YYYY-MM-DD') where surname='Torvalds';
update users set dob=to_date('1955-02-24','YYYY-MM-DD') where surname='Jobs';
