
--
-- TOC entry 165 (class 1259 OID 102266)
-- Dependencies: 9
-- Name: dm_actionaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_type (
    id integer NOT NULL,
    name character varying(256),
    database character(1),
    deletedir character(1)
) WITH (
  OIDS=FALSE
);
