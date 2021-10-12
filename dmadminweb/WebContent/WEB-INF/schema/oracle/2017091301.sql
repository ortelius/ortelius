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

-- add zipget/zipadd/zipdel fragment wrappers around underlying DMScript functions
-- zipget
insert into dm.dm_fragments(id,name,summary,categoryid,exitpoints) values(111,'Extract File from ZIP','Extracts a named file from a ZIP/WAR/JAR/EAR file',9,1);
insert into dm.dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder,required) values(11101,111,'entry','zipfile','N',1,'Y');
insert into dm.dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder,required) values(11102,111,'entry','FileToExtract','N',2,'Y');
insert into dm.dm_fragmenttext(fragmentid,data,type) values(111,'zipget(zipfile: "@zipfile@", file: "@filetoextract@");',0);
insert into dm.dm_fragment_categories(id,categoryid) values(111,9);

-- zipadd
insert into dm.dm_fragments(id,name,summary,categoryid,exitpoints) values(112,'Add File to ZIP','Puts a dropzone file into a ZIP/WAR/JAR/EAR file',9,1);
insert into dm.dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder,required) values(11201,112,'entry','zipfile','N',1,'Y');
insert into dm.dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder,required) values(11202,112,'entry','FileToAdd','N',2,'Y');
insert into dm.dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder,required) values(11203,112,'checkbox','DeleteFromDropzone','N',3,'N');
insert into dm.dm_fragmenttext(fragmentid,data,type) values(112,'zipadd(zipfile: "@zipfile@", file: "@filetoadd@", delete: @deletefromdropzone@);',0);
insert into dm.dm_fragment_categories(id,categoryid) values(112,9);

-- zipdel
insert into dm.dm_fragments(id,name,summary,categoryid,exitpoints) values(113,'Remove File from ZIP','Deletes a file from a ZIP/WAR/JAR/EAR archive',9,1);
insert into dm.dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder,required) values(11301,113,'entry','zipfile','N',1,'Y');
insert into dm.dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder,required) values(11302,113,'entry','FileToDelete','N',2,'Y');
insert into dm.dm_fragmenttext(fragmentid,data,type) values(113,'zipdel(zipfile: "@zipfile@", file: "@filetodelete@");',0);
insert into dm.dm_fragment_categories(id,categoryid) values(113,9);

