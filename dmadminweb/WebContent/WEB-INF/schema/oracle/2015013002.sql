insert into dm.dm_category values (22,'File Logic');

update dm.dm_category set name = 'Flow Logic' where id = 1;
update dm.dm_fragments set categoryid = 22 where id = 130;
update dm.dm_fragments set categoryid = 22 where id = 15;
