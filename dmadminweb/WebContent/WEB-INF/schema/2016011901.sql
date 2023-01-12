CREATE TABLE dm.dm_action_categories (
    id integer NOT NULL,
    categoryid integer NOT NULL
);

CREATE TABLE dm.dm_component_categories (
    id integer NOT NULL,
    categoryid integer NOT NULL
);

insert into dm.dm_category(id,name) values(10,'General');

insert into dm.dm_action_categories(id,categoryid) values(12,9);

insert into dm.dm_action_categories(id,categoryid) values(104,4);

insert into dm.dm_action_categories(id,categoryid) values(105,20);

insert into dm.dm_action_categories(id,categoryid) values(1,6);

insert into dm.dm_action_categories(id,categoryid) values(9,8);

insert into dm.dm_action_categories(id,categoryid) values(10,8);

insert into dm.dm_action_categories select id, categoryid from dm.dm_action where categoryid is not NULL;

ALTER TABLE dm.dm_action_categories OWNER TO postgres;

delete from dm.dm_fragmentattrs where typeid = 150;

delete from dm.dm_fragmentattrs where typeid = 160;

delete from dm.dm_fragmentattrs where typeid = 170;

delete from dm.dm_fragmentattrs where typeid = 180;

delete from dm.dm_fragments where id = 150;

delete from dm.dm_fragments where id = 160;

delete from dm.dm_fragments where id = 170;

delete from dm.dm_fragments where id = 180;

insert into dm.dm_action_categories select id, 4 from dm.dm_action where id not in (12,104,105,1,9,10) and categoryid is NULL;

insert into dm.dm_fragment_categories select id, 4 from dm.dm_fragments where categoryid is NULL;

insert into dm.dm_component_categories select id, 10 from dm.dm_component;
