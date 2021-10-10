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

-- create the function
INSERT INTO dm_action(id,name,filepath,summary,domainid,function,resultisexpr,
  graphical,ownerid,creatorid,modifierid,status,copy,kind,categoryid)
VALUES(15,'md5sum','md5sum','Calculates the MD5 sum of a file',1,'Y','N',
  'N',1,1,1,'N','N',4,1);

INSERT INTO dm_actionarg(actionid,name,type,outpos,required,pad,inpos)
VALUES(15,'filename','Entry',1,'Y','Y',1);

-- create the fragment to call the function
INSERT INTO dm_fragments(id,name,summary,categoryid,exitpoints) VALUES(15,'Calculate MD5','Calculates the MD5 of a file',1,1);
INSERT INTO dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) VALUES(1501,15,'entry','variable','N',1);
INSERT INTO dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) VALUES(1502,15,'entry','filename','N',1);
INSERT INTO dm_fragmenttext(fragmentid,data,type) VALUES(15,'set @variable@ = md5sum(filename: "@filename@");',0);