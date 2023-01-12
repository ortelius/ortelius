CREATE TABLE dm.dm_fragment_categories (
    id integer NOT NULL,
    categoryid integer NOT NULL
);

insert into dm.dm_fragment_categories select id, categoryid from dm.dm_fragments;

ALTER TABLE dm.dm_fragment_categories OWNER TO postgres;
