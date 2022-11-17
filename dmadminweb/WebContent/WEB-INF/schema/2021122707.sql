CREATE TABLE IF NOT EXISTS dm_comp_provides (
    id integer NOT NULL,
    path character varying(1024) NOT NULL,
    verb character varying(40)
);

CREATE TABLE IF NOT EXISTS dm_comp_consumes (
    id integer NOT NULL,
    path character varying(1024) NOT NULL,
    verb character varying(40)
);
