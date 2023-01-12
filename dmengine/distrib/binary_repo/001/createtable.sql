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

create table users (
	firstname varchar(40),
	surname varchar(40)
	);

insert into users(firstname,surname) values('Bill','Gates');
insert into users(firstname,surname) values('Larry','Ellison');
insert into users(firstname,surname) values('Scott','McNealy');
insert into users(firstname,surname) values('Linus','Torvalds');
insert into users(firstname,surname) values('Steve','Jobs');
