insert into dm_fragments(id,name,summary,categoryid,exitpoints) values(291,'XML - Set Attribute','Sets an attribute name in an XML element',9,1);
insert into dm_fragments(id,name,summary,categoryid,exitpoints) values(292,'XML - Set Text','Sets the text in an XML element',9,1);
insert into dm_fragments(id,name,summary,categoryid,exitpoints) values(293,'XML - Remove Element','Removes an XML element',9,1);
insert into dm_fragments(id,name,summary,categoryid,exitpoints) values(294,'XML - Add Element','Adds a New XML element',9,1);

insert into dm_fragmenttext(fragmentid,data,type) values(291,'modify(modifier: ''xml'', file: "@filename@", serverspecific: @server_specific@) {
  set_attribute(xpath: "@xpath@", value: "@attribute@"); }',0);
insert into dm_fragmenttext(fragmentid,data,type) values(292,'modify(modifier: ''xml'', file: "@filename@", serverspecific: @server_specific@) {
  set_text(xpath: "@xpath@", value: "@text@"); }',0);
insert into dm_fragmenttext(fragmentid,data,type) values(293,'modify(modifier: ''xml'', file: "@filename@", serverspecific: @server_specific@) {
  remove_element(xpath: "@xpath@"); }',0);
insert into dm_fragmenttext(fragmentid,data,type) values(294,'modify(modifier: ''xml'', file: "@filename@", serverspecific: @server_specific@) {
  add_element(xpath: "@xpath@", value: "@element@", pos: "@position@"); }',0);

insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29101,291,'entry','filename','N',1);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29102,291,'entry','xpath','N',2);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29103,291,'entry','attribute','N',3);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29104,291,'Checkbox','Server Specific','N',4);

insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29201,292,'entry','filename','N',1);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29202,292,'entry','xpath','N',2);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29203,292,'entry','text','N',3);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29204,292,'Checkbox','Server Specific','N',4);

insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29301,293,'entry','filename','N',1);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29302,293,'entry','xpath','N',2);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29303,293,'Checkbox','Server Specific','N',3);

insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29401,294,'entry','filename','N',1);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29402,294,'entry','xpath','N',2);
insert into dm_fragmentattrs(id,typeid,attype,atname,tablename,inherit,atorder) values(29403,294,'dropdown','position','dm_xmlpos','N',3);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29404,294,'entry','element','N',5);
insert into dm_fragmentattrs(id,typeid,attype,atname,inherit,atorder) values(29405,294,'Checkbox','Server Specific','N',5);


create table dm_xmlpos(
id		integer,
name 		character(10),
domainid	integer
);
insert into dm_xmlpos(id,name,domainid) values(1,'before',0);
insert into dm_xmlpos(id,name,domainid) values(2,'after',0);
insert into dm_xmlpos(id,name,domainid) values(3,'inside',0);
