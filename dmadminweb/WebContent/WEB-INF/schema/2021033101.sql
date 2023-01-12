CREATE TABLE IF NOT EXISTS dm_user_auth (
    id integer NOT NULL,
    jti character varying(40) NOT NULL,
    lastseen timestamp
);

ALTER TABLE dm.dm_user_auth OWNER TO postgres;
