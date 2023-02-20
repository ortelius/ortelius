SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;


CREATE SCHEMA dm;


ALTER SCHEMA dm OWNER TO postgres;

SET search_path = dm, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;


CREATE TABLE dm_action (
    id integer NOT NULL,
    name character varying(256),
    filepath character varying(2048),
    summary character varying(256),
    domainid integer,
    textid integer,
    repositoryid integer,
    function character(1),
    pluginid integer,
    resultisexpr character(1),
    remote character(1),
    graphical character(1),
    ownerid integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    ogrpid integer,
    status character(1) NOT NULL,
    copy character(1),
    kind integer NOT NULL,
    categoryid integer
);


ALTER TABLE dm.dm_action OWNER TO postgres;

--
-- TOC entry 165 (class 1259 OID 102266)
-- Dependencies: 9
-- Name: dm_actionaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_actionaccess (
    actionid integer NOT NULL,
    usrgrpid integer NOT NULL,
    readaccess character(1),
    writeaccess character(1),
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_actionaccess OWNER TO postgres;

--
-- TOC entry 166 (class 1259 OID 102269)
-- Dependencies: 9
-- Name: dm_actionarg; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_actionarg (
    actionid integer NOT NULL,
    name character varying(256) NOT NULL,
    type character varying(32),
    outpos integer,
    required character(1),
    switch character varying(256),
    pad character(1),
    inpos integer,
    switchmode character(1),
    negswitch character varying(64)
);


ALTER TABLE dm.dm_actionarg OWNER TO postgres;

--
-- TOC entry 167 (class 1259 OID 102275)
-- Dependencies: 9
-- Name: dm_actionedit; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_actionedit (
    actionid integer,
    editid integer,
    userid integer
);


ALTER TABLE dm.dm_actionedit OWNER TO postgres;

--
-- TOC entry 168 (class 1259 OID 102278)
-- Dependencies: 9
-- Name: dm_actionfilepath; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_actionfilepath (
    actionid integer NOT NULL,
    typeid integer NOT NULL,
    filepath character varying(2048) NOT NULL,
    copy character(1)
);


ALTER TABLE dm.dm_actionfilepath OWNER TO postgres;

--
-- TOC entry 169 (class 1259 OID 102284)
-- Dependencies: 9
-- Name: dm_actionflows; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_actionflows (
    actionid integer,
    flowid integer,
    nodefrom integer,
    nodeto integer,
    pos integer
);


ALTER TABLE dm.dm_actionflows OWNER TO postgres;

--
-- TOC entry 170 (class 1259 OID 102287)
-- Dependencies: 9
-- Name: dm_actionfragattrs; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_actionfragattrs (
    actionid integer,
    windowid integer,
    attrid integer,
    value character varying(2048)
);


ALTER TABLE dm.dm_actionfragattrs OWNER TO postgres;

--
-- TOC entry 171 (class 1259 OID 102293)
-- Dependencies: 9
-- Name: dm_actionfrags; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_actionfrags (
    actionid integer,
    windowid integer,
    xpos integer,
    ypos integer,
    typeid integer,
    title character varying(32),
    summary character varying(256),
    parentwindowid integer
);


ALTER TABLE dm.dm_actionfrags OWNER TO postgres;

--
-- TOC entry 172 (class 1259 OID 102296)
-- Dependencies: 9
-- Name: dm_actiontext; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_actiontext (
    id integer NOT NULL,
    data text
);


ALTER TABLE dm.dm_actiontext OWNER TO postgres;

--
-- TOC entry 173 (class 1259 OID 102302)
-- Dependencies: 9
-- Name: dm_application; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_application (
    id integer NOT NULL,
    name character varying(2048),
    domainid integer,
    ownerid integer,
    ogrpid integer,
    summary character varying(256),
    actionid integer,
    version character varying(40),
    parentid integer,
    preactionid integer,
    postactionid integer,
    predecessorid integer,
    creatorid integer,
    modifierid integer,
    created integer,
    modified integer,
    xpos integer,
    ypos integer,
    startx integer,
    status character(1) NOT NULL,
    branch character varying(128),
    successtemplateid integer,
    failuretemplateid integer,
    isrelease character(1)
);


ALTER TABLE dm.dm_application OWNER TO postgres;

--
-- TOC entry 2663 (class 0 OID 0)
-- Dependencies: 173
-- Name: TABLE dm_application; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_application IS 'Applications';


--
-- TOC entry 2664 (class 0 OID 0)
-- Dependencies: 173
-- Name: COLUMN dm_application.name; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_application.name IS 'Name of the Application';


--
-- TOC entry 2665 (class 0 OID 0)
-- Dependencies: 173
-- Name: COLUMN dm_application.version; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_application.version IS 'This is the version of the application (if appropriate). If this field is set then the parentversionid should also be set to point to the application';


--
-- TOC entry 2666 (class 0 OID 0)
-- Dependencies: 173
-- Name: COLUMN dm_application.parentid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_application.parentid IS 'If this is an application version then this column points to the actual application';


--
-- TOC entry 2667 (class 0 OID 0)
-- Dependencies: 173
-- Name: COLUMN dm_application.startx; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_application.startx IS 'X pos of Start Window on component view';


--
-- TOC entry 2668 (class 0 OID 0)
-- Dependencies: 173
-- Name: COLUMN dm_application.branch; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_application.branch IS 'Contains the branch name if this is the first version of a new branch';


--
-- TOC entry 174 (class 1259 OID 102308)
-- Dependencies: 9
-- Name: dm_applicationaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_applicationaccess (
    appid integer NOT NULL,
    usrgrpid integer NOT NULL,
    readaccess character(1),
    writeaccess character(1),
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_applicationaccess OWNER TO postgres;

--
-- TOC entry 175 (class 1259 OID 102311)
-- Dependencies: 9
-- Name: dm_applicationcomponent; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_applicationcomponent (
    appid integer NOT NULL,
    compid integer,
    childappid integer,
    xpos integer,
    ypos integer
);


ALTER TABLE dm.dm_applicationcomponent OWNER TO postgres;

--
-- TOC entry 176 (class 1259 OID 102314)
-- Dependencies: 9
-- Name: dm_applicationcomponentflows; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_applicationcomponentflows (
    appid integer,
    objfrom integer,
    objto integer
);


ALTER TABLE dm.dm_applicationcomponentflows OWNER TO postgres;

--
-- TOC entry 177 (class 1259 OID 102317)
-- Dependencies: 9
-- Name: dm_applicationvars; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_applicationvars (
    appid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    arrayid integer,
    nocase character(1)
);


ALTER TABLE dm.dm_applicationvars OWNER TO postgres;

--
-- TOC entry 2669 (class 0 OID 0)
-- Dependencies: 177
-- Name: TABLE dm_applicationvars; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_applicationvars IS 'Associated variables for dm_application';


--
-- TOC entry 2670 (class 0 OID 0)
-- Dependencies: 177
-- Name: COLUMN dm_applicationvars.appid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_applicationvars.appid IS 'id of the owning Application';


--
-- TOC entry 2671 (class 0 OID 0)
-- Dependencies: 177
-- Name: COLUMN dm_applicationvars.name; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_applicationvars.name IS 'Name of the variable';


--
-- TOC entry 2672 (class 0 OID 0)
-- Dependencies: 177
-- Name: COLUMN dm_applicationvars.value; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_applicationvars.value IS 'Value of the variable or null if the value is an array';


--
-- TOC entry 2673 (class 0 OID 0)
-- Dependencies: 177
-- Name: COLUMN dm_applicationvars.arrayid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_applicationvars.arrayid IS 'id of the array in ArrayValues or null if the value is simple text';


--
-- TOC entry 178 (class 1259 OID 102323)
-- Dependencies: 9
-- Name: dm_approval; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_approval (
    id integer NOT NULL,
    appid integer,
    "WHEN" integer,
    userid integer,
    approved character(1) NOT NULL,
    note character varying(2048),
    domainid integer
);


ALTER TABLE dm.dm_approval OWNER TO postgres;

--
-- TOC entry 179 (class 1259 OID 102329)
-- Dependencies: 9
-- Name: dm_appsallowedinenv; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_appsallowedinenv (
    envid integer NOT NULL,
    appid integer NOT NULL
);


ALTER TABLE dm.dm_appsallowedinenv OWNER TO postgres;

--
-- TOC entry 2674 (class 0 OID 0)
-- Dependencies: 179
-- Name: TABLE dm_appsallowedinenv; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_appsallowedinenv IS 'Join table for dm_application to dm_environment';


--
-- TOC entry 2675 (class 0 OID 0)
-- Dependencies: 179
-- Name: COLUMN dm_appsallowedinenv.envid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_appsallowedinenv.envid IS 'id of the Environment';


--
-- TOC entry 2676 (class 0 OID 0)
-- Dependencies: 179
-- Name: COLUMN dm_appsallowedinenv.appid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_appsallowedinenv.appid IS 'id of the Application';


--
-- TOC entry 180 (class 1259 OID 102332)
-- Dependencies: 9
-- Name: dm_appsinenv; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_appsinenv (
    envid integer NOT NULL,
    appid integer NOT NULL,
    deploymentid integer,
    modifierid integer,
    modified integer,
    note character varying(2048)
);


ALTER TABLE dm.dm_appsinenv OWNER TO postgres;

--
-- TOC entry 181 (class 1259 OID 102338)
-- Dependencies: 9
-- Name: dm_arrayvalues; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_arrayvalues (
    id integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048)
);


ALTER TABLE dm.dm_arrayvalues OWNER TO postgres;

--
-- TOC entry 2677 (class 0 OID 0)
-- Dependencies: 181
-- Name: TABLE dm_arrayvalues; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_arrayvalues IS 'Values for arrays in any of the dm_???vars tables';


--
-- TOC entry 2678 (class 0 OID 0)
-- Dependencies: 181
-- Name: COLUMN dm_arrayvalues.id; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_arrayvalues.id IS 'id of the Array';


--
-- TOC entry 2679 (class 0 OID 0)
-- Dependencies: 181
-- Name: COLUMN dm_arrayvalues.name; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_arrayvalues.name IS 'Name (or key) of the array element';


--
-- TOC entry 2680 (class 0 OID 0)
-- Dependencies: 181
-- Name: COLUMN dm_arrayvalues.value; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_arrayvalues.value IS 'Value of the array element';


--
-- TOC entry 182 (class 1259 OID 102344)
-- Dependencies: 9
-- Name: dm_availability; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_availability (
    envid integer,
    unavailstart integer,
    unavailend integer
);


ALTER TABLE dm.dm_availability OWNER TO postgres;

--
-- TOC entry 183 (class 1259 OID 102347)
-- Dependencies: 9
-- Name: dm_calendar; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_calendar (
    id integer NOT NULL,
    envid integer,
    eventname character varying(64),
    eventtype character(20),
    starttime integer,
    endtime integer,
    appid integer,
    description character varying(128),
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    allday character(1),
    status character(1)
);


ALTER TABLE dm.dm_calendar OWNER TO postgres;

--
-- TOC entry 184 (class 1259 OID 102350)
-- Dependencies: 9
-- Name: dm_category; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_category (
    id integer,
    name character varying(64)
);


ALTER TABLE dm.dm_category OWNER TO postgres;

--
-- TOC entry 185 (class 1259 OID 102353)
-- Dependencies: 9
-- Name: dm_compitemprops; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_compitemprops (
    compitemid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    encrypted character(1),
    overridable character(1),
    appendable character(1)
);


ALTER TABLE dm.dm_compitemprops OWNER TO postgres;

--
-- TOC entry 2681 (class 0 OID 0)
-- Dependencies: 185
-- Name: TABLE dm_compitemprops; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_compitemprops IS 'Associated properties for component items in dm_componentitem';


--
-- TOC entry 186 (class 1259 OID 102359)
-- Dependencies: 9
-- Name: dm_component; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_component (
    id integer NOT NULL,
    name character varying(2048),
    domainid integer,
    summary character varying(256),
    ownerid integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    parentid integer,
    predecessorid integer,
    xpos integer,
    ypos integer,
    ogrpid integer,
    preactionid integer,
    postactionid integer,
    actionid integer,
    status character(1) NOT NULL,
    rollup integer,
    rollback integer,
    filteritems character(1),
    deployalways character(1),
    basedir character varying(2048)
);


ALTER TABLE dm.dm_component OWNER TO postgres;

--
-- TOC entry 187 (class 1259 OID 102365)
-- Dependencies: 9
-- Name: dm_componentaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_componentaccess (
    compid integer NOT NULL,
    usrgrpid integer NOT NULL,
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_componentaccess OWNER TO postgres;

--
-- TOC entry 188 (class 1259 OID 102368)
-- Dependencies: 9
-- Name: dm_componentitem; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_componentitem (
    id integer NOT NULL,
    compid integer,
    repositoryid integer,
    target character varying(2048),
    name character varying(128),
    summary character varying(256),
    predecessorid integer,
    xpos integer,
    ypos integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    status character(1) NOT NULL,
    rollup integer,
    rollback integer
);


ALTER TABLE dm.dm_componentitem OWNER TO postgres;

--
-- TOC entry 189 (class 1259 OID 102374)
-- Dependencies: 9
-- Name: dm_componentvars; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_componentvars (
    compid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    arrayid integer,
    nocase character(1)
);


ALTER TABLE dm.dm_componentvars OWNER TO postgres;

--
-- TOC entry 2682 (class 0 OID 0)
-- Dependencies: 189
-- Name: TABLE dm_componentvars; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_componentvars IS 'Associated variables for dm_component';


--
-- TOC entry 190 (class 1259 OID 102380)
-- Dependencies: 9
-- Name: dm_compsallowedonserv; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_compsallowedonserv (
    compid integer NOT NULL,
    serverid integer NOT NULL
);


ALTER TABLE dm.dm_compsallowedonserv OWNER TO postgres;

--
-- TOC entry 191 (class 1259 OID 102383)
-- Dependencies: 9
-- Name: dm_compsonserv; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_compsonserv (
    compid integer NOT NULL,
    serverid integer NOT NULL,
    deploymentid integer,
    modifierid integer,
    modified integer,
    note character varying(2048)
);


ALTER TABLE dm.dm_compsonserv OWNER TO postgres;

--
-- TOC entry 192 (class 1259 OID 102389)
-- Dependencies: 9
-- Name: dm_credentials; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_credentials (
    id integer NOT NULL,
    kind integer,
    encusername character varying(2048),
    encpassword character varying(2048),
    filename character varying(2048),
    name character varying(2048),
    domainid integer,
    summary character varying(256),
    ownerid integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    status character(1) NOT NULL
);


ALTER TABLE dm.dm_credentials OWNER TO postgres;

--
-- TOC entry 193 (class 1259 OID 102395)
-- Dependencies: 9
-- Name: dm_credentialsaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_credentialsaccess (
    credid integer NOT NULL,
    usrgrpid integer NOT NULL,
    readaccess character(1),
    writeaccess character(1),
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_credentialsaccess OWNER TO postgres;

--
-- TOC entry 194 (class 1259 OID 102398)
-- Dependencies: 9
-- Name: dm_datasource; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_datasource (
    id integer NOT NULL,
    name character varying(2048),
    type character varying(256),
    domainid integer,
    ownerid integer,
    ogrpid integer,
    defid integer,
    summary character varying(256),
    credid integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    status character(1) NOT NULL
);


ALTER TABLE dm.dm_datasource OWNER TO postgres;

--
-- TOC entry 2683 (class 0 OID 0)
-- Dependencies: 194
-- Name: TABLE dm_datasource; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_datasource IS 'Datasources';


--
-- TOC entry 2684 (class 0 OID 0)
-- Dependencies: 194
-- Name: COLUMN dm_datasource.name; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_datasource.name IS 'Name of the Datasource';


--
-- TOC entry 2685 (class 0 OID 0)
-- Dependencies: 194
-- Name: COLUMN dm_datasource.type; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_datasource.type IS 'Type of the Datasource (matches a provider)';


--
-- TOC entry 195 (class 1259 OID 102404)
-- Dependencies: 9
-- Name: dm_datasourceaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_datasourceaccess (
    datasourceid integer NOT NULL,
    usrgrpid integer NOT NULL,
    readaccess character(1),
    writeaccess character(1),
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_datasourceaccess OWNER TO postgres;

--
-- TOC entry 196 (class 1259 OID 102407)
-- Dependencies: 9
-- Name: dm_datasourceprops; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_datasourceprops (
    datasourceid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    encrypted character(1),
    overridable character(1),
    appendable character(1)
);


ALTER TABLE dm.dm_datasourceprops OWNER TO postgres;

--
-- TOC entry 2686 (class 0 OID 0)
-- Dependencies: 196
-- Name: TABLE dm_datasourceprops; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_datasourceprops IS 'Associated properties for dm_datasouce';


--
-- TOC entry 2687 (class 0 OID 0)
-- Dependencies: 196
-- Name: COLUMN dm_datasourceprops.datasourceid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_datasourceprops.datasourceid IS 'id of the owning Datasource';


--
-- TOC entry 2688 (class 0 OID 0)
-- Dependencies: 196
-- Name: COLUMN dm_datasourceprops.name; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_datasourceprops.name IS 'Name of the property';


--
-- TOC entry 2689 (class 0 OID 0)
-- Dependencies: 196
-- Name: COLUMN dm_datasourceprops.value; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_datasourceprops.value IS 'Value of the property';


--
-- TOC entry 197 (class 1259 OID 102413)
-- Dependencies: 9
-- Name: dm_datasourcevars; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_datasourcevars (
    datasourceid integer NOT NULL,
    name character varying(64) NOT NULL,
    value character varying(2048),
    arrayid integer,
    nocase character(1)
);


ALTER TABLE dm.dm_datasourcevars OWNER TO postgres;

--
-- TOC entry 198 (class 1259 OID 102419)
-- Dependencies: 9
-- Name: dm_deployment; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_deployment (
    deploymentid integer NOT NULL,
    userid integer,
    startts timestamp with time zone,
    finishts timestamp with time zone,
    exitcode integer,
    exitstatus character varying(256),
    appid integer,
    envid integer,
    started integer,
    finished integer,
    sessionid character varying(64),
    eventid integer
);


ALTER TABLE dm.dm_deployment OWNER TO postgres;

--
-- TOC entry 199 (class 1259 OID 102422)
-- Dependencies: 9
-- Name: dm_deploymentaction; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_deploymentaction (
    deploymentid integer,
    actionid integer,
    runtime timestamp with time zone,
    changed character(1),
    checksum character varying(256)
);


ALTER TABLE dm.dm_deploymentaction OWNER TO postgres;

--
-- TOC entry 200 (class 1259 OID 102425)
-- Dependencies: 9
-- Name: dm_deploymentlog; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_deploymentlog (
    deploymentid integer,
    runtime timestamp with time zone,
    stream integer,
    line text,
    thread integer,
    lineno integer
);


ALTER TABLE dm.dm_deploymentlog OWNER TO postgres;

--
-- TOC entry 201 (class 1259 OID 102431)
-- Dependencies: 9
-- Name: dm_deploymentprops; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_deploymentprops (
    deploymentid integer,
    stepid integer,
    instanceid integer,
    name character varying(64),
    value character varying(2048)
);


ALTER TABLE dm.dm_deploymentprops OWNER TO postgres;

--
-- TOC entry 202 (class 1259 OID 102437)
-- Dependencies: 9
-- Name: dm_deploymentscript; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_deploymentscript (
    deploymentid integer NOT NULL,
    stepid integer NOT NULL,
    serverid integer NOT NULL,
    servername character varying(64) NOT NULL,
    serverhostname character varying(256),
    serverprotocol character varying(64),
    cmd character varying(2048),
    exitcode integer
);


ALTER TABLE dm.dm_deploymentscript OWNER TO postgres;

--
-- TOC entry 203 (class 1259 OID 102443)
-- Dependencies: 9
-- Name: dm_deploymentstep; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_deploymentstep (
    deploymentid integer NOT NULL,
    stepid integer NOT NULL,
    type character varying(64),
    startts timestamp with time zone,
    finishts timestamp with time zone,
    started integer,
    finished integer
);


ALTER TABLE dm.dm_deploymentstep OWNER TO postgres;

--
-- TOC entry 204 (class 1259 OID 102446)
-- Dependencies: 9
-- Name: dm_deploymentxfer; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_deploymentxfer (
    deploymentid integer NOT NULL,
    stepid integer NOT NULL,
    repoid integer,
    reponame character varying(64),
    repoinstanceid integer,
    repopath character varying(2048),
    repover character varying(32),
    componentid integer,
    componentname character varying(64),
    componentitemid integer,
    serverid integer NOT NULL,
    servername character varying(64) NOT NULL,
    targetfilename character varying(2048),
    serverhostname character varying(256),
    serverprotocol character varying(64),
    checksum1 character varying(256),
    checksum2 character varying(256),
    created character(1),
    modified character(1),
    renamed character(1),
    size1 integer,
    size2 integer,
    istext character(1)
);


ALTER TABLE dm.dm_deploymentxfer OWNER TO postgres;

--
-- TOC entry 205 (class 1259 OID 102452)
-- Dependencies: 9
-- Name: dm_domain; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_domain (
    id integer NOT NULL,
    name character varying(2048),
    domainid integer,
    ownerid integer,
    ogrpid integer,
    summary character varying(256),
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    status character(1) NOT NULL,
    lifecycle character(1),
    "position" integer
);


ALTER TABLE dm.dm_domain OWNER TO postgres;

--
-- TOC entry 206 (class 1259 OID 102458)
-- Dependencies: 9
-- Name: dm_domainaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_domainaccess (
    domainid integer NOT NULL,
    usrgrpid integer NOT NULL,
    readaccess character(1),
    writeaccess character(1),
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_domainaccess OWNER TO postgres;

--
-- TOC entry 207 (class 1259 OID 102461)
-- Dependencies: 9
-- Name: dm_domaininherit; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_domaininherit (
    domainid integer NOT NULL,
    usrgrpid integer NOT NULL,
    readaccess character(1),
    writeaccess character(1),
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_domaininherit OWNER TO postgres;

--
-- TOC entry 208 (class 1259 OID 102464)
-- Dependencies: 9
-- Name: dm_engine; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_engine (
    id integer NOT NULL,
    name character varying(64),
    hostname character varying(256),
    domainid integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    status character(1) NOT NULL
);


ALTER TABLE dm.dm_engine OWNER TO postgres;

--
-- TOC entry 209 (class 1259 OID 102467)
-- Dependencies: 9
-- Name: dm_engineaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_engineaccess (
    engineid integer NOT NULL,
    usrgrpid integer NOT NULL,
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_engineaccess OWNER TO postgres;

--
-- TOC entry 210 (class 1259 OID 102470)
-- Dependencies: 9
-- Name: dm_engineconfig; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_engineconfig (
    engineid integer NOT NULL,
    name character varying(64) NOT NULL,
    value character varying(2048)
);


ALTER TABLE dm.dm_engineconfig OWNER TO postgres;

--
-- TOC entry 211 (class 1259 OID 102476)
-- Dependencies: 9
-- Name: dm_environment; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_environment (
    id integer NOT NULL,
    name character varying(2048),
    ownerid integer,
    ogrpid integer,
    domainid integer,
    credid integer,
    summary character varying(256),
    notes character varying(2048),
    basedir character varying(1024),
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    calusage character(1),
    calstart integer,
    calend integer,
    status character(1) NOT NULL
);


ALTER TABLE dm.dm_environment OWNER TO postgres;

--
-- TOC entry 2690 (class 0 OID 0)
-- Dependencies: 211
-- Name: TABLE dm_environment; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_environment IS 'Environments (aka logical servers)';


--
-- TOC entry 2691 (class 0 OID 0)
-- Dependencies: 211
-- Name: COLUMN dm_environment.name; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_environment.name IS 'Name of the Environment';


--
-- TOC entry 2692 (class 0 OID 0)
-- Dependencies: 211
-- Name: COLUMN dm_environment.ownerid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_environment.ownerid IS 'id of the owning User or null if owned by Usergroup';


--
-- TOC entry 2693 (class 0 OID 0)
-- Dependencies: 211
-- Name: COLUMN dm_environment.ogrpid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_environment.ogrpid IS 'id of the owning Usergroup or null if owned by User';


--
-- TOC entry 212 (class 1259 OID 102482)
-- Dependencies: 9
-- Name: dm_environmentaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_environmentaccess (
    envid integer NOT NULL,
    usrgrpid integer NOT NULL,
    writeaccess character(1),
    readaccess character(1),
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_environmentaccess OWNER TO postgres;

--
-- TOC entry 213 (class 1259 OID 102485)
-- Dependencies: 9
-- Name: dm_environmentvars; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_environmentvars (
    envid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    arrayid integer,
    nocase character(1)
);


ALTER TABLE dm.dm_environmentvars OWNER TO postgres;

--
-- TOC entry 2694 (class 0 OID 0)
-- Dependencies: 213
-- Name: TABLE dm_environmentvars; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_environmentvars IS 'Associated variables for dm_environment';


--
-- TOC entry 2695 (class 0 OID 0)
-- Dependencies: 213
-- Name: COLUMN dm_environmentvars.envid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_environmentvars.envid IS 'id of the owning Environment';


--
-- TOC entry 2696 (class 0 OID 0)
-- Dependencies: 213
-- Name: COLUMN dm_environmentvars.name; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_environmentvars.name IS 'Name of the variable';


--
-- TOC entry 2697 (class 0 OID 0)
-- Dependencies: 213
-- Name: COLUMN dm_environmentvars.value; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_environmentvars.value IS 'Value of the variable or null if the value is an array';


--
-- TOC entry 2698 (class 0 OID 0)
-- Dependencies: 213
-- Name: COLUMN dm_environmentvars.arrayid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_environmentvars.arrayid IS 'id of the array in ArrayValues or null if the value is simple text';


--
-- TOC entry 214 (class 1259 OID 102491)
-- Dependencies: 9
-- Name: dm_field; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_field (
    id integer NOT NULL,
    name character varying(256),
    type integer,
    querystring character varying(2048),
    summary character varying(256),
    domainid integer,
    actionid integer,
    datasourceid integer
);


ALTER TABLE dm.dm_field OWNER TO postgres;

--
-- TOC entry 215 (class 1259 OID 102497)
-- Dependencies: 9
-- Name: dm_fieldvalues; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_fieldvalues (
    fieldid integer NOT NULL,
    positionid integer NOT NULL,
    fieldvalue character varying(128)
);


ALTER TABLE dm.dm_fieldvalues OWNER TO postgres;

--
-- TOC entry 216 (class 1259 OID 102500)
-- Dependencies: 9
-- Name: dm_fragmentattrs; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_fragmentattrs (
    id integer NOT NULL,
    typeid integer,
    attype character varying(64),
    atname character varying(64),
    tablename character varying(64),
    inherit character(1),
    atorder integer
);


ALTER TABLE dm.dm_fragmentattrs OWNER TO postgres;

--
-- TOC entry 217 (class 1259 OID 102503)
-- Dependencies: 9
-- Name: dm_fragments; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_fragments (
    id integer,
    name character varying(64),
    summary character varying(2048),
    categoryid integer,
    exitpoints integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    drilldown character(1),
    actionid integer
);


ALTER TABLE dm.dm_fragments OWNER TO postgres;

--
-- TOC entry 218 (class 1259 OID 102509)
-- Dependencies: 9
-- Name: dm_fragmenttext; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_fragmenttext (
    fragmentid integer NOT NULL,
    data text,
    type integer NOT NULL
);


ALTER TABLE dm.dm_fragmenttext OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 102515)
-- Dependencies: 9
-- Name: dm_historyattachment; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_historyattachment (
    objid integer NOT NULL,
    kind integer NOT NULL,
    id integer NOT NULL,
    filename character varying(64) NOT NULL,
    size integer
);


ALTER TABLE dm.dm_historyattachment OWNER TO postgres;

--
-- TOC entry 220 (class 1259 OID 102518)
-- Dependencies: 9
-- Name: dm_historycomment; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_historycomment (
    id integer NOT NULL,
    kind integer NOT NULL,
    note character varying(2048) NOT NULL,
    userid integer NOT NULL,
    "WHEN" integer
);


ALTER TABLE dm.dm_historycomment OWNER TO postgres;

--
-- TOC entry 221 (class 1259 OID 102524)
-- Dependencies: 9
-- Name: dm_historynote; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_historynote (
    objid integer NOT NULL,
    kind integer NOT NULL,
    "WHEN" integer NOT NULL,
    note character varying(2048),
    userid integer,
    icon character varying(64),
    id integer NOT NULL
);


ALTER TABLE dm.dm_historynote OWNER TO postgres;

--
-- TOC entry 222 (class 1259 OID 102530)
-- Dependencies: 9
-- Name: dm_historysubs; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_historysubs (
    id integer NOT NULL,
    kind integer NOT NULL,
    userid integer NOT NULL
);


ALTER TABLE dm.dm_historysubs OWNER TO postgres;

--
-- TOC entry 223 (class 1259 OID 102533)
-- Dependencies: 9
-- Name: dm_keys; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_keys (
    object character varying(64),
    id integer
);


ALTER TABLE dm.dm_keys OWNER TO postgres;

--
-- TOC entry 224 (class 1259 OID 102536)
-- Dependencies: 9
-- Name: dm_notify; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_notify (
    id integer NOT NULL,
    name character varying(2048),
    type character varying(256),
    domainid integer,
    ownerid integer,
    ogrpid integer,
    defid integer,
    summary character varying(256),
    credid integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    status character(1) NOT NULL
);


ALTER TABLE dm.dm_notify OWNER TO postgres;

--
-- TOC entry 2699 (class 0 OID 0)
-- Dependencies: 224
-- Name: TABLE dm_notify; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_notify IS 'Notifiers';


--
-- TOC entry 2700 (class 0 OID 0)
-- Dependencies: 224
-- Name: COLUMN dm_notify.name; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_notify.name IS 'Name of the Notifier';


--
-- TOC entry 2701 (class 0 OID 0)
-- Dependencies: 224
-- Name: COLUMN dm_notify.type; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_notify.type IS 'Type of the Notifier (matches a provider)';


--
-- TOC entry 225 (class 1259 OID 102542)
-- Dependencies: 9
-- Name: dm_notifyaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_notifyaccess (
    notifyid integer NOT NULL,
    usrgrpid integer NOT NULL,
    readaccess character(1),
    writeaccess character(1),
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_notifyaccess OWNER TO postgres;

--
-- TOC entry 226 (class 1259 OID 102545)
-- Dependencies: 9
-- Name: dm_notifyprops; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_notifyprops (
    notifyid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    encrypted character(1),
    overridable character(1),
    appendable character(1)
);


ALTER TABLE dm.dm_notifyprops OWNER TO postgres;

--
-- TOC entry 2702 (class 0 OID 0)
-- Dependencies: 226
-- Name: TABLE dm_notifyprops; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_notifyprops IS 'Associated properties for dm_notify';


--
-- TOC entry 2703 (class 0 OID 0)
-- Dependencies: 226
-- Name: COLUMN dm_notifyprops.notifyid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_notifyprops.notifyid IS 'id of the owning Notifier';


--
-- TOC entry 2704 (class 0 OID 0)
-- Dependencies: 226
-- Name: COLUMN dm_notifyprops.name; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_notifyprops.name IS 'Name of the property';


--
-- TOC entry 2705 (class 0 OID 0)
-- Dependencies: 226
-- Name: COLUMN dm_notifyprops.value; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_notifyprops.value IS 'Value of the property';


--
-- TOC entry 227 (class 1259 OID 102551)
-- Dependencies: 9
-- Name: dm_notifytext; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_notifytext (
    id integer NOT NULL,
    name character varying(64) NOT NULL,
    summary character varying(256),
    data text
);


ALTER TABLE dm.dm_notifytext OWNER TO postgres;

--
-- TOC entry 228 (class 1259 OID 102557)
-- Dependencies: 9
-- Name: dm_notifyvars; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_notifyvars (
    notifyid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    nocase character(1),
    arrayid integer
);


ALTER TABLE dm.dm_notifyvars OWNER TO postgres;

--
-- TOC entry 229 (class 1259 OID 102563)
-- Dependencies: 9
-- Name: dm_plugin; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_plugin (
    id integer NOT NULL,
    version integer,
    library character varying(256)
);


ALTER TABLE dm.dm_plugin OWNER TO postgres;

--
-- TOC entry 230 (class 1259 OID 102566)
-- Dependencies: 9
-- Name: dm_prerequisities; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_prerequisities (
    appid integer,
    deptype character varying(4),
    compid integer,
    depappid integer,
    option character varying(16),
    notes character varying(256)
);


ALTER TABLE dm.dm_prerequisities OWNER TO postgres;

--
-- TOC entry 231 (class 1259 OID 102569)
-- Dependencies: 9
-- Name: dm_privileges; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_privileges (
    groupid integer NOT NULL,
    cobjtype integer NOT NULL
);


ALTER TABLE dm.dm_privileges OWNER TO postgres;

--
-- TOC entry 232 (class 1259 OID 102572)
-- Dependencies: 9
-- Name: dm_propertydef; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_propertydef (
    defid integer NOT NULL,
    name character varying(256) NOT NULL,
    required character(1),
    appendable character(1)
);


ALTER TABLE dm.dm_propertydef OWNER TO postgres;

--
-- TOC entry 233 (class 1259 OID 102575)
-- Dependencies: 9
-- Name: dm_providerdef; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_providerdef (
    id integer NOT NULL,
    name character varying(256),
    kind integer,
    kindstr character varying(256),
    pluginid integer
);


ALTER TABLE dm.dm_providerdef OWNER TO postgres;

--
-- TOC entry 234 (class 1259 OID 102581)
-- Dependencies: 9
-- Name: dm_providerdefprops; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_providerdefprops (
    defid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    encrypted character varying(1),
    overridable character(1),
    appendable character(1),
    engineid integer
);


ALTER TABLE dm.dm_providerdefprops OWNER TO postgres;

--
-- TOC entry 235 (class 1259 OID 102587)
-- Dependencies: 9
-- Name: dm_repository; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_repository (
    id integer NOT NULL,
    name character varying(2048),
    type character varying(256),
    domainid integer,
    ownerid integer,
    ogrpid integer,
    defid integer,
    summary character varying(256),
    credid integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    status character(1) NOT NULL
);


ALTER TABLE dm.dm_repository OWNER TO postgres;

--
-- TOC entry 2706 (class 0 OID 0)
-- Dependencies: 235
-- Name: TABLE dm_repository; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_repository IS 'Sources';


--
-- TOC entry 236 (class 1259 OID 102593)
-- Dependencies: 9
-- Name: dm_repositoryaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_repositoryaccess (
    repositoryid integer NOT NULL,
    usrgrpid integer NOT NULL,
    readaccess character(1),
    writeaccess character(1),
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_repositoryaccess OWNER TO postgres;

--
-- TOC entry 237 (class 1259 OID 102596)
-- Dependencies: 9
-- Name: dm_repositoryignore; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_repositoryignore (
    defid integer NOT NULL,
    pattern character varying(256) NOT NULL,
    isdir character(1)
);


ALTER TABLE dm.dm_repositoryignore OWNER TO postgres;

--
-- TOC entry 238 (class 1259 OID 102599)
-- Dependencies: 9
-- Name: dm_repositoryprops; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_repositoryprops (
    repositoryid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    encrypted character(1),
    overridable character(1),
    appendable character(1)
);


ALTER TABLE dm.dm_repositoryprops OWNER TO postgres;

--
-- TOC entry 2707 (class 0 OID 0)
-- Dependencies: 238
-- Name: TABLE dm_repositoryprops; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_repositoryprops IS 'Associated properties for sources in dm_source';


--
-- TOC entry 239 (class 1259 OID 102605)
-- Dependencies: 9
-- Name: dm_repositorytextpattern; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_repositorytextpattern (
    repositoryid integer NOT NULL,
    path character varying(2048) NOT NULL,
    pattern character varying(256) NOT NULL,
    istext character(1)
);


ALTER TABLE dm.dm_repositorytextpattern OWNER TO postgres;

--
-- TOC entry 240 (class 1259 OID 102611)
-- Dependencies: 9
-- Name: dm_repositoryvars; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_repositoryvars (
    repositoryid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    arrayid integer,
    nocase character(1)
);


ALTER TABLE dm.dm_repositoryvars OWNER TO postgres;

--
-- TOC entry 2708 (class 0 OID 0)
-- Dependencies: 240
-- Name: TABLE dm_repositoryvars; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_repositoryvars IS 'Associated variables for dm_source';


--
-- TOC entry 241 (class 1259 OID 102617)
-- Dependencies: 9
-- Name: dm_request; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_request (
    id integer NOT NULL,
    taskid integer,
    userid integer,
    "WHEN" integer,
    note character varying(2048),
    calendarid integer,
    appid integer,
    status character(1),
    completed integer,
    completedby integer
);


ALTER TABLE dm.dm_request OWNER TO postgres;

--
-- TOC entry 242 (class 1259 OID 102623)
-- Dependencies: 9
-- Name: dm_sequence; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_sequence (
    seqname character varying(20) NOT NULL,
    nextval integer
);


ALTER TABLE dm.dm_sequence OWNER TO postgres;

--
-- TOC entry 243 (class 1259 OID 102626)
-- Dependencies: 9
-- Name: dm_server; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_server (
    id integer NOT NULL,
    name character varying(2048),
    hostname character varying(2048),
    ownerid integer,
    protocol character varying(256),
    ogrpid integer,
    credid integer,
    summary character varying(256),
    notes character varying(2048),
    domainid integer,
    basedir character varying(1024),
    typeid integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    status character(1) NOT NULL
);


ALTER TABLE dm.dm_server OWNER TO postgres;

--
-- TOC entry 2709 (class 0 OID 0)
-- Dependencies: 243
-- Name: TABLE dm_server; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_server IS 'Servers (aka physical servers)';


--
-- TOC entry 2710 (class 0 OID 0)
-- Dependencies: 243
-- Name: COLUMN dm_server.name; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_server.name IS 'Name of the server';


--
-- TOC entry 2711 (class 0 OID 0)
-- Dependencies: 243
-- Name: COLUMN dm_server.hostname; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_server.hostname IS 'Hostname of the Server (if not the same as Name)';


--
-- TOC entry 2712 (class 0 OID 0)
-- Dependencies: 243
-- Name: COLUMN dm_server.ownerid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_server.ownerid IS 'id of the owning User or null if owned by a UserGroup';


--
-- TOC entry 2713 (class 0 OID 0)
-- Dependencies: 243
-- Name: COLUMN dm_server.protocol; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_server.protocol IS 'Protocol to use (matches a provider)';


--
-- TOC entry 2714 (class 0 OID 0)
-- Dependencies: 243
-- Name: COLUMN dm_server.ogrpid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_server.ogrpid IS 'id of the owning UserGroup or null if owned by a User';


--
-- TOC entry 244 (class 1259 OID 102632)
-- Dependencies: 9
-- Name: dm_server_connections; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_server_connections (
    envid integer,
    serverfrom integer,
    serverfromedge integer,
    serverto integer,
    servertoedge integer,
    label character varying(64),
    style character varying(64)
);


ALTER TABLE dm.dm_server_connections OWNER TO postgres;

--
-- TOC entry 245 (class 1259 OID 102635)
-- Dependencies: 9
-- Name: dm_serveraccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_serveraccess (
    serverid integer NOT NULL,
    usrgrpid integer NOT NULL,
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_serveraccess OWNER TO postgres;

--
-- TOC entry 246 (class 1259 OID 102638)
-- Dependencies: 9
-- Name: dm_serversinenv; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_serversinenv (
    envid integer NOT NULL,
    serverid integer NOT NULL,
    xpos integer,
    ypos integer
);


ALTER TABLE dm.dm_serversinenv OWNER TO postgres;

--
-- TOC entry 2715 (class 0 OID 0)
-- Dependencies: 246
-- Name: TABLE dm_serversinenv; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_serversinenv IS 'Join table for dm_server to dm_environment';


--
-- TOC entry 2716 (class 0 OID 0)
-- Dependencies: 246
-- Name: COLUMN dm_serversinenv.envid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_serversinenv.envid IS 'id of the Environment';


--
-- TOC entry 2717 (class 0 OID 0)
-- Dependencies: 246
-- Name: COLUMN dm_serversinenv.serverid; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON COLUMN dm_serversinenv.serverid IS 'id of the Server';


--
-- TOC entry 247 (class 1259 OID 102641)
-- Dependencies: 9
-- Name: dm_servertype; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_servertype (
    id integer NOT NULL,
    name character varying(256),
    lineends integer,
    pathformat character varying(256),
    scriptext character varying(10)
);


ALTER TABLE dm.dm_servertype OWNER TO postgres;

--
-- TOC entry 248 (class 1259 OID 102647)
-- Dependencies: 9
-- Name: dm_servervars; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_servervars (
    serverid integer NOT NULL,
    name character varying(256) NOT NULL,
    value character varying(2048),
    arrayid integer,
    nocase character(1)
);


ALTER TABLE dm.dm_servervars OWNER TO postgres;

--
-- TOC entry 2718 (class 0 OID 0)
-- Dependencies: 248
-- Name: TABLE dm_servervars; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_servervars IS 'Associated variables for dm_server';


--
-- TOC entry 249 (class 1259 OID 102653)
-- Dependencies: 9
-- Name: dm_tableinfo; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_tableinfo (
    schemaver integer NOT NULL
);


ALTER TABLE dm.dm_tableinfo OWNER TO postgres;

--
-- TOC entry 250 (class 1259 OID 102656)
-- Dependencies: 9
-- Name: dm_task; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_task (
    id integer NOT NULL,
    name character varying(256),
    typeid integer,
    domainid integer,
    preactionid integer,
    postactionid integer,
    ownerid integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    logoutput character(1),
    successtemplateid integer,
    failuretemplateid integer
);


ALTER TABLE dm.dm_task OWNER TO postgres;

--
-- TOC entry 251 (class 1259 OID 102659)
-- Dependencies: 9
-- Name: dm_taskaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_taskaccess (
    taskid integer,
    usrgrpid integer,
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_taskaccess OWNER TO postgres;

--
-- TOC entry 252 (class 1259 OID 102662)
-- Dependencies: 9
-- Name: dm_taskapprove; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_taskapprove (
    id integer,
    approvaldomain integer
);


ALTER TABLE dm.dm_taskapprove OWNER TO postgres;

--
-- TOC entry 253 (class 1259 OID 102665)
-- Dependencies: 9
-- Name: dm_taskcopy; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_taskcopy (
    id integer,
    targetdomain integer
);


ALTER TABLE dm.dm_taskcopy OWNER TO postgres;

--
-- TOC entry 254 (class 1259 OID 102668)
-- Dependencies: 9
-- Name: dm_taskcreateversion; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_taskcreateversion (
    id integer NOT NULL,
    targetdomain integer
);


ALTER TABLE dm.dm_taskcreateversion OWNER TO postgres;

--
-- TOC entry 255 (class 1259 OID 102671)
-- Dependencies: 9
-- Name: dm_taskmove; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_taskmove (
    id integer,
    targetdomain integer
);


ALTER TABLE dm.dm_taskmove OWNER TO postgres;

--
-- TOC entry 256 (class 1259 OID 102674)
-- Dependencies: 9
-- Name: dm_taskrequest; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_taskrequest (
    id integer,
    linkedtaskid integer
);


ALTER TABLE dm.dm_taskrequest OWNER TO postgres;

--
-- TOC entry 257 (class 1259 OID 102677)
-- Dependencies: 9
-- Name: dm_tasktypes; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_tasktypes (
    id integer,
    name character varying(256),
    domain character(1),
    environment character(1),
    application character(1),
    appversion character(1)
);


ALTER TABLE dm.dm_tasktypes OWNER TO postgres;

--
-- TOC entry 258 (class 1259 OID 102680)
-- Dependencies: 9
-- Name: dm_template; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_template (
    id integer,
    name character varying(128),
    summary character varying(256),
    notifierid integer,
    creatorid integer,
    modifierid integer,
    created integer,
    modified integer,
    status character(1),
    subject character varying(256),
    body character varying(2048)
);


ALTER TABLE dm.dm_template OWNER TO postgres;

--
-- TOC entry 259 (class 1259 OID 102686)
-- Dependencies: 9
-- Name: dm_templaterecipients; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_templaterecipients (
    templateid integer,
    usrgrpid integer,
    userid integer,
    ownertype integer
);


ALTER TABLE dm.dm_templaterecipients OWNER TO postgres;

--
-- TOC entry 260 (class 1259 OID 102689)
-- Dependencies: 9
-- Name: dm_user; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_user (
    id integer NOT NULL,
    name character varying(256),
    email character varying(2048),
    passhash character varying(2048),
    domainid integer,
    realname character varying(128),
    phone character varying(32),
    lastlogin timestamp with time zone,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    locked character(1),
    forcechange character(1),
    status character(1) NOT NULL
);


ALTER TABLE dm.dm_user OWNER TO postgres;

--
-- TOC entry 2719 (class 0 OID 0)
-- Dependencies: 260
-- Name: TABLE dm_user; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_user IS 'Users';


--
-- TOC entry 261 (class 1259 OID 102695)
-- Dependencies: 9
-- Name: dm_useraccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_useraccess (
    userid integer NOT NULL,
    usrgrpid integer NOT NULL,
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_useraccess OWNER TO postgres;

--
-- TOC entry 262 (class 1259 OID 102698)
-- Dependencies: 9
-- Name: dm_usergroup; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_usergroup (
    id integer NOT NULL,
    name character varying(256),
    email character varying(2048),
    domainid integer,
    summary character varying(256),
    ownerid integer,
    creatorid integer,
    created integer,
    modifierid integer,
    modified integer,
    status character(1) NOT NULL,
    ogrpid integer,
    acloverride character(1),
    tabendpoints character(1),
    tabapplications character(1),
    tabproviders character(1),
    tabusers character(1),
    tabactions character(1)
);


ALTER TABLE dm.dm_usergroup OWNER TO postgres;

--
-- TOC entry 2720 (class 0 OID 0)
-- Dependencies: 262
-- Name: TABLE dm_usergroup; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_usergroup IS 'User Groups';


--
-- TOC entry 263 (class 1259 OID 102704)
-- Dependencies: 9
-- Name: dm_usergroupaccess; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_usergroupaccess (
    usergroupid integer NOT NULL,
    usrgrpid integer NOT NULL,
    viewaccess character(1),
    updateaccess character(1)
);


ALTER TABLE dm.dm_usergroupaccess OWNER TO postgres;

--
-- TOC entry 264 (class 1259 OID 102707)
-- Dependencies: 9
-- Name: dm_usersingroup; Type: TABLE; Schema: dm; Owner: postgres; Tablespace:
--

CREATE TABLE dm_usersingroup (
    userid integer NOT NULL,
    groupid integer NOT NULL
);


ALTER TABLE dm.dm_usersingroup OWNER TO postgres;

--
-- TOC entry 2721 (class 0 OID 0)
-- Dependencies: 264
-- Name: TABLE dm_usersingroup; Type: COMMENT; Schema: dm; Owner: postgres
--

COMMENT ON TABLE dm_usersingroup IS 'Join table for dm_usergroup to dm_user';


--
-- TOC entry 2560 (class 0 OID 102260)
-- Dependencies: 164
-- Data for Name: dm_action; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_action VALUES (1, 'DeployWS', '${DMHOME}\scripts\deployws.${EXT}', NULL, 1, NULL, NULL, 'N', NULL, NULL, NULL, 'N', 1, 1, NULL, 1, NULL, NULL, 'N', 'Y', 4, 5);
INSERT INTO dm_action VALUES (2, 'dir', '${DMHOME}\scripts\dir.${EXT}', NULL, 1, NULL, NULL, 'Y', NULL, 'Y', NULL, 'N', 1, 1, NULL, 1, NULL, NULL, 'N', 'Y', 4, 1);
INSERT INTO dm_action VALUES (3, 'servicectl', '${DMHOME}\scripts\servicectl.vbs', NULL, 1, NULL, NULL, 'N', NULL, 'N', NULL, 'N', 1, 1, NULL, 1, NULL, NULL, 'N', 'Y', 4, 4);
INSERT INTO dm_action VALUES (4, 'listservices', '${DMHOME}\scripts\services.vbs', NULL, 1, NULL, NULL, 'Y', NULL, 'Y', NULL, 'N', 1, 1, NULL, 1, NULL, NULL, 'N', 'Y', 4, 4);
INSERT INTO dm_action VALUES (5, 'StopServer', '${DMHOME}\scripts\deployws.${EXT}', NULL, 1, NULL, NULL, 'N', NULL, NULL, NULL, 'N', 1, 1, NULL, 1, NULL, NULL, 'N', 'Y', 4, 5);
INSERT INTO dm_action VALUES (6, 'StartServer', '${DMHOME}\scripts\deployws.${EXT}', NULL, 1, NULL, NULL, 'N', NULL, NULL, NULL, 'N', 1, 1, NULL, 1, NULL, NULL, 'N', 'Y', 4, 5);
INSERT INTO dm_action VALUES (7, 'StopApp', '${DMHOME}\scripts\deployws.${EXT}', NULL, 1, NULL, NULL, 'N', NULL, NULL, NULL, 'N', 1, 1, NULL, 1, NULL, NULL, 'N', 'Y', 4, 5);
INSERT INTO dm_action VALUES (8, 'StartApp', '${DMHOME}\scripts\deployws.${EXT}', NULL, 1, NULL, NULL, 'N', NULL, NULL, NULL, 'N', 1, 1, NULL, 1, NULL, NULL, 'N', 'Y', 4, 5);
INSERT INTO dm_action VALUES (9, 'TomcatPreDeploy', '$DMHOME\scripts\tomcatctl.${EXT}', 'Shuts down Tomcat before deployment', 1, NULL, NULL, 'N', NULL, NULL, NULL, 'N', 1, 1, 1404852263, 1, 1404852566, NULL, 'N', 'Y', 4, NULL);
INSERT INTO dm_action VALUES (10, 'TomcatPostDeploy', '$DMHOME\scripts\tomcatctl.${EXT}', 'Cleans Work Directory and Starts Tomcat', 1, NULL, NULL, 'N', NULL, NULL, NULL, 'N', 1, 1, 1404852532, 1, 1404852640, NULL, 'N', 'Y', 4, NULL);
INSERT INTO dm_action VALUES (11, 'Rollup_Rollback', NULL, 'Iterates through all versions of an application, deploying the components to the appropriate server(s), using the component''s rollup and rollback switches', 1, NULL, NULL, 'N', NULL, NULL, NULL, 'Y', 1, 1, 1404909582, 1, 1404910018, NULL, 'N', NULL, 6, NULL);
INSERT INTO dm_action VALUES (12, 'Demo_Amend_Config', NULL, 'Amends the configuration file for each target server', 2, NULL, NULL, 'N', NULL, NULL, NULL, 'Y', NULL, NULL, NULL, NULL, NULL, NULL, 'N', NULL, 6, NULL);
INSERT INTO dm_action VALUES (13, 'extractzip', '${DMHOME}\scripts\extractzip.vbs', 'Extracts the contents of a zipfile', 1, NULL, NULL, 'N', NULL, 'N', NULL, 'N', 1, 1, NULL, 1, NULL, NULL, 'N', 'Y', 4, 4);
INSERT INTO dm_action VALUES (104, 'Demo_Extract_Zip', NULL, NULL, 2, NULL, NULL, 'N', NULL, NULL, NULL, 'Y', 1, 1, 1405296767, 1, 1405296767, NULL, 'N', NULL, 6, NULL);


--
-- TOC entry 2561 (class 0 OID 102266)
-- Dependencies: 165
-- Data for Name: dm_actionaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2562 (class 0 OID 102269)
-- Dependencies: 166
-- Data for Name: dm_actionarg; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_actionarg VALUES (1, 'arg17', 'false', 3, 'N', '-p', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (1, 'arg18', 'false', 4, 'N', '${server.basedir}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (1, 'arg16', 'false', 1, 'N', '-w', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (1, 'arg15', 'false', 2, 'N', '${PATH_TO_WSADMIN}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (1, 'arg22', 'false', 8, 'N', '${WSPASSWORD}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (1, 'arg20', 'false', 6, 'N', '${WSUSERNAME}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (1, 'arg21', 'false', 7, 'N', '-C', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (1, 'arg19', 'false', 5, 'N', '-U', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (1, 'DeployWebService', 'Checkbox', 16, 'N', '-s', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (1, 'CreateMBeansForResources', 'Checkbox', 14, 'N', '-m', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (1, 'ProcessEmbeddedConfiguration', 'Checkbox', 18, 'N', '-q', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (1, 'UseBinaryConfiguration', 'Checkbox', 11, 'N', '-b', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (1, 'ValidateInstall', 'Entry', 17, 'Y', '-v', 'N', NULL, 'S', '');
INSERT INTO dm_actionarg VALUES (1, 'DeployEnterpriseBeans', 'Checkbox', 12, 'N', '-j', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (1, 'DistributeApplication', 'Checkbox', 10, 'N', '-d', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (1, 'AllowDispatchingIncludesToRemoteResources', 'Checkbox', 19, 'N', '-D', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (1, 'AllowEJBReferenceTargetsToResolveAutomatically', 'Checkbox', 21, 'N', '-A', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (1, 'EnableClassReloading', 'Checkbox', 15, 'N', '-r', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (1, 'PrecompileJSPs', 'Checkbox', 22, 'N', '-c', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (1, 'ApplicationName', 'Entry', 13, 'Y', '-a', 'N', NULL, 'S', '');
INSERT INTO dm_actionarg VALUES (1, 'earfile', 'Entry', 9, 'Y', '-e', 'N', NULL, 'S', '');
INSERT INTO dm_actionarg VALUES (1, 'AllowServicingIncludesFromRemoteResources', 'Checkbox', 20, 'N', '-S', 'N', NULL, 'B', '');
INSERT INTO dm_actionarg VALUES (2, 'dirname', 'Entry', 1, 'N', NULL, 'Y', 1, NULL, NULL);
INSERT INTO dm_actionarg VALUES (3, 'servicename', 'Entry', 1, 'Y', NULL, 'Y', 2, NULL, NULL);
INSERT INTO dm_actionarg VALUES (3, 'action', 'Entry', 2, 'Y', NULL, 'Y', 1, NULL, NULL);
INSERT INTO dm_actionarg VALUES (5, 'arg1', 'false', 1, 'N', '-w', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (5, 'arg2', 'false', 2, 'N', '${PATH_TO_WSADMIN}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (5, 'arg3', 'false', 3, 'N', '-U', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (5, 'arg4', 'false', 4, 'N', '${WSUSERNAME}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (5, 'arg5', 'false', 5, 'N', '-C', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (5, 'arg6', 'false', 6, 'N', '${WSPASSWORD}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (5, 'nodename', 'Entry', 7, 'Y', '-node', 'Y', 1, 'S', NULL);
INSERT INTO dm_actionarg VALUES (5, 'servername', 'Entry', 8, 'Y', '-server', 'Y', 2, 'S', NULL);
INSERT INTO dm_actionarg VALUES (5, 'arg9', 'false', 9, 'N', '-tasks', 'N', NULL, 'A', NULL);
INSERT INTO dm_actionarg VALUES (5, 'arg10', 'false', 10, 'N', 'stopServer', 'N', NULL, 'A', NULL);
INSERT INTO dm_actionarg VALUES (6, 'arg1', 'false', 1, 'N', '-w', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (6, 'arg2', 'false', 2, 'N', '${PATH_TO_WSADMIN}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (6, 'arg3', 'false', 3, 'N', '-U', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (6, 'arg4', 'false', 4, 'N', '${WSUSERNAME}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (6, 'arg5', 'false', 5, 'N', '-C', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (6, 'arg6', 'false', 6, 'N', '${WSPASSWORD}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (6, 'nodename', 'Entry', 7, 'Y', '-node', 'Y', 1, 'S', NULL);
INSERT INTO dm_actionarg VALUES (6, 'servername', 'Entry', 8, 'Y', '-server', 'Y', 2, 'S', NULL);
INSERT INTO dm_actionarg VALUES (6, 'arg9', 'false', 9, 'N', '-tasks', 'N', NULL, 'A', NULL);
INSERT INTO dm_actionarg VALUES (6, 'arg10', 'false', 10, 'N', 'startServer', 'N', NULL, 'A', NULL);
INSERT INTO dm_actionarg VALUES (7, 'arg1', 'false', 1, 'N', '-w', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (7, 'arg2', 'false', 2, 'N', '${PATH_TO_WSADMIN}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (7, 'arg3', 'false', 3, 'N', '-U', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (7, 'arg4', 'false', 4, 'N', '${WSUSERNAME}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (7, 'arg5', 'false', 5, 'N', '-C', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (7, 'arg6', 'false', 6, 'N', '${WSPASSWORD}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (7, 'nodename', 'Entry', 7, 'Y', '-node', 'Y', 1, 'S', NULL);
INSERT INTO dm_actionarg VALUES (7, 'servername', 'Entry', 8, 'Y', '-server', 'Y', 2, 'S', NULL);
INSERT INTO dm_actionarg VALUES (7, 'appname', 'entry', 9, 'Y', '-a', 'Y', 3, 'S', NULL);
INSERT INTO dm_actionarg VALUES (7, 'arg10', 'false', 10, 'N', '-tasks', 'N', NULL, 'A', NULL);
INSERT INTO dm_actionarg VALUES (7, 'arg11', 'false', 11, 'N', 'stopApp', 'N', NULL, 'A', NULL);
INSERT INTO dm_actionarg VALUES (8, 'arg1', 'false', 1, 'N', '-w', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (8, 'arg2', 'false', 2, 'N', '${PATH_TO_WSADMIN}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (8, 'arg3', 'false', 3, 'N', '-U', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (8, 'arg4', 'false', 4, 'N', '${WSUSERNAME}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (8, 'arg5', 'false', 5, 'N', '-C', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (8, 'arg6', 'false', 6, 'N', '${WSPASSWORD}', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (8, 'nodename', 'Entry', 7, 'Y', '-node', 'Y', 1, 'S', NULL);
INSERT INTO dm_actionarg VALUES (8, 'servername', 'Entry', 8, 'Y', '-server', 'Y', 2, 'S', NULL);
INSERT INTO dm_actionarg VALUES (8, 'appname', 'entry', 9, 'Y', '-a', 'Y', 3, 'S', NULL);
INSERT INTO dm_actionarg VALUES (8, 'arg10', 'false', 10, 'N', '-tasks', 'N', NULL, 'A', NULL);
INSERT INTO dm_actionarg VALUES (8, 'arg11', 'false', 11, 'N', 'startApp', 'N', NULL, 'A', NULL);
INSERT INTO dm_actionarg VALUES (9, 'arg2', 'false', 2, 'N', '$TOMCAT_SERVICE_NAME', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (9, 'arg3', 'false', 3, 'N', '$TOMCAT_ROOT', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (9, 'arg1', 'false', 1, 'N', 'Pre', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (9, 'arg4', 'false', 4, 'N', '$APPLICATION_NAME', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (10, 'arg2', 'false', 2, 'N', '$TOMCAT_SERVICE_NAME', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (10, 'arg3', 'false', 3, 'N', '$TOMCAT_ROOT', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (10, 'arg1', 'false', 1, 'N', 'Post', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (10, 'arg4', 'false', 4, 'N', '$APPLICATION_NAME', 'N', NULL, 'A', '');
INSERT INTO dm_actionarg VALUES (13, 'folder', 'Entry', 1, 'N', NULL, 'Y', 1, NULL, NULL);
INSERT INTO dm_actionarg VALUES (13, 'zipfile', 'Entry', 2, 'N', NULL, 'Y', 2, NULL, NULL);


--
-- TOC entry 2563 (class 0 OID 102275)
-- Dependencies: 167
-- Data for Name: dm_actionedit; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_actionedit VALUES (12, 12, 1);
INSERT INTO dm_actionedit VALUES (104, 104, 1);


--
-- TOC entry 2564 (class 0 OID 102278)
-- Dependencies: 168
-- Data for Name: dm_actionfilepath; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2565 (class 0 OID 102284)
-- Dependencies: 169
-- Data for Name: dm_actionflows; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_actionflows VALUES (11, 1, 0, 1, 1);
INSERT INTO dm_actionflows VALUES (11, 1, 0, 2, 1);
INSERT INTO dm_actionflows VALUES (11, 1, 0, 2, 1);
INSERT INTO dm_actionflows VALUES (11, 1, 0, 4, 1);
INSERT INTO dm_actionflows VALUES (11, 1, 4, 3, 1);
INSERT INTO dm_actionflows VALUES (12, 1, 0, 1, 1);
INSERT INTO dm_actionflows VALUES (104, 1, 0, 1, 1);


--
-- TOC entry 2566 (class 0 OID 102287)
-- Dependencies: 170
-- Data for Name: dm_actionfragattrs; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_actionfragattrs VALUES (11, 4, 5501, 'Deploying Component ${component.name}');
INSERT INTO dm_actionfragattrs VALUES (12, 1, 28001, 'config.txt');
INSERT INTO dm_actionfragattrs VALUES (12, 1, 28002, 'SERVERNAME');
INSERT INTO dm_actionfragattrs VALUES (12, 1, 28003, '${server.name}');
INSERT INTO dm_actionfragattrs VALUES (12, 1, 28004, 'Y');
INSERT INTO dm_actionfragattrs VALUES (104, 1, 29002, '$ZIPFILE_NAME');
INSERT INTO dm_actionfragattrs VALUES (104, 1, 29001, '${server.basedir}');


--
-- TOC entry 2567 (class 0 OID 102293)
-- Dependencies: 171
-- Data for Name: dm_actionfrags; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_actionfrags VALUES (11, 1, 340, 260, 210, NULL, NULL, NULL);
INSERT INTO dm_actionfrags VALUES (11, 2, 340, 280, 220, NULL, NULL, 1);
INSERT INTO dm_actionfrags VALUES (11, 4, 340, 240, 55, 'Feedback Message', '', 2);
INSERT INTO dm_actionfrags VALUES (11, 3, 340, 360, 10, NULL, NULL, 2);
INSERT INTO dm_actionfrags VALUES (12, 1, 340, 300, 280, 'Amend Config', NULL, NULL);
INSERT INTO dm_actionfrags VALUES (104, 1, 340, 280, 290, '', '', NULL);


--
-- TOC entry 2568 (class 0 OID 102296)
-- Dependencies: 172
-- Data for Name: dm_actiontext; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2569 (class 0 OID 102302)
-- Dependencies: 173
-- Data for Name: dm_application; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_application VALUES (1, 'demoapp', 2, 1, NULL, 'Demo Application', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 100, 100, 100, 'N', NULL, 1, 2, 'N');


--
-- TOC entry 2570 (class 0 OID 102308)
-- Dependencies: 174
-- Data for Name: dm_applicationaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2571 (class 0 OID 102311)
-- Dependencies: 175
-- Data for Name: dm_applicationcomponent; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_applicationcomponent VALUES (1, 1, NULL, 100, 100);
INSERT INTO dm_applicationcomponent VALUES (1, 2, NULL, 100, 200);
INSERT INTO dm_applicationcomponent VALUES (1, 3, NULL, 100, 300);


--
-- TOC entry 2572 (class 0 OID 102314)
-- Dependencies: 176
-- Data for Name: dm_applicationcomponentflows; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_applicationcomponentflows VALUES (1, 0, 1);
INSERT INTO dm_applicationcomponentflows VALUES (1, 1, 2);
INSERT INTO dm_applicationcomponentflows VALUES (1, 2, 3);


--
-- TOC entry 2573 (class 0 OID 102317)
-- Dependencies: 177
-- Data for Name: dm_applicationvars; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2574 (class 0 OID 102323)
-- Dependencies: 178
-- Data for Name: dm_approval; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2575 (class 0 OID 102329)
-- Dependencies: 179
-- Data for Name: dm_appsallowedinenv; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_appsallowedinenv VALUES (1, 1);


--
-- TOC entry 2576 (class 0 OID 102332)
-- Dependencies: 180
-- Data for Name: dm_appsinenv; Type: TABLE DATA; Schema: dm; Owner: postgres
--

-- INSERT INTO dm_appsinenv VALUES (1, 1, 129, NULL, NULL, NULL);


--
-- TOC entry 2577 (class 0 OID 102338)
-- Dependencies: 181
-- Data for Name: dm_arrayvalues; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2578 (class 0 OID 102344)
-- Dependencies: 182
-- Data for Name: dm_availability; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2579 (class 0 OID 102347)
-- Dependencies: 183
-- Data for Name: dm_calendar; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2580 (class 0 OID 102350)
-- Dependencies: 184
-- Data for Name: dm_category; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_category VALUES (1, 'General');
INSERT INTO dm_category VALUES (2, 'Loops');
INSERT INTO dm_category VALUES (3, 'Build');
INSERT INTO dm_category VALUES (4, 'Deploy');
INSERT INTO dm_category VALUES (5, 'Windows');
INSERT INTO dm_category VALUES (6, 'WebSphere');
INSERT INTO dm_category VALUES (7, 'WebLogic');
INSERT INTO dm_category VALUES (8, 'Tomcat');
INSERT INTO dm_category VALUES (9, 'Dropzone');


--
-- TOC entry 2581 (class 0 OID 102353)
-- Dependencies: 185
-- Data for Name: dm_compitemprops; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_compitemprops VALUES (1, 'pattern', 'democomp1*.txt', 'N', 'N', 'N');
INSERT INTO dm_compitemprops VALUES (2, 'pattern', 'config.txt', 'N', 'N', 'N');
INSERT INTO dm_compitemprops VALUES (3, 'pattern', 'demo1.zip', 'N', 'N', 'N');
INSERT INTO dm_compitemprops VALUES (4, 'pattern', 'demo2.zip', 'N', 'N', 'N');


--
-- TOC entry 2582 (class 0 OID 102359)
-- Dependencies: 186
-- Data for Name: dm_component; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_component VALUES (1, 'DemoConfig', 2, 'Config File(s)', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 100, 100, NULL, 12, NULL, NULL, 'N', NULL, NULL, NULL, 'Y', NULL);
INSERT INTO dm_component VALUES (2, 'DemoApp1', 2, 'Demo Application Component 1', NULL, NULL, NULL, NULL, NULL, NULL, NULL, 100, 200, NULL, NULL, 104, NULL, 'N', NULL, NULL, NULL, 'Y', NULL);
INSERT INTO dm_component VALUES (3, 'DemoApp2', 2, 'Demo Application Component 2', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 104, NULL, 'N', NULL, NULL, NULL, 'Y', NULL);


--
-- TOC entry 2583 (class 0 OID 102365)
-- Dependencies: 187
-- Data for Name: dm_componentaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2584 (class 0 OID 102368)
-- Dependencies: 188
-- Data for Name: dm_componentitem; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_componentitem VALUES (1, 1, 1, NULL, 'Item 1', 'democomp1 - Item 1', NULL, 140, 80, NULL, NULL, NULL, NULL, 'N', NULL, NULL);
INSERT INTO dm_componentitem VALUES (2, 1, 1, NULL, 'Item 2', 'democomp1 - config file', 1, 140, 220, NULL, NULL, NULL, NULL, 'N', NULL, NULL);
INSERT INTO dm_componentitem VALUES (3, 2, 1, NULL, 'demo1', 'zipfile for demo1', NULL, 140, 80, NULL, NULL, 1, 1405296563, 'N', NULL, NULL);
INSERT INTO dm_componentitem VALUES (4, 3, 1, NULL, 'Demo2', NULL, NULL, 160, 220, 1, 1405339037, 1, 1405339072, 'N', NULL, NULL);


--
-- TOC entry 2585 (class 0 OID 102374)
-- Dependencies: 189
-- Data for Name: dm_componentvars; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_componentvars VALUES (3, 'ZIPFILE_NAME', 'demo2.zip', NULL, NULL);
INSERT INTO dm_componentvars VALUES (2, 'ZIPFILE_NAME', 'demo1.zip', NULL, NULL);


--
-- TOC entry 2586 (class 0 OID 102380)
-- Dependencies: 190
-- Data for Name: dm_compsallowedonserv; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_compsallowedonserv VALUES (1, 1);
INSERT INTO dm_compsallowedonserv VALUES (1, 2);
INSERT INTO dm_compsallowedonserv VALUES (2, 1);
INSERT INTO dm_compsallowedonserv VALUES (3, 2);


--
-- TOC entry 2587 (class 0 OID 102383)
-- Dependencies: 191
-- Data for Name: dm_compsonserv; Type: TABLE DATA; Schema: dm; Owner: postgres
--

-- INSERT INTO dm_compsonserv VALUES (1, 1, 129, NULL, NULL, NULL);
-- INSERT INTO dm_compsonserv VALUES (1, 2, 129, NULL, NULL, NULL);
-- INSERT INTO dm_compsonserv VALUES (2, 1, 129, NULL, NULL, NULL);
-- INSERT INTO dm_compsonserv VALUES (3, 2, 129, NULL, NULL, NULL);


--
-- TOC entry 2588 (class 0 OID 102389)
-- Dependencies: 192
-- Data for Name: dm_credentials; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2589 (class 0 OID 102395)
-- Dependencies: 193
-- Data for Name: dm_credentialsaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2590 (class 0 OID 102398)
-- Dependencies: 194
-- Data for Name: dm_datasource; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2591 (class 0 OID 102404)
-- Dependencies: 195
-- Data for Name: dm_datasourceaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2592 (class 0 OID 102407)
-- Dependencies: 196
-- Data for Name: dm_datasourceprops; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2593 (class 0 OID 102413)
-- Dependencies: 197
-- Data for Name: dm_datasourcevars; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2594 (class 0 OID 102419)
-- Dependencies: 198
-- Data for Name: dm_deployment; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2595 (class 0 OID 102422)
-- Dependencies: 199
-- Data for Name: dm_deploymentaction; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2596 (class 0 OID 102425)
-- Dependencies: 200
-- Data for Name: dm_deploymentlog; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2597 (class 0 OID 102431)
-- Dependencies: 201
-- Data for Name: dm_deploymentprops; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2598 (class 0 OID 102437)
-- Dependencies: 202
-- Data for Name: dm_deploymentscript; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2599 (class 0 OID 102443)
-- Dependencies: 203
-- Data for Name: dm_deploymentstep; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2600 (class 0 OID 102446)
-- Dependencies: 204
-- Data for Name: dm_deploymentxfer; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2601 (class 0 OID 102452)
-- Dependencies: 205
-- Data for Name: dm_domain; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_domain VALUES (1, 'GLOBAL', NULL, 1, NULL, NULL, NULL, NULL, NULL, NULL, 'N', NULL, NULL);
INSERT INTO dm_domain VALUES (2, 'Demo', 1, 1, NULL, NULL, NULL, NULL, NULL, NULL, 'N', NULL, NULL);


--
-- TOC entry 2602 (class 0 OID 102458)
-- Dependencies: 206
-- Data for Name: dm_domainaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_domainaccess VALUES (1, 1, 'Y', 'Y', 'Y', NULL);


--
-- TOC entry 2603 (class 0 OID 102461)
-- Dependencies: 207
-- Data for Name: dm_domaininherit; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_domaininherit VALUES (1, 1, 'Y', 'Y', 'Y', 'Y');


--
-- TOC entry 2604 (class 0 OID 102464)
-- Dependencies: 208
-- Data for Name: dm_engine; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_engine VALUES (1, 'Deployment Engine', 'localhost', 1, NULL, NULL, NULL, NULL, 'N');


--
-- TOC entry 2605 (class 0 OID 102467)
-- Dependencies: 209
-- Data for Name: dm_engineaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2606 (class 0 OID 102470)
-- Dependencies: 210
-- Data for Name: dm_engineconfig; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_engineconfig VALUES (1, 'threadlimit', '4');


--
-- TOC entry 2607 (class 0 OID 102476)
-- Dependencies: 211
-- Data for Name: dm_environment; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_environment VALUES (1, 'demoenv', 1, NULL, 2, NULL, 'Demo Environment', NULL, NULL, NULL, NULL, NULL, NULL, NULL , NULL, NULL, 'N');


--
-- TOC entry 2608 (class 0 OID 102482)
-- Dependencies: 212
-- Data for Name: dm_environmentaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2609 (class 0 OID 102485)
-- Dependencies: 213
-- Data for Name: dm_environmentvars; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2610 (class 0 OID 102491)
-- Dependencies: 214
-- Data for Name: dm_field; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2611 (class 0 OID 102497)
-- Dependencies: 215
-- Data for Name: dm_fieldvalues; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2612 (class 0 OID 102500)
-- Dependencies: 216
-- Data for Name: dm_fragmentattrs; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_fragmentattrs VALUES (1001, 10, 'dropdown', 'Component', 'dm_component', 'Y', 1);
INSERT INTO dm_fragmentattrs VALUES (2001, 20, 'dropdown', 'Repository', 'dm_repository', 'Y', 1);
INSERT INTO dm_fragmentattrs VALUES (2002, 20, 'entry', 'Path', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (2003, 20, 'entry', 'Pattern', NULL, 'N', 3);
INSERT INTO dm_fragmentattrs VALUES (3001, 30, 'dropdown', 'Repository', 'dm_repository', 'Y', 1);
INSERT INTO dm_fragmentattrs VALUES (3002, 30, 'entry', 'Path', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (3003, 30, 'entry', 'Pattern', NULL, 'N', 3);
INSERT INTO dm_fragmentattrs VALUES (4001, 40, 'entry', 'Attribute Name', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (4002, 40, 'entry', 'Attribute Value', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (4003, 40, 'entry', 'Dropzone Name', NULL, 'N', 3);
INSERT INTO dm_fragmentattrs VALUES (5001, 50, 'entry', 'name', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (5002, 50, 'entry', 'value', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (5101, 51, 'entry', 'name', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (5201, 52, 'entry', 'name', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (5202, 52, 'entry', 'amount', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (5301, 53, 'entry', 'name', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (5302, 53, 'entry', 'value1', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (5303, 53, 'entry', 'value2', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (5501, 55, 'entry', 'Message', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (5601, 56, 'entry', 'Stream Name', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (5801, 58, 'entry', 'Delay', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (6001, 60, 'entry', 'Expression', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (7001, 70, 'entry', 'Script', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (7002, 70, 'entry', 'Param 1', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (7003, 70, 'entry', 'Param 2', NULL, 'N', 3);
INSERT INTO dm_fragmentattrs VALUES (7004, 70, 'entry', 'Param 3', NULL, 'N', 4);
INSERT INTO dm_fragmentattrs VALUES (7005, 70, 'entry', 'Param 4', NULL, 'N', 5);
INSERT INTO dm_fragmentattrs VALUES (7006, 70, 'entry', 'Param 5', NULL, 'N', 6);
INSERT INTO dm_fragmentattrs VALUES (8001, 80, 'entry', 'Service Name', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (9001, 90, 'entry', 'Service Name', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (1101, 110, 'entry', 'Message', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (12001, 120, 'Checkbox', 'PrecompileJSPs', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (12002, 120, 'Entry', 'earfile', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (12003, 120, 'Checkbox', 'DistributeApplication', NULL, 'N', 3);
INSERT INTO dm_fragmentattrs VALUES (12004, 120, 'Checkbox', 'UseBinaryConfiguration', NULL, 'N', 4);
INSERT INTO dm_fragmentattrs VALUES (12005, 120, 'Checkbox', 'DeployEnterpriseBeans', NULL, 'N', 5);
INSERT INTO dm_fragmentattrs VALUES (12006, 120, 'Entry', 'ApplicationName', NULL, 'N', 6);
INSERT INTO dm_fragmentattrs VALUES (12007, 120, 'Checkbox', 'CreateMBeansForResources', NULL, 'N', 7);
INSERT INTO dm_fragmentattrs VALUES (12008, 120, 'Checkbox', 'EnableClassReloading', NULL, 'N', 8);
INSERT INTO dm_fragmentattrs VALUES (12009, 120, 'Checkbox', 'DeployWebService', NULL, 'N', 9);
INSERT INTO dm_fragmentattrs VALUES (12010, 120, 'Entry', 'ValidateInstall', NULL, 'N', 10);
INSERT INTO dm_fragmentattrs VALUES (12011, 120, 'Checkbox', 'ProcessEmbeddedConfiguration', NULL, 'N', 11);
INSERT INTO dm_fragmentattrs VALUES (12012, 120, 'Checkbox', 'AllowDispatchingIncludesToRemoteResources', NULL, 'N', 12);
INSERT INTO dm_fragmentattrs VALUES (12013, 120, 'Checkbox', 'AllowServicingIncludesFromRemoteResources', NULL, 'N', 13);
INSERT INTO dm_fragmentattrs VALUES (12014, 120, 'Checkbox', 'AllowEJBReferenceTargetsToResolveAutomatically', NULL, 'N', 14);
INSERT INTO dm_fragmentattrs VALUES (13001, 130, 'entry', 'dirname', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (13002, 130, 'entry', 'arrayname', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (14001, 140, 'entry', 'arrayname', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (15001, 150, 'entry', 'nodename', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (15002, 150, 'entry', 'servername', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (16001, 160, 'entry', 'nodename', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (16002, 160, 'entry', 'servername', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (17001, 170, 'entry', 'nodename', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (17002, 170, 'entry', 'servername', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (17003, 170, 'entry', 'appname', NULL, 'N', 3);
INSERT INTO dm_fragmentattrs VALUES (18001, 180, 'entry', 'nodename', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (18002, 180, 'entry', 'servername', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (18003, 180, 'entry', 'appname', NULL, 'N', 3);
INSERT INTO dm_fragmentattrs VALUES (20001, 200, 'entry', 'key', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (20002, 200, 'entry', 'array', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (23001, 230, 'entry', 'dropzone', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (24001, 240, 'entry', 'varname', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (24002, 240, 'entry', 'pattern', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (25001, 250, 'entry', 'filename', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (26001, 260, 'entry', 'oldname', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (26002, 260, 'entry', 'newname', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (26003, 260, 'Checkbox', 'Server Specific', NULL, 'N', 3);
INSERT INTO dm_fragmentattrs VALUES (27001, 270, 'entry', 'filename', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (27002, 270, 'entry', 'xpath', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (27003, 270, 'entry', 'NewElement', NULL, 'N', 3);
INSERT INTO dm_fragmentattrs VALUES (27004, 270, 'Checkbox', 'Server Specific', NULL, 'N', 4);
INSERT INTO dm_fragmentattrs VALUES (28001, 280, 'entry', 'filename', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (28002, 280, 'entry', 'Old String', NULL, 'N', 2);
INSERT INTO dm_fragmentattrs VALUES (28003, 280, 'entry', 'New String', NULL, 'N', 3);
INSERT INTO dm_fragmentattrs VALUES (28004, 280, 'Checkbox', 'Server Specific', NULL, 'N', 4);
INSERT INTO dm_fragmentattrs VALUES (29001, 290, 'entry', 'folder', NULL, 'N', 1);
INSERT INTO dm_fragmentattrs VALUES (29002, 290, 'entry', 'zipfile', NULL, 'N', 2);


--
-- TOC entry 2613 (class 0 OID 102503)
-- Dependencies: 217
-- Data for Name: dm_fragments; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_fragments VALUES (10, 'Deploy Component', 'Deploys the specified component to the appropriate server(s) within the targeted environment', 4, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (20, 'Deploy', 'Extracts items from the specified repository and deploys then to the server(s) within the targeted environment. Use IF conditions to filter the server list to those matching the specified criteria', 4, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (30, 'Checkout', 'Extracts items from the specified repository and places them in the dropzone', 4, 1, 1, NULL, 1, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (40, 'Transfer', 'Takes items from the specified dropzone and deploys then to the server(s) within the targeted environment. Use IF conditions to filter the server list to those matching the specified criteria"', 4, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (50, 'Set Variable', 'Creates and sets a variable', 1, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (51, 'Increment Variable', 'Increments a variable', 1, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (52, 'Add Value to  Variable', 'Add a value to a variable', 1, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (53, 'Add Two Values', 'Add two values and stores the result in a variable', 1, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (54, 'Server Loop', 'Iterates through the current set of servers', 2, 1, NULL, NULL, NULL, NULL, 'Y', NULL);
INSERT INTO dm_fragments VALUES (55, 'Echo', 'Echoes a message', 1, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (56, 'Using Stream', 'Use a named stream', 1, 1, NULL, NULL, NULL, NULL, 'Y', NULL);
INSERT INTO dm_fragments VALUES (57, 'Parallel', 'Execute blocks in parallel', 1, 2, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (58, 'Sleep', 'Pauses for n seconds', 1, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (60, 'If', 'General Condition Expression evaluation. Also use this to filter the list of servers based on those matching the specified criteria', 1, 2, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (70, 'Execute', 'Runs a script on the appropriate server(s) within the targeted environment. Use IF conditions to filter the server list to those matching the specified criteria', 4, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (80, 'Stop Service', 'Stops a Windows Service on the appropriate server(s) within the targeted environment. Use IF conditions to filter the server list to those matching the specified criteria', 5, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (90, 'Start Service', 'Starts a Windows Service on the appropriate server(s) within the targeted environment. Use IF conditions to filter the server list to those matching the specified criteria', 5, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (110, 'Abort', 'Terminates Deployment with Error Condition', 1, 0, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (120, 'Deploy App', 'Deploys an Application to Websphere', 6, 1, 1, NULL, 1, NULL, NULL, 1);
INSERT INTO dm_fragments VALUES (130, 'Get Directory', 'Returns the contents of a directory into an array', 1, 1, 1, NULL, 1, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (140, 'List Services', 'Returns a list of installed services into an array', 5, 1, 1, NULL, 1, NULL, 'N', NULL);
INSERT INTO dm_fragments VALUES (150, 'Stop Server', 'Stops a Websphere Server', 6, 1, 1, NULL, 1, NULL, NULL, 5);
INSERT INTO dm_fragments VALUES (160, 'Start Server', 'Starts a Websphere Server', 6, 1, 1, NULL, 1, NULL, NULL, 6);
INSERT INTO dm_fragments VALUES (170, 'Stop Application', 'Stops a Websphere Application', 6, 1, 1, NULL, 1, NULL, NULL, 7);
INSERT INTO dm_fragments VALUES (180, 'Start Application', 'Starts a Websphere Application', 6, 1, 1, NULL, 1, NULL, NULL, 8);
INSERT INTO dm_fragments VALUES (200, 'Loop Through Array Keys', 'Loops through each element of an array, setting a variable to each key', 2, 1, 1, NULL, 1, NULL, 'Y', NULL);
INSERT INTO dm_fragments VALUES (201, 'TomcatPreDeploy', 'Shuts down Tomcat before deployment', 8, 1, 1, NULL, 1, NULL, NULL, 9);
INSERT INTO dm_fragments VALUES (202, 'TomcatPostDeploy', 'Cleans Work Directory and Starts Tomcat', 8, 1, 1, NULL, 1, NULL, NULL, 10);
INSERT INTO dm_fragments VALUES (210, 'Version Loop', 'Loops through each version between version on target server and version being deployed', 2, 1, 1, NULL, 1, NULL, 'Y', NULL);
INSERT INTO dm_fragments VALUES (220, 'Component Loop', 'Loops through each component that makes up the application', 2, 1, 1, NULL, 1, NULL, 'Y', NULL);
INSERT INTO dm_fragments VALUES (230, 'Using Dropzone', 'Use a named dropzone', 9, 1, NULL, NULL, NULL, NULL, 'Y', NULL);
INSERT INTO dm_fragments VALUES (240, 'Iterate through Dropzone', 'Loops through each file in the dropzone', 9, 1, NULL, NULL, NULL, NULL, 'Y', NULL);
INSERT INTO dm_fragments VALUES (250, 'Delete File', 'Removes a file from the dropzone', 9, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (260, 'Rename File', 'Renames a file in the dropzone. Setting "Server Specific" makes a copy specific to the current server', 9, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (270, 'XML - add Element', 'Modifies an XML file in the dropzone. Setting "Server Specific" makes a copy specific for each server', 9, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (280, 'Amend File Content', 'Modifies a flat file in the dropzone. Setting "Server Specific" makes a copy specific for each server', 9, 1, NULL, NULL, NULL, NULL, NULL, NULL);
INSERT INTO dm_fragments VALUES (290, 'Extract Zip', 'Extracts the contents of a zip file', 5, 1, NULL, NULL, NULL, NULL, NULL, NULL);


--
-- TOC entry 2614 (class 0 OID 102509)
-- Dependencies: 218
-- Data for Name: dm_fragmenttext; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_fragmenttext VALUES (10, 'using component "@component@" {
  deploy;
}', 0);
INSERT INTO dm_fragmenttext VALUES (20, 'deploy(repository: "@repository@", path: @''@path@'', pattern: "@pattern@");', 0);
INSERT INTO dm_fragmenttext VALUES (30, 'checkout(repository: "@repository@", path: @''@path@'', pattern: "@pattern@");', 0);
INSERT INTO dm_fragmenttext VALUES (40, 'transfer;', 0);
INSERT INTO dm_fragmenttext VALUES (50, 'set @name@ = @value@;', 0);
INSERT INTO dm_fragmenttext VALUES (51, 'incr @name@;', 0);
INSERT INTO dm_fragmenttext VALUES (52, 'set @name@ += @amount@;', 0);
INSERT INTO dm_fragmenttext VALUES (53, 'set @name@ = @value1@ + @value2@;', 0);
INSERT INTO dm_fragmenttext VALUES (54, 'psloop {
  @0@
}', 0);
INSERT INTO dm_fragmenttext VALUES (55, 'echo "@message@";', 0);
INSERT INTO dm_fragmenttext VALUES (56, 'using stream $@stream_name@ {
  @0@
}
', 0);
INSERT INTO dm_fragmenttext VALUES (57, 'parallel {
#if 1
  sequential {
    @1@
  }
#endif
#if 2
  sequential {
    @2@
  }
#endif
}
', 0);
INSERT INTO dm_fragmenttext VALUES (58, 'sleep(delay: @delay@);', 0);
INSERT INTO dm_fragmenttext VALUES (60, 'if (@expression@) {
    @1@
}
#if 2
else {
    @2@}
#endif', 1);
INSERT INTO dm_fragmenttext VALUES (80, 'servicectl(action: ''Stop'', servicename: "@servicename@");', 0);
INSERT INTO dm_fragmenttext VALUES (90, 'servicectl(action: ''Start'', servicename: "@servicename@");', 0);
INSERT INTO dm_fragmenttext VALUES (110, 'abort(msg: ''@message@'');', 0);
INSERT INTO dm_fragmenttext VALUES (130, 'set @arrayname@ = dir("@dirname@");', 0);
INSERT INTO dm_fragmenttext VALUES (140, 'set @arrayname@ = listservices();', 0);
INSERT INTO dm_fragmenttext VALUES (200, 'foreach(@key@: $@array@) {
 @0@
}', 0);
INSERT INTO dm_fragmenttext VALUES (210, 'avloop {
  @0@
}', 0);
INSERT INTO dm_fragmenttext VALUES (220, 'comploop {
  @0@
}', 0);
INSERT INTO dm_fragmenttext VALUES (230, 'using dropzone $@dropzone@ {
  @0@
}
', 0);
INSERT INTO dm_fragmenttext VALUES (240, 'foreach(@varname@ : ${dropzone.find("@pattern@")}) {
  @0@
}', 0);
INSERT INTO dm_fragmenttext VALUES (250, 'delete(file: "@filename@");', 0);
INSERT INTO dm_fragmenttext VALUES (260, 'rename(from: "@oldname@", to: "@newname@", serverspecific: @server_specific@);', 0);
INSERT INTO dm_fragmenttext VALUES (270, 'modify(modifier: ''xml'', file: "@filename@", serverspecific: @server_specific@) {
  add_element(xpath: "@xpath@", pos: ''inside'',  value: "@newelement");}', 0);
INSERT INTO dm_fragmenttext VALUES (280, 'modify(modifier: ''text'', file: "@filename@", serverspecific: @server_specific@) {
  text_replace(find: "@old_string@", replace: "@new_string@");}', 0);
INSERT INTO dm_fragmenttext VALUES (290, 'extractzip(folder: "@folder@", zipfile: "@zipfile@");', 0);


--
-- TOC entry 2615 (class 0 OID 102515)
-- Dependencies: 219
-- Data for Name: dm_historyattachment; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2616 (class 0 OID 102518)
-- Dependencies: 220
-- Data for Name: dm_historycomment; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2617 (class 0 OID 102524)
-- Dependencies: 221
-- Data for Name: dm_historynote; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2618 (class 0 OID 102530)
-- Dependencies: 222
-- Data for Name: dm_historysubs; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2619 (class 0 OID 102533)
-- Dependencies: 223
-- Data for Name: dm_keys; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_keys VALUES ('Usergroup', 100);
INSERT INTO dm_keys VALUES ('approval', 100);
INSERT INTO dm_keys VALUES ('Environment', 100);
INSERT INTO dm_keys VALUES ('User', 100);
INSERT INTO dm_keys VALUES ('HistoryAttachment', 100);
INSERT INTO dm_keys VALUES ('fragmentattrs', 20000);
INSERT INTO dm_keys VALUES ('task', 100);
INSERT INTO dm_keys VALUES ('HistoryNote', 100);
INSERT INTO dm_keys VALUES ('Domain', 100);
INSERT INTO dm_keys VALUES ('credentials', 100);
INSERT INTO dm_keys VALUES ('Server', 100);
INSERT INTO dm_keys VALUES ('Application', 100);
INSERT INTO dm_keys VALUES ('calendar', 100);
INSERT INTO dm_keys VALUES ('Repository', 100);
INSERT INTO dm_keys VALUES ('category', 7);
INSERT INTO dm_keys VALUES ('fragments', 202);
INSERT INTO dm_keys VALUES ('notify', 1);
INSERT INTO dm_keys VALUES ('template', 2);
INSERT INTO dm_keys VALUES ('Action', 104);
INSERT INTO dm_keys VALUES ('component', 101);
INSERT INTO dm_keys VALUES ('componentitem', 101);


--
-- TOC entry 2620 (class 0 OID 102536)
-- Dependencies: 224
-- Data for Name: dm_notify; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_notify VALUES (1, 'SMTP_Notifier', NULL, 1, 1, NULL, 3, 'SMTP Notifier', NULL, 1, 1404910551, 1, 1404910600, 'N');


--
-- TOC entry 2621 (class 0 OID 102542)
-- Dependencies: 225
-- Data for Name: dm_notifyaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2622 (class 0 OID 102545)
-- Dependencies: 226
-- Data for Name: dm_notifyprops; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_notifyprops VALUES (1, 'mailserver', 'PLEASE_SET_MAILSERVER_DETAILS', 'N', 'N', 'N');
INSERT INTO dm_notifyprops VALUES (1, 'from', 'ReleaseEngineer', 'N', 'N', 'N');


--
-- TOC entry 2623 (class 0 OID 102551)
-- Dependencies: 227
-- Data for Name: dm_notifytext; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2624 (class 0 OID 102557)
-- Dependencies: 228
-- Data for Name: dm_notifyvars; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2625 (class 0 OID 102563)
-- Dependencies: 229
-- Data for Name: dm_plugin; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_plugin VALUES (1, 10000, 'harvest');
INSERT INTO dm_plugin VALUES (2, 10000, 'iisplugin');
INSERT INTO dm_plugin VALUES (3, 10000, 'perforce');
INSERT INTO dm_plugin VALUES (4, 10000, 'http');


--
-- TOC entry 2626 (class 0 OID 102566)
-- Dependencies: 230
-- Data for Name: dm_prerequisities; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2627 (class 0 OID 102569)
-- Dependencies: 231
-- Data for Name: dm_privileges; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_privileges VALUES (2, 2);
INSERT INTO dm_privileges VALUES (2, 3);
INSERT INTO dm_privileges VALUES (2, 4);
INSERT INTO dm_privileges VALUES (2, 5);
INSERT INTO dm_privileges VALUES (2, 6);
INSERT INTO dm_privileges VALUES (2, 7);
INSERT INTO dm_privileges VALUES (2, 8);
INSERT INTO dm_privileges VALUES (2, 9);
INSERT INTO dm_privileges VALUES (2, 10);
INSERT INTO dm_privileges VALUES (2, 11);
INSERT INTO dm_privileges VALUES (2, 13);
INSERT INTO dm_privileges VALUES (2, 17);
INSERT INTO dm_privileges VALUES (2, 19);
INSERT INTO dm_privileges VALUES (2, 90);


--
-- TOC entry 2628 (class 0 OID 102572)
-- Dependencies: 232
-- Data for Name: dm_propertydef; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_propertydef VALUES (1, 'broker', 'Y', 'N');
INSERT INTO dm_propertydef VALUES (1, 'datasource', 'N', 'N');
INSERT INTO dm_propertydef VALUES (1, 'dbpass', 'N', 'N');
INSERT INTO dm_propertydef VALUES (1, 'dbuser', 'N', 'N');
INSERT INTO dm_propertydef VALUES (1, 'viewpath', 'N', 'Y');
INSERT INTO dm_propertydef VALUES (1, 'process', 'N', 'N');
INSERT INTO dm_propertydef VALUES (1, 'state', 'N', 'N');
INSERT INTO dm_propertydef VALUES (1, 'project', 'N', 'N');
INSERT INTO dm_propertydef VALUES (2, 'url', 'N', 'N');
INSERT INTO dm_propertydef VALUES (2, 'path', 'N', 'Y');
INSERT INTO dm_propertydef VALUES (2, 'revision', 'N', 'N');
INSERT INTO dm_propertydef VALUES (3, 'mailserver', 'N', 'N');
INSERT INTO dm_propertydef VALUES (3, 'mailport', 'N', 'N');
INSERT INTO dm_propertydef VALUES (3, 'username', 'N', 'N');
INSERT INTO dm_propertydef VALUES (3, 'password', 'N', 'N');
INSERT INTO dm_propertydef VALUES (3, 'from', 'Y', 'N');
INSERT INTO dm_propertydef VALUES (3, 'logfile', 'N', 'N');
INSERT INTO dm_propertydef VALUES (4, 'username', 'N', 'N');
INSERT INTO dm_propertydef VALUES (4, 'password', 'N', 'N');
INSERT INTO dm_propertydef VALUES (4, 'dsn', 'N', 'N');
INSERT INTO dm_propertydef VALUES (10, 'filepath', 'Y', 'Y');
INSERT INTO dm_propertydef VALUES (10, 'pattern', 'N', 'N');
INSERT INTO dm_propertydef VALUES (10, 'recursive', 'N', 'N');
INSERT INTO dm_propertydef VALUES (10, 'version', 'N', 'N');
INSERT INTO dm_propertydef VALUES (11, 'password', 'N', 'N');
INSERT INTO dm_propertydef VALUES (11, 'username', 'N', 'N');
INSERT INTO dm_propertydef VALUES (11, 'from', 'Y', 'N');
INSERT INTO dm_propertydef VALUES (12, 'host', 'Y', 'N');
INSERT INTO dm_propertydef VALUES (12, 'view', 'Y', 'Y');
INSERT INTO dm_propertydef VALUES (12, 'port', 'N', 'N');
INSERT INTO dm_propertydef VALUES (13, 'filepath', 'N', 'Y');
INSERT INTO dm_propertydef VALUES (13, 'server', 'Y', 'N');
INSERT INTO dm_propertydef VALUES (13, 'version', 'N', 'N');
INSERT INTO dm_propertydef VALUES (13, 'pattern', 'N', 'N');
INSERT INTO dm_propertydef VALUES (13, 'recursive', 'N', 'N');
INSERT INTO dm_propertydef VALUES (13, 'UseSSL', 'N', 'N');
INSERT INTO dm_propertydef VALUES (14, 'filename', 'N', 'N');
INSERT INTO dm_propertydef VALUES (14, 'params', 'N', 'Y');
INSERT INTO dm_propertydef VALUES (14, 'port', 'N', 'N');
INSERT INTO dm_propertydef VALUES (14, 'server', 'Y', 'Y');
INSERT INTO dm_propertydef VALUES (14, 'URI', 'N', 'Y');
INSERT INTO dm_propertydef VALUES (14, 'version', 'N', 'N');
INSERT INTO dm_propertydef VALUES (15, 'filepath', 'Y', 'Y');
INSERT INTO dm_propertydef VALUES (15, 'pattern', 'N', 'N');
INSERT INTO dm_propertydef VALUES (15, 'recursive', 'N', 'N');
INSERT INTO dm_propertydef VALUES (15, 'buildno', 'N', 'N');


--
-- TOC entry 2629 (class 0 OID 102575)
-- Dependencies: 233
-- Data for Name: dm_providerdef; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_providerdef VALUES (0, 'unconfigured', 0, 'unconfigured', NULL);
INSERT INTO dm_providerdef VALUES (1, 'harvest', 8, 'repository', 1);
INSERT INTO dm_providerdef VALUES (2, 'svn', 8, 'repository', NULL);
INSERT INTO dm_providerdef VALUES (3, 'smtpemail', 9, 'notify', NULL);
INSERT INTO dm_providerdef VALUES (4, 'odbc', 10, 'datasource', NULL);
INSERT INTO dm_providerdef VALUES (5, 'ftp', 16, 'transport', NULL);
INSERT INTO dm_providerdef VALUES (6, 'ftps', 16, 'transport', NULL);
INSERT INTO dm_providerdef VALUES (7, 'sftp', 16, 'transport', NULL);
INSERT INTO dm_providerdef VALUES (8, 'win', 16, 'transport', NULL);
INSERT INTO dm_providerdef VALUES (9, 'remote', 11, 'script', NULL);
INSERT INTO dm_providerdef VALUES (10, 'filesystem', 8, 'repository', NULL);
INSERT INTO dm_providerdef VALUES (11, 'txtlocal', 9, 'notify', NULL);
INSERT INTO dm_providerdef VALUES (12, 'perforce', 8, 'repository', 3);
INSERT INTO dm_providerdef VALUES (13, 'ftprep', 8, 'repository', NULL);
INSERT INTO dm_providerdef VALUES (14, 'http', 8, 'repository', 4);
INSERT INTO dm_providerdef VALUES (15, 'meister', 8, 'repository', NULL);


--
-- TOC entry 2630 (class 0 OID 102581)
-- Dependencies: 234
-- Data for Name: dm_providerdefprops; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_providerdefprops VALUES (1, 'hci_bin', '${HARVESTHOME}\hci.exe', NULL, NULL, NULL, 1);
INSERT INTO dm_providerdefprops VALUES (1, 'hco_bin', '${HARVESTHOME}\hco.exe', NULL, NULL, NULL, 1);
INSERT INTO dm_providerdefprops VALUES (1, 'hcp_bin', '${HARVESTHOME}\hcp.exe', NULL, NULL, NULL, 1);
INSERT INTO dm_providerdefprops VALUES (2, 'svn_bin', '${DMHOME}\scripts\svn_wrapper.cmd', NULL, NULL, NULL, 1);
INSERT INTO dm_providerdefprops VALUES (12, 'p4_bin', '${PERFORCEHOME}\p4.exe', NULL, NULL, NULL, 1);


--
-- TOC entry 2631 (class 0 OID 102587)
-- Dependencies: 235
-- Data for Name: dm_repository; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_repository VALUES (1, 'demorep', 'filesystem', 2, NULL, NULL, 10, 'Demo Repository', NULL, NULL, NULL, NULL, NULL, 'N');


--
-- TOC entry 2632 (class 0 OID 102593)
-- Dependencies: 236
-- Data for Name: dm_repositoryaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2633 (class 0 OID 102596)
-- Dependencies: 237
-- Data for Name: dm_repositoryignore; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_repositoryignore VALUES (1, '.harvest.sig', 'f');
INSERT INTO dm_repositoryignore VALUES (1, 'harvest.sig', 'f');
INSERT INTO dm_repositoryignore VALUES (2, '.svn', 't');


--
-- TOC entry 2634 (class 0 OID 102599)
-- Dependencies: 238
-- Data for Name: dm_repositoryprops; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_repositoryprops VALUES (1, 'filepath', '$DMHOME\demorep', 'N', 'N', 'N');


--
-- TOC entry 2635 (class 0 OID 102605)
-- Dependencies: 239
-- Data for Name: dm_repositorytextpattern; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2636 (class 0 OID 102611)
-- Dependencies: 240
-- Data for Name: dm_repositoryvars; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2637 (class 0 OID 102617)
-- Dependencies: 241
-- Data for Name: dm_request; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2638 (class 0 OID 102623)
-- Dependencies: 242
-- Data for Name: dm_sequence; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_sequence VALUES ('DEPLOYID', 129);


--
-- TOC entry 2639 (class 0 OID 102626)
-- Dependencies: 243
-- Data for Name: dm_server; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_server VALUES (1, 'demo_server1', 'localhost', 1, 'win', NULL, NULL, 'Demo Server 1', NULL, 2, '$DMHOME\demo\demoserver1', 2, NULL, NULL, NULL, NULL, 'N');
INSERT INTO dm_server VALUES (2, 'demo_server2', 'localhost', 1, 'win', NULL, NULL, 'Demo Server 2', NULL, 2, '$DMHOME\demo\demoserver2', 2, NULL, NULL, NULL, NULL, 'N');


--
-- TOC entry 2640 (class 0 OID 102632)
-- Dependencies: 244
-- Data for Name: dm_server_connections; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_server_connections VALUES (1, 1, 4, 2, 1, NULL, NULL);


--
-- TOC entry 2641 (class 0 OID 102635)
-- Dependencies: 245
-- Data for Name: dm_serveraccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2642 (class 0 OID 102638)
-- Dependencies: 246
-- Data for Name: dm_serversinenv; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_serversinenv VALUES (1, 1, 100, 100);
INSERT INTO dm_serversinenv VALUES (1, 2, 100, 200);


--
-- TOC entry 2643 (class 0 OID 102641)
-- Dependencies: 247
-- Data for Name: dm_servertype; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_servertype VALUES (1, 'unix', 1, 'unix', 'sh');
INSERT INTO dm_servertype VALUES (2, 'windows', 2, 'windows', 'vbs');
INSERT INTO dm_servertype VALUES (3, 'as400', 0, 'as400', NULL);
INSERT INTO dm_servertype VALUES (4, 'mac', 3, 'unix', 'sh');
INSERT INTO dm_servertype VALUES (5, 'Tandem', 1, 'unix', NULL);
INSERT INTO dm_servertype VALUES (6, 'Stratus', 1, 'unix', NULL);
INSERT INTO dm_servertype VALUES (7, 'OpenVMS', 1, 'unix', NULL);


--
-- TOC entry 2644 (class 0 OID 102647)
-- Dependencies: 248
-- Data for Name: dm_servervars; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2645 (class 0 OID 102653)
-- Dependencies: 249
-- Data for Name: dm_tableinfo; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_tableinfo VALUES (20000);


--
-- TOC entry 2646 (class 0 OID 102656)
-- Dependencies: 250
-- Data for Name: dm_task; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_task VALUES (1, 'Deploy', 2, 2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);


--
-- TOC entry 2647 (class 0 OID 102659)
-- Dependencies: 251
-- Data for Name: dm_taskaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_taskaccess VALUES (1, 2, NULL, NULL);


--
-- TOC entry 2648 (class 0 OID 102662)
-- Dependencies: 252
-- Data for Name: dm_taskapprove; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2649 (class 0 OID 102665)
-- Dependencies: 253
-- Data for Name: dm_taskcopy; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2650 (class 0 OID 102668)
-- Dependencies: 254
-- Data for Name: dm_taskcreateversion; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2651 (class 0 OID 102671)
-- Dependencies: 255
-- Data for Name: dm_taskmove; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2652 (class 0 OID 102674)
-- Dependencies: 256
-- Data for Name: dm_taskrequest; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2653 (class 0 OID 102677)
-- Dependencies: 257
-- Data for Name: dm_tasktypes; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_tasktypes VALUES (1, 'Build', 'N', 'N', 'Y', 'Y');
INSERT INTO dm_tasktypes VALUES (2, 'Deploy', 'N', 'N', 'Y', 'Y');
INSERT INTO dm_tasktypes VALUES (3, 'Approve', 'N', 'N', 'Y', 'Y');
INSERT INTO dm_tasktypes VALUES (4, 'Move', 'N', 'N', 'Y', 'Y');
INSERT INTO dm_tasktypes VALUES (5, 'Copy', 'N', 'N', 'Y', 'Y');
INSERT INTO dm_tasktypes VALUES (6, 'Remove', 'N', 'N', 'Y', 'Y');
INSERT INTO dm_tasktypes VALUES (7, 'CreateVersion', 'N', 'N', 'Y', 'N');
INSERT INTO dm_tasktypes VALUES (8, 'Request', 'N', 'N', 'Y', 'Y');


--
-- TOC entry 2654 (class 0 OID 102680)
-- Dependencies: 258
-- Data for Name: dm_template; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_template VALUES (2, 'DeployFailure', NULL, 1, 1, 1, 1404910760, 1404910796, 'N', 'Application ${application.name} failed to deploy', 'Application ${application.name} failed to deploy to environment ${environment.name}.');
INSERT INTO dm_template VALUES (1, 'DeploySuccess', 'Success Template', 1, 1, 1, 1404910660, 1404910803, 'N', 'Application ${application.name} has been deployed successfully', 'Application ${application.name} has been deployed to environment ${environment.name} by user ${user.name}.');


--
-- TOC entry 2655 (class 0 OID 102686)
-- Dependencies: 259
-- Data for Name: dm_templaterecipients; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2656 (class 0 OID 102689)
-- Dependencies: 260
-- Data for Name: dm_user; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_user VALUES (1, 'admin', NULL, 'jGl25bVBBBW96Qi9Te4V37Fnqchz/Eu4qB9vKrRIqRg=', 1, 'Administrator', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'N');


--
-- TOC entry 2657 (class 0 OID 102695)
-- Dependencies: 261
-- Data for Name: dm_useraccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2658 (class 0 OID 102698)
-- Dependencies: 262
-- Data for Name: dm_usergroup; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_usergroup VALUES (2, 'Administrators', NULL, 1, 'Admin Users', NULL, NULL, NULL, 5, 1381338656, 'N', NULL, 'Y', 'Y', 'Y', 'Y', 'Y', 'Y');
INSERT INTO dm_usergroup VALUES (1, 'Everyone', NULL, 1, 'Everyone', NULL, NULL, NULL, 2, 1381312809, 'N', NULL, NULL, NULL, NULL, NULL, NULL, NULL);


--
-- TOC entry 2659 (class 0 OID 102704)
-- Dependencies: 263
-- Data for Name: dm_usergroupaccess; Type: TABLE DATA; Schema: dm; Owner: postgres
--



--
-- TOC entry 2660 (class 0 OID 102707)
-- Dependencies: 264
-- Data for Name: dm_usersingroup; Type: TABLE DATA; Schema: dm; Owner: postgres
--

INSERT INTO dm_usersingroup VALUES (1, 2);


--
-- TOC entry 2300 (class 2606 OID 102711)
-- Dependencies: 164 164
-- Name: action-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_action
    ADD CONSTRAINT "action-pk" PRIMARY KEY (id);


--
-- TOC entry 2302 (class 2606 OID 102713)
-- Dependencies: 165 165 165
-- Name: actionaccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_actionaccess
    ADD CONSTRAINT "actionaccess-pk" PRIMARY KEY (actionid, usrgrpid);


--
-- TOC entry 2304 (class 2606 OID 102715)
-- Dependencies: 166 166 166
-- Name: actionarg-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_actionarg
    ADD CONSTRAINT "actionarg-pk" PRIMARY KEY (actionid, name);


--
-- TOC entry 2306 (class 2606 OID 102717)
-- Dependencies: 168 168 168
-- Name: actionfilepath_pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_actionfilepath
    ADD CONSTRAINT actionfilepath_pk PRIMARY KEY (actionid, typeid);


--
-- TOC entry 2308 (class 2606 OID 102719)
-- Dependencies: 172 172
-- Name: actiontext-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_actiontext
    ADD CONSTRAINT "actiontext-pk" PRIMARY KEY (id);


--
-- TOC entry 2310 (class 2606 OID 102721)
-- Dependencies: 173 173
-- Name: application-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_application
    ADD CONSTRAINT "application-pk" PRIMARY KEY (id);


--
-- TOC entry 2312 (class 2606 OID 102723)
-- Dependencies: 174 174 174
-- Name: applicationaccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_applicationaccess
    ADD CONSTRAINT "applicationaccess-pk" PRIMARY KEY (appid, usrgrpid);


--
-- TOC entry 2314 (class 2606 OID 102727)
-- Dependencies: 177 177 177
-- Name: applicationvars-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_applicationvars
    ADD CONSTRAINT "applicationvars-pk" PRIMARY KEY (appid, name);


--
-- TOC entry 2316 (class 2606 OID 102729)
-- Dependencies: 178 178
-- Name: approval-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_approval
    ADD CONSTRAINT "approval-pk" PRIMARY KEY (id);


--
-- TOC entry 2318 (class 2606 OID 102731)
-- Dependencies: 179 179 179
-- Name: appsinenv-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_appsallowedinenv
    ADD CONSTRAINT "appsinenv-pk" PRIMARY KEY (envid, appid);


--
-- TOC entry 2320 (class 2606 OID 102733)
-- Dependencies: 180 180 180
-- Name: appsinenv2-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_appsinenv
    ADD CONSTRAINT "appsinenv2-pk" PRIMARY KEY (envid, appid);


--
-- TOC entry 2322 (class 2606 OID 102735)
-- Dependencies: 181 181 181
-- Name: arrayvalues-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_arrayvalues
    ADD CONSTRAINT "arrayvalues-pk" PRIMARY KEY (id, name);


--
-- TOC entry 2324 (class 2606 OID 102737)
-- Dependencies: 183 183
-- Name: calendar-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_calendar
    ADD CONSTRAINT "calendar-pk" PRIMARY KEY (id);


--
-- TOC entry 2326 (class 2606 OID 102739)
-- Dependencies: 185 185 185
-- Name: compitemprops-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_compitemprops
    ADD CONSTRAINT "compitemprops-pk" PRIMARY KEY (compitemid, name);


--
-- TOC entry 2328 (class 2606 OID 102741)
-- Dependencies: 186 186
-- Name: component-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_component
    ADD CONSTRAINT "component-pk" PRIMARY KEY (id);


--
-- TOC entry 2330 (class 2606 OID 102743)
-- Dependencies: 187 187 187
-- Name: componentaccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_componentaccess
    ADD CONSTRAINT "componentaccess-pk" PRIMARY KEY (compid, usrgrpid);


--
-- TOC entry 2332 (class 2606 OID 102745)
-- Dependencies: 188 188
-- Name: componentitem-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_componentitem
    ADD CONSTRAINT "componentitem-pk" PRIMARY KEY (id);


--
-- TOC entry 2334 (class 2606 OID 102747)
-- Dependencies: 189 189 189
-- Name: componentvars-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_componentvars
    ADD CONSTRAINT "componentvars-pk" PRIMARY KEY (compid, name);


--
-- TOC entry 2336 (class 2606 OID 102749)
-- Dependencies: 190 190 190
-- Name: compsonserv-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_compsallowedonserv
    ADD CONSTRAINT "compsonserv-pk" PRIMARY KEY (compid, serverid);


--
-- TOC entry 2338 (class 2606 OID 102751)
-- Dependencies: 191 191 191
-- Name: compsonserv2-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_compsonserv
    ADD CONSTRAINT "compsonserv2-pk" PRIMARY KEY (compid, serverid);


--
-- TOC entry 2340 (class 2606 OID 102753)
-- Dependencies: 192 192
-- Name: credentials-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_credentials
    ADD CONSTRAINT "credentials-pk" PRIMARY KEY (id);


--
-- TOC entry 2342 (class 2606 OID 102755)
-- Dependencies: 193 193 193
-- Name: credentialsaccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_credentialsaccess
    ADD CONSTRAINT "credentialsaccess-pk" PRIMARY KEY (credid, usrgrpid);


--
-- TOC entry 2344 (class 2606 OID 102757)
-- Dependencies: 194 194
-- Name: datasource-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_datasource
    ADD CONSTRAINT "datasource-pk" PRIMARY KEY (id);


--
-- TOC entry 2346 (class 2606 OID 102759)
-- Dependencies: 195 195 195
-- Name: datasourceaccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_datasourceaccess
    ADD CONSTRAINT "datasourceaccess-pk" PRIMARY KEY (datasourceid, usrgrpid);


--
-- TOC entry 2348 (class 2606 OID 102761)
-- Dependencies: 196 196 196
-- Name: datasourceprops-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_datasourceprops
    ADD CONSTRAINT "datasourceprops-pk" PRIMARY KEY (datasourceid, name);


--
-- TOC entry 2350 (class 2606 OID 102763)
-- Dependencies: 197 197 197
-- Name: datasourcevars-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_datasourcevars
    ADD CONSTRAINT "datasourcevars-pk" PRIMARY KEY (datasourceid, name);


--
-- TOC entry 2419 (class 2606 OID 102765)
-- Dependencies: 242 242
-- Name: deploymentid_pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_sequence
    ADD CONSTRAINT deploymentid_pk PRIMARY KEY (seqname);


--
-- TOC entry 2372 (class 2606 OID 102767)
-- Dependencies: 216 216
-- Name: dm_fragmentattrs_pkey; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_fragmentattrs
    ADD CONSTRAINT dm_fragmentattrs_pkey PRIMARY KEY (id);


--
-- TOC entry 2443 (class 2606 OID 102769)
-- Dependencies: 264 264 264
-- Name: dm_usersingroup_pkey; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_usersingroup
    ADD CONSTRAINT dm_usersingroup_pkey PRIMARY KEY (userid, groupid);


--
-- TOC entry 2352 (class 2606 OID 102771)
-- Dependencies: 205 205
-- Name: domain-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_domain
    ADD CONSTRAINT "domain-pk" PRIMARY KEY (id);


--
-- TOC entry 2354 (class 2606 OID 102773)
-- Dependencies: 206 206 206
-- Name: domainaccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_domainaccess
    ADD CONSTRAINT "domainaccess-pk" PRIMARY KEY (domainid, usrgrpid);


--
-- TOC entry 2356 (class 2606 OID 102775)
-- Dependencies: 207 207 207
-- Name: domaininherit-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_domaininherit
    ADD CONSTRAINT "domaininherit-pk" PRIMARY KEY (domainid, usrgrpid);


--
-- TOC entry 2358 (class 2606 OID 102777)
-- Dependencies: 208 208
-- Name: engine-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_engine
    ADD CONSTRAINT "engine-pk" PRIMARY KEY (id);


--
-- TOC entry 2360 (class 2606 OID 102779)
-- Dependencies: 209 209 209
-- Name: engineaccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_engineaccess
    ADD CONSTRAINT "engineaccess-pk" PRIMARY KEY (engineid, usrgrpid);


--
-- TOC entry 2362 (class 2606 OID 102781)
-- Dependencies: 210 210 210
-- Name: engineconfig-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_engineconfig
    ADD CONSTRAINT "engineconfig-pk" PRIMARY KEY (engineid, name);


--
-- TOC entry 2364 (class 2606 OID 102783)
-- Dependencies: 211 211
-- Name: environment-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_environment
    ADD CONSTRAINT "environment-pk" PRIMARY KEY (id);


--
-- TOC entry 2366 (class 2606 OID 102785)
-- Dependencies: 213 213 213
-- Name: environmentvars-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_environmentvars
    ADD CONSTRAINT "environmentvars-pk" PRIMARY KEY (envid, name);


--
-- TOC entry 2368 (class 2606 OID 102787)
-- Dependencies: 214 214
-- Name: field-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_field
    ADD CONSTRAINT "field-pk" PRIMARY KEY (id);


--
-- TOC entry 2370 (class 2606 OID 102789)
-- Dependencies: 215 215 215
-- Name: fieldvalues-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_fieldvalues
    ADD CONSTRAINT "fieldvalues-pk" PRIMARY KEY (fieldid, positionid);


--
-- TOC entry 2375 (class 2606 OID 102791)
-- Dependencies: 218 218 218
-- Name: fragmenttext-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_fragmenttext
    ADD CONSTRAINT "fragmenttext-pk" PRIMARY KEY (fragmentid, type);


--
-- TOC entry 2377 (class 2606 OID 102793)
-- Dependencies: 219 219
-- Name: historyattachment-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_historyattachment
    ADD CONSTRAINT "historyattachment-pk" PRIMARY KEY (id);


--
-- TOC entry 2379 (class 2606 OID 102795)
-- Dependencies: 220 220 220 220 220
-- Name: historycomment-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_historycomment
    ADD CONSTRAINT "historycomment-pk" PRIMARY KEY (id, kind, note, userid);


--
-- TOC entry 2381 (class 2606 OID 102797)
-- Dependencies: 221 221
-- Name: historynote-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_historynote
    ADD CONSTRAINT "historynote-pk" PRIMARY KEY (id);


--
-- TOC entry 2383 (class 2606 OID 102799)
-- Dependencies: 222 222 222 222
-- Name: historysubs-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_historysubs
    ADD CONSTRAINT "historysubs-pk" PRIMARY KEY (id, kind, userid);


--
-- TOC entry 2385 (class 2606 OID 102801)
-- Dependencies: 224 224
-- Name: notify-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_notify
    ADD CONSTRAINT "notify-pk" PRIMARY KEY (id);


--
-- TOC entry 2387 (class 2606 OID 102803)
-- Dependencies: 225 225 225
-- Name: notifyaccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_notifyaccess
    ADD CONSTRAINT "notifyaccess-pk" PRIMARY KEY (notifyid, usrgrpid);


--
-- TOC entry 2389 (class 2606 OID 102805)
-- Dependencies: 226 226 226
-- Name: notifyprops-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_notifyprops
    ADD CONSTRAINT "notifyprops-pk" PRIMARY KEY (notifyid, name);


--
-- TOC entry 2391 (class 2606 OID 102807)
-- Dependencies: 227 227
-- Name: notifytext-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_notifytext
    ADD CONSTRAINT "notifytext-pk" PRIMARY KEY (id);


--
-- TOC entry 2393 (class 2606 OID 102809)
-- Dependencies: 228 228 228
-- Name: notifyvars-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_notifyvars
    ADD CONSTRAINT "notifyvars-pk" PRIMARY KEY (notifyid, name);


--
-- TOC entry 2395 (class 2606 OID 102811)
-- Dependencies: 229 229
-- Name: plugin-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_plugin
    ADD CONSTRAINT "plugin-pk" PRIMARY KEY (id);


--
-- TOC entry 2397 (class 2606 OID 102813)
-- Dependencies: 231 231 231
-- Name: privileges-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_privileges
    ADD CONSTRAINT "privileges-pk" PRIMARY KEY (groupid, cobjtype);


--
-- TOC entry 2399 (class 2606 OID 102815)
-- Dependencies: 232 232 232
-- Name: propertydef-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_propertydef
    ADD CONSTRAINT "propertydef-pk" PRIMARY KEY (defid, name);


--
-- TOC entry 2401 (class 2606 OID 102817)
-- Dependencies: 233 233
-- Name: providerdef-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_providerdef
    ADD CONSTRAINT "providerdef-pk" PRIMARY KEY (id);


--
-- TOC entry 2403 (class 2606 OID 102819)
-- Dependencies: 234 234 234
-- Name: providerdefprops-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_providerdefprops
    ADD CONSTRAINT "providerdefprops-pk" PRIMARY KEY (defid, name);


--
-- TOC entry 2407 (class 2606 OID 102821)
-- Dependencies: 236 236
-- Name: repositoryaccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_repositoryaccess
    ADD CONSTRAINT "repositoryaccess-pk" PRIMARY KEY (repositoryid,usrgrpid);


--
-- TOC entry 2411 (class 2606 OID 102823)
-- Dependencies: 238 238 238
-- Name: repositoryprops-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_repositoryprops
    ADD CONSTRAINT "repositoryprops-pk" PRIMARY KEY (repositoryid, name);


--
-- TOC entry 2413 (class 2606 OID 102825)
-- Dependencies: 239 239 239 239
-- Name: repositorytextpatterns; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_repositorytextpattern
    ADD CONSTRAINT repositorytextpatterns PRIMARY KEY (repositoryid, path, pattern);


--
-- TOC entry 2415 (class 2606 OID 102827)
-- Dependencies: 240 240 240
-- Name: repositoryvars-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_repositoryvars
    ADD CONSTRAINT "repositoryvars-pk" PRIMARY KEY (repositoryid, name);


--
-- TOC entry 2417 (class 2606 OID 102829)
-- Dependencies: 241 241
-- Name: request-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_request
    ADD CONSTRAINT "request-pk" PRIMARY KEY (id);


--
-- TOC entry 2405 (class 2606 OID 102831)
-- Dependencies: 235 235
-- Name: respository-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_repository
    ADD CONSTRAINT "respository-pk" PRIMARY KEY (id);


--
-- TOC entry 2409 (class 2606 OID 102833)
-- Dependencies: 237 237 237
-- Name: respositoryignore-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_repositoryignore
    ADD CONSTRAINT "respositoryignore-pk" PRIMARY KEY (defid, pattern);


--
-- TOC entry 2421 (class 2606 OID 102835)
-- Dependencies: 243 243
-- Name: server-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_server
    ADD CONSTRAINT "server-pk" PRIMARY KEY (id);


--
-- TOC entry 2423 (class 2606 OID 102837)
-- Dependencies: 245 245 245
-- Name: serveraccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_serveraccess
    ADD CONSTRAINT "serveraccess-pk" PRIMARY KEY (serverid, usrgrpid);


--
-- TOC entry 2425 (class 2606 OID 102839)
-- Dependencies: 246 246 246
-- Name: serversinenv-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_serversinenv
    ADD CONSTRAINT "serversinenv-pk" PRIMARY KEY (envid, serverid);


--
-- TOC entry 2427 (class 2606 OID 102841)
-- Dependencies: 247 247
-- Name: servertype-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_servertype
    ADD CONSTRAINT "servertype-pk" PRIMARY KEY (id);


--
-- TOC entry 2429 (class 2606 OID 102843)
-- Dependencies: 248 248 248
-- Name: servervars-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_servervars
    ADD CONSTRAINT "servervars-pk" PRIMARY KEY (serverid, name);


--
-- TOC entry 2431 (class 2606 OID 102845)
-- Dependencies: 250 250
-- Name: task-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_task
    ADD CONSTRAINT "task-pk" PRIMARY KEY (id);


--
-- TOC entry 2433 (class 2606 OID 102847)
-- Dependencies: 254 254
-- Name: taskcreateversion-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_taskcreateversion
    ADD CONSTRAINT "taskcreateversion-pk" PRIMARY KEY (id);


--
-- TOC entry 2435 (class 2606 OID 102849)
-- Dependencies: 260 260
-- Name: user-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_user
    ADD CONSTRAINT "user-pk" PRIMARY KEY (id);


--
-- TOC entry 2437 (class 2606 OID 102851)
-- Dependencies: 261 261 261
-- Name: useraccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_useraccess
    ADD CONSTRAINT "useraccess-pk" PRIMARY KEY (userid, usrgrpid);


--
-- TOC entry 2439 (class 2606 OID 102853)
-- Dependencies: 262 262
-- Name: usergroup-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_usergroup
    ADD CONSTRAINT "usergroup-pk" PRIMARY KEY (id);


--
-- TOC entry 2441 (class 2606 OID 102855)
-- Dependencies: 263 263 263
-- Name: usergroupaccess-pk; Type: CONSTRAINT; Schema: dm; Owner: postgres; Tablespace:
--

ALTER TABLE dm_usergroupaccess
    ADD CONSTRAINT "usergroupaccess-pk" PRIMARY KEY (usergroupid, usrgrpid);


--
-- TOC entry 2373 (class 1259 OID 102856)
-- Dependencies: 216
-- Name: dm_indfrag1; Type: INDEX; Schema: dm; Owner: postgres; Tablespace:
--

CREATE INDEX dm_indfrag1 ON dm_fragmentattrs USING btree (id);


--
-- TOC entry 2444 (class 2606 OID 102857)
-- Dependencies: 164 2351 205
-- Name: action-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_action
    ADD CONSTRAINT "action-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2445 (class 2606 OID 102862)
-- Dependencies: 164 229 2394
-- Name: action-pluginid-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_action
    ADD CONSTRAINT "action-pluginid-fk" FOREIGN KEY (pluginid) REFERENCES dm_plugin(id);


--
-- TOC entry 2446 (class 2606 OID 102867)
-- Dependencies: 164 235 2404
-- Name: action-repository-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_action
    ADD CONSTRAINT "action-repository-fk" FOREIGN KEY (repositoryid) REFERENCES dm_repository(id);


--
-- TOC entry 2447 (class 2606 OID 102872)
-- Dependencies: 2307 164 172
-- Name: action-text-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_action
    ADD CONSTRAINT "action-text-fk" FOREIGN KEY (textid) REFERENCES dm_actiontext(id);


--
-- TOC entry 2448 (class 2606 OID 102877)
-- Dependencies: 164 165 2299
-- Name: actionaccess-action-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_actionaccess
    ADD CONSTRAINT "actionaccess-action-fk" FOREIGN KEY (actionid) REFERENCES dm_action(id);


--
-- TOC entry 2449 (class 2606 OID 102882)
-- Dependencies: 165 2438 262
-- Name: actionaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_actionaccess
    ADD CONSTRAINT "actionaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2450 (class 2606 OID 102887)
-- Dependencies: 168 2299 164
-- Name: actionfilepath_action_fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_actionfilepath
    ADD CONSTRAINT actionfilepath_action_fk FOREIGN KEY (actionid) REFERENCES dm_action(id);


--
-- TOC entry 2451 (class 2606 OID 102892)
-- Dependencies: 2426 247 168
-- Name: actionfilepath_servertype_fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_actionfilepath
    ADD CONSTRAINT actionfilepath_servertype_fk FOREIGN KEY (typeid) REFERENCES dm_servertype(id);


--
-- TOC entry 2452 (class 2606 OID 102897)
-- Dependencies: 173 2299 164
-- Name: application-action-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_application
    ADD CONSTRAINT "application-action-fk" FOREIGN KEY (actionid) REFERENCES dm_action(id);


--
-- TOC entry 2453 (class 2606 OID 102902)
-- Dependencies: 205 2351 173
-- Name: application-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_application
    ADD CONSTRAINT "application-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2454 (class 2606 OID 102907)
-- Dependencies: 2434 173 260
-- Name: application-owner-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_application
    ADD CONSTRAINT "application-owner-fk" FOREIGN KEY (ownerid) REFERENCES dm_user(id);


--
-- TOC entry 2455 (class 2606 OID 102912)
-- Dependencies: 262 2438 173
-- Name: application-owngrp-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_application
    ADD CONSTRAINT "application-owngrp-fk" FOREIGN KEY (ogrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2456 (class 2606 OID 102917)
-- Dependencies: 174 2309 173
-- Name: applicationaccess-application-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_applicationaccess
    ADD CONSTRAINT "applicationaccess-application-fk" FOREIGN KEY (appid) REFERENCES dm_application(id);


--
-- TOC entry 2457 (class 2606 OID 102922)
-- Dependencies: 174 262 2438
-- Name: applicationaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_applicationaccess
    ADD CONSTRAINT "applicationaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2458 (class 2606 OID 102927)
-- Dependencies: 175 2309 173
-- Name: applicationcomponent-application-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_applicationcomponent
    ADD CONSTRAINT "applicationcomponent-application-fk" FOREIGN KEY (appid) REFERENCES dm_application(id);


--
-- TOC entry 2459 (class 2606 OID 102932)
-- Dependencies: 175 2327 186
-- Name: applicationcomponent-component-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_applicationcomponent
    ADD CONSTRAINT "applicationcomponent-component-fk" FOREIGN KEY (compid) REFERENCES dm_component(id);


--
-- TOC entry 2460 (class 2606 OID 102937)
-- Dependencies: 2309 173 177
-- Name: applicationvars-application-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_applicationvars
    ADD CONSTRAINT "applicationvars-application-fk" FOREIGN KEY (appid) REFERENCES dm_application(id);


--
-- TOC entry 2461 (class 2606 OID 102942)
-- Dependencies: 173 2309 178
-- Name: approval-application-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_approval
    ADD CONSTRAINT "approval-application-fk" FOREIGN KEY (appid) REFERENCES dm_application(id);


--
-- TOC entry 2462 (class 2606 OID 102947)
-- Dependencies: 205 2351 178
-- Name: approval-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_approval
    ADD CONSTRAINT "approval-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2463 (class 2606 OID 102952)
-- Dependencies: 260 2434 178
-- Name: approval-user-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_approval
    ADD CONSTRAINT "approval-user-fk" FOREIGN KEY (userid) REFERENCES dm_user(id);


--
-- TOC entry 2464 (class 2606 OID 102957)
-- Dependencies: 2309 173 179
-- Name: appsinenv-appid-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_appsallowedinenv
    ADD CONSTRAINT "appsinenv-appid-fk" FOREIGN KEY (appid) REFERENCES dm_application(id);


--
-- TOC entry 2465 (class 2606 OID 102962)
-- Dependencies: 179 211 2363
-- Name: appsinenv-envid-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_appsallowedinenv
    ADD CONSTRAINT "appsinenv-envid-fk" FOREIGN KEY (envid) REFERENCES dm_environment(id);


--
-- TOC entry 2466 (class 2606 OID 102967)
-- Dependencies: 180 2309 173
-- Name: appsinenv2-application-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_appsinenv
    ADD CONSTRAINT "appsinenv2-application-fk" FOREIGN KEY (appid) REFERENCES dm_application(id);


--
-- TOC entry 2467 (class 2606 OID 102972)
-- Dependencies: 211 180 2363
-- Name: appsinenv2-environment-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_appsinenv
    ADD CONSTRAINT "appsinenv2-environment-fk" FOREIGN KEY (envid) REFERENCES dm_environment(id);


--
-- TOC entry 2468 (class 2606 OID 102977)
-- Dependencies: 2434 260 180
-- Name: appsinenv2-user-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_appsinenv
    ADD CONSTRAINT "appsinenv2-user-fk" FOREIGN KEY (modifierid) REFERENCES dm_user(id);


--
-- TOC entry 2469 (class 2606 OID 102982)
-- Dependencies: 185 188 2331
-- Name: compitemprops-componentitem-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_compitemprops
    ADD CONSTRAINT "compitemprops-componentitem-fk" FOREIGN KEY (compitemid) REFERENCES dm_componentitem(id);


--
-- TOC entry 2470 (class 2606 OID 102987)
-- Dependencies: 186 205 2351
-- Name: component-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_component
    ADD CONSTRAINT "component-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2471 (class 2606 OID 102992)
-- Dependencies: 187 186 2327
-- Name: componentaccess-server-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_componentaccess
    ADD CONSTRAINT "componentaccess-server-fk" FOREIGN KEY (compid) REFERENCES dm_component(id);


--
-- TOC entry 2472 (class 2606 OID 102997)
-- Dependencies: 2438 262 187
-- Name: componentaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_componentaccess
    ADD CONSTRAINT "componentaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2473 (class 2606 OID 103002)
-- Dependencies: 188 186 2327
-- Name: componentitem-component-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_componentitem
    ADD CONSTRAINT "componentitem-component-fk" FOREIGN KEY (compid) REFERENCES dm_component(id);


--
-- TOC entry 2474 (class 2606 OID 103007)
-- Dependencies: 188 235 2404
-- Name: componentitem-repository-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_componentitem
    ADD CONSTRAINT "componentitem-repository-fk" FOREIGN KEY (repositoryid) REFERENCES dm_repository(id);


--
-- TOC entry 2475 (class 2606 OID 103012)
-- Dependencies: 189 186 2327
-- Name: componentvars-component-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_componentvars
    ADD CONSTRAINT "componentvars-component-fk" FOREIGN KEY (compid) REFERENCES dm_component(id);


--
-- TOC entry 2476 (class 2606 OID 103017)
-- Dependencies: 2327 186 190
-- Name: compsonserv-component-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_compsallowedonserv
    ADD CONSTRAINT "compsonserv-component-fk" FOREIGN KEY (compid) REFERENCES dm_component(id);


--
-- TOC entry 2477 (class 2606 OID 103022)
-- Dependencies: 243 190 2420
-- Name: compsonserv-server-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_compsallowedonserv
    ADD CONSTRAINT "compsonserv-server-fk" FOREIGN KEY (serverid) REFERENCES dm_server(id);


--
-- TOC entry 2478 (class 2606 OID 103027)
-- Dependencies: 2327 186 191
-- Name: compsonserv2-component-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_compsonserv
    ADD CONSTRAINT "compsonserv2-component-fk" FOREIGN KEY (compid) REFERENCES dm_component(id);


--
-- TOC entry 2479 (class 2606 OID 103032)
-- Dependencies: 2420 243 191
-- Name: compsonserv2-server-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_compsonserv
    ADD CONSTRAINT "compsonserv2-server-fk" FOREIGN KEY (serverid) REFERENCES dm_server(id);


--
-- TOC entry 2480 (class 2606 OID 103037)
-- Dependencies: 2434 191 260
-- Name: compsonserv2-user-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_compsonserv
    ADD CONSTRAINT "compsonserv2-user-fk" FOREIGN KEY (modifierid) REFERENCES dm_user(id);


--
-- TOC entry 2481 (class 2606 OID 103042)
-- Dependencies: 205 192 2351
-- Name: credentials-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_credentials
    ADD CONSTRAINT "credentials-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2482 (class 2606 OID 103047)
-- Dependencies: 193 262 2438
-- Name: credentialsaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_credentialsaccess
    ADD CONSTRAINT "credentialsaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2483 (class 2606 OID 103052)
-- Dependencies: 192 194 2339
-- Name: datasource-credentials-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_datasource
    ADD CONSTRAINT "datasource-credentials-fk" FOREIGN KEY (credid) REFERENCES dm_credentials(id);


--
-- TOC entry 2484 (class 2606 OID 103057)
-- Dependencies: 2351 205 194
-- Name: datasource-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_datasource
    ADD CONSTRAINT "datasource-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2485 (class 2606 OID 103062)
-- Dependencies: 2434 260 194
-- Name: datasource-owner-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_datasource
    ADD CONSTRAINT "datasource-owner-fk" FOREIGN KEY (ownerid) REFERENCES dm_user(id);


--
-- TOC entry 2486 (class 2606 OID 103067)
-- Dependencies: 194 2438 262
-- Name: datasource-owngrp-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_datasource
    ADD CONSTRAINT "datasource-owngrp-fk" FOREIGN KEY (ogrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2487 (class 2606 OID 103072)
-- Dependencies: 233 194 2400
-- Name: datasource-providerdef-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_datasource
    ADD CONSTRAINT "datasource-providerdef-fk" FOREIGN KEY (defid) REFERENCES dm_providerdef(id);


--
-- TOC entry 2488 (class 2606 OID 103077)
-- Dependencies: 194 195 2343
-- Name: datasourceaccess-datasource-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_datasourceaccess
    ADD CONSTRAINT "datasourceaccess-datasource-fk" FOREIGN KEY (datasourceid) REFERENCES dm_datasource(id);


--
-- TOC entry 2489 (class 2606 OID 103082)
-- Dependencies: 262 195 2438
-- Name: datasourceaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_datasourceaccess
    ADD CONSTRAINT "datasourceaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2490 (class 2606 OID 103087)
-- Dependencies: 2343 194 196
-- Name: datasourceprops-datasource-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_datasourceprops
    ADD CONSTRAINT "datasourceprops-datasource-fk" FOREIGN KEY (datasourceid) REFERENCES dm_datasource(id);


--
-- TOC entry 2491 (class 2606 OID 103092)
-- Dependencies: 194 197 2343
-- Name: datasourcevars-datasource-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_datasourcevars
    ADD CONSTRAINT "datasourcevars-datasource-fk" FOREIGN KEY (datasourceid) REFERENCES dm_datasource(id);


--
-- TOC entry 2492 (class 2606 OID 103097)
-- Dependencies: 2351 205 205
-- Name: domain-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_domain
    ADD CONSTRAINT "domain-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2493 (class 2606 OID 103102)
-- Dependencies: 260 2434 205
-- Name: domain-owner-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_domain
    ADD CONSTRAINT "domain-owner-fk" FOREIGN KEY (ownerid) REFERENCES dm_user(id);


--
-- TOC entry 2494 (class 2606 OID 103107)
-- Dependencies: 205 262 2438
-- Name: domain-owngrp-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_domain
    ADD CONSTRAINT "domain-owngrp-fk" FOREIGN KEY (ogrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2495 (class 2606 OID 103112)
-- Dependencies: 206 205 2351
-- Name: domainaccess-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_domainaccess
    ADD CONSTRAINT "domainaccess-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2496 (class 2606 OID 103117)
-- Dependencies: 2438 262 206
-- Name: domainaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_domainaccess
    ADD CONSTRAINT "domainaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2497 (class 2606 OID 103122)
-- Dependencies: 205 2351 207
-- Name: domaininherit-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_domaininherit
    ADD CONSTRAINT "domaininherit-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2498 (class 2606 OID 103127)
-- Dependencies: 262 2438 207
-- Name: domaininherit-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_domaininherit
    ADD CONSTRAINT "domaininherit-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2499 (class 2606 OID 103132)
-- Dependencies: 2434 208 260
-- Name: engine-creator-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_engine
    ADD CONSTRAINT "engine-creator-fk" FOREIGN KEY (creatorid) REFERENCES dm_user(id);


--
-- TOC entry 2500 (class 2606 OID 103137)
-- Dependencies: 208 260 2434
-- Name: engine-modifier-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_engine
    ADD CONSTRAINT "engine-modifier-fk" FOREIGN KEY (modifierid) REFERENCES dm_user(id);


--
-- TOC entry 2501 (class 2606 OID 103142)
-- Dependencies: 2357 208 209
-- Name: engineaccess-engine-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_engineaccess
    ADD CONSTRAINT "engineaccess-engine-fk" FOREIGN KEY (engineid) REFERENCES dm_engine(id);


--
-- TOC entry 2502 (class 2606 OID 103147)
-- Dependencies: 209 262 2438
-- Name: engineaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_engineaccess
    ADD CONSTRAINT "engineaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2503 (class 2606 OID 103152)
-- Dependencies: 210 208 2357
-- Name: engineconfig-engine-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_engineconfig
    ADD CONSTRAINT "engineconfig-engine-fk" FOREIGN KEY (engineid) REFERENCES dm_engine(id);


--
-- TOC entry 2504 (class 2606 OID 103157)
-- Dependencies: 211 192 2339
-- Name: environment-credentials-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_environment
    ADD CONSTRAINT "environment-credentials-fk" FOREIGN KEY (credid) REFERENCES dm_credentials(id);


--
-- TOC entry 2505 (class 2606 OID 103162)
-- Dependencies: 262 211 2438
-- Name: environment-ogrpid-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_environment
    ADD CONSTRAINT "environment-ogrpid-fk" FOREIGN KEY (ogrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2506 (class 2606 OID 103167)
-- Dependencies: 2434 211 260
-- Name: environment-ownerid-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_environment
    ADD CONSTRAINT "environment-ownerid-fk" FOREIGN KEY (ownerid) REFERENCES dm_user(id);


--
-- TOC entry 2507 (class 2606 OID 103172)
-- Dependencies: 211 212 2363
-- Name: environmentaccess-environment-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_environmentaccess
    ADD CONSTRAINT "environmentaccess-environment-fk" FOREIGN KEY (envid) REFERENCES dm_environment(id);


--
-- TOC entry 2508 (class 2606 OID 103177)
-- Dependencies: 212 262 2438
-- Name: environmentaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_environmentaccess
    ADD CONSTRAINT "environmentaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2509 (class 2606 OID 103182)
-- Dependencies: 211 213 2363
-- Name: environmentvars-environment-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_environmentvars
    ADD CONSTRAINT "environmentvars-environment-fk" FOREIGN KEY (envid) REFERENCES dm_environment(id);


--
-- TOC entry 2510 (class 2606 OID 103187)
-- Dependencies: 2299 164 214
-- Name: field-action-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_field
    ADD CONSTRAINT "field-action-fk" FOREIGN KEY (actionid) REFERENCES dm_action(id);


--
-- TOC entry 2511 (class 2606 OID 103192)
-- Dependencies: 214 2343 194
-- Name: field-datasource-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_field
    ADD CONSTRAINT "field-datasource-fk" FOREIGN KEY (datasourceid) REFERENCES dm_datasource(id);


--
-- TOC entry 2512 (class 2606 OID 103197)
-- Dependencies: 214 2351 205
-- Name: field-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_field
    ADD CONSTRAINT "field-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2513 (class 2606 OID 103202)
-- Dependencies: 2367 215 214
-- Name: fieldvalues-field-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_fieldvalues
    ADD CONSTRAINT "fieldvalues-field-fk" FOREIGN KEY (fieldid) REFERENCES dm_field(id);


--
-- TOC entry 2514 (class 2606 OID 103207)
-- Dependencies: 2339 192 224
-- Name: notify-credentials-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_notify
    ADD CONSTRAINT "notify-credentials-fk" FOREIGN KEY (credid) REFERENCES dm_credentials(id);


--
-- TOC entry 2515 (class 2606 OID 103212)
-- Dependencies: 2351 205 224
-- Name: notify-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_notify
    ADD CONSTRAINT "notify-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2516 (class 2606 OID 103217)
-- Dependencies: 260 2434 224
-- Name: notify-owner-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_notify
    ADD CONSTRAINT "notify-owner-fk" FOREIGN KEY (ownerid) REFERENCES dm_user(id);


--
-- TOC entry 2517 (class 2606 OID 103222)
-- Dependencies: 2438 262 224
-- Name: notify-owngrp-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_notify
    ADD CONSTRAINT "notify-owngrp-fk" FOREIGN KEY (ogrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2518 (class 2606 OID 103227)
-- Dependencies: 233 2400 224
-- Name: notify-providerdef-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_notify
    ADD CONSTRAINT "notify-providerdef-fk" FOREIGN KEY (defid) REFERENCES dm_providerdef(id);


--
-- TOC entry 2519 (class 2606 OID 103232)
-- Dependencies: 225 224 2384
-- Name: notifyaccess-source-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_notifyaccess
    ADD CONSTRAINT "notifyaccess-source-fk" FOREIGN KEY (notifyid) REFERENCES dm_notify(id);


--
-- TOC entry 2520 (class 2606 OID 103237)
-- Dependencies: 225 2438 262
-- Name: notifyaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_notifyaccess
    ADD CONSTRAINT "notifyaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2521 (class 2606 OID 103242)
-- Dependencies: 226 224 2384
-- Name: notifyprops-notify-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_notifyprops
    ADD CONSTRAINT "notifyprops-notify-fk" FOREIGN KEY (notifyid) REFERENCES dm_notify(id);


--
-- TOC entry 2522 (class 2606 OID 103247)
-- Dependencies: 224 228 2384
-- Name: notifyvars-notify-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_notifyvars
    ADD CONSTRAINT "notifyvars-notify-fk" FOREIGN KEY (notifyid) REFERENCES dm_notify(id);


--
-- TOC entry 2523 (class 2606 OID 103252)
-- Dependencies: 231 262 2438
-- Name: privileges-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_privileges
    ADD CONSTRAINT "privileges-usergroup-fk" FOREIGN KEY (groupid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2524 (class 2606 OID 103257)
-- Dependencies: 232 2400 233
-- Name: propertydef-providerdef-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_propertydef
    ADD CONSTRAINT "propertydef-providerdef-fk" FOREIGN KEY (defid) REFERENCES dm_providerdef(id);


--
-- TOC entry 2525 (class 2606 OID 103262)
-- Dependencies: 2394 229 233
-- Name: providerdef-plugin-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_providerdef
    ADD CONSTRAINT "providerdef-plugin-fk" FOREIGN KEY (pluginid) REFERENCES dm_plugin(id);


--
-- TOC entry 2526 (class 2606 OID 103267)
-- Dependencies: 234 2400 233
-- Name: providerdefprops-providerdef-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_providerdefprops
    ADD CONSTRAINT "providerdefprops-providerdef-fk" FOREIGN KEY (defid) REFERENCES dm_providerdef(id);


--
-- TOC entry 2527 (class 2606 OID 103272)
-- Dependencies: 235 192 2339
-- Name: repository-credentials-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repository
    ADD CONSTRAINT "repository-credentials-fk" FOREIGN KEY (credid) REFERENCES dm_credentials(id);


--
-- TOC entry 2528 (class 2606 OID 103277)
-- Dependencies: 235 2351 205
-- Name: repository-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repository
    ADD CONSTRAINT "repository-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2529 (class 2606 OID 103282)
-- Dependencies: 2400 233 235
-- Name: repository-providerdef-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repository
    ADD CONSTRAINT "repository-providerdef-fk" FOREIGN KEY (defid) REFERENCES dm_providerdef(id);


--
-- TOC entry 2532 (class 2606 OID 103287)
-- Dependencies: 235 236 2404
-- Name: repositoryaccess-repository-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repositoryaccess
    ADD CONSTRAINT "repositoryaccess-repository-fk" FOREIGN KEY (repositoryid) REFERENCES dm_repository(id);


--
-- TOC entry 2533 (class 2606 OID 103292)
-- Dependencies: 2438 262 236
-- Name: repositoryaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repositoryaccess
    ADD CONSTRAINT "repositoryaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2535 (class 2606 OID 103297)
-- Dependencies: 238 2404 235
-- Name: repositoryprops-repository-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repositoryprops
    ADD CONSTRAINT "repositoryprops-repository-fk" FOREIGN KEY (repositoryid) REFERENCES dm_repository(id);


--
-- TOC entry 2536 (class 2606 OID 103302)
-- Dependencies: 2404 235 239
-- Name: repositorytextpatterns-repository-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repositorytextpattern
    ADD CONSTRAINT "repositorytextpatterns-repository-fk" FOREIGN KEY (repositoryid) REFERENCES dm_repository(id);


--
-- TOC entry 2537 (class 2606 OID 103307)
-- Dependencies: 240 235 2404
-- Name: repositoryvars-repository-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repositoryvars
    ADD CONSTRAINT "repositoryvars-repository-fk" FOREIGN KEY (repositoryid) REFERENCES dm_repository(id);


--
-- TOC entry 2538 (class 2606 OID 103312)
-- Dependencies: 241 183 2323
-- Name: request-calendar-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_request
    ADD CONSTRAINT "request-calendar-fk" FOREIGN KEY (calendarid) REFERENCES dm_calendar(id);


--
-- TOC entry 2539 (class 2606 OID 103317)
-- Dependencies: 241 250 2430
-- Name: request-task-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_request
    ADD CONSTRAINT "request-task-fk" FOREIGN KEY (taskid) REFERENCES dm_task(id);


--
-- TOC entry 2540 (class 2606 OID 103322)
-- Dependencies: 260 2434 241
-- Name: request-user-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_request
    ADD CONSTRAINT "request-user-fk" FOREIGN KEY (userid) REFERENCES dm_user(id);


--
-- TOC entry 2530 (class 2606 OID 103327)
-- Dependencies: 260 235 2434
-- Name: respository-owner-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repository
    ADD CONSTRAINT "respository-owner-fk" FOREIGN KEY (ownerid) REFERENCES dm_user(id);


--
-- TOC entry 2531 (class 2606 OID 103332)
-- Dependencies: 2438 235 262
-- Name: respository-owngrp-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repository
    ADD CONSTRAINT "respository-owngrp-fk" FOREIGN KEY (ogrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2534 (class 2606 OID 103337)
-- Dependencies: 237 233 2400
-- Name: respositoryignore-providerdef-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_repositoryignore
    ADD CONSTRAINT "respositoryignore-providerdef-fk" FOREIGN KEY (defid) REFERENCES dm_providerdef(id);


--
-- TOC entry 2541 (class 2606 OID 103342)
-- Dependencies: 243 2339 192
-- Name: server-credentials-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_server
    ADD CONSTRAINT "server-credentials-fk" FOREIGN KEY (credid) REFERENCES dm_credentials(id);


--
-- TOC entry 2542 (class 2606 OID 103347)
-- Dependencies: 2438 262 243
-- Name: server-ogrpid-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_server
    ADD CONSTRAINT "server-ogrpid-fk" FOREIGN KEY (ogrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2543 (class 2606 OID 103352)
-- Dependencies: 260 243 2434
-- Name: server-ownerid-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_server
    ADD CONSTRAINT "server-ownerid-fk" FOREIGN KEY (ownerid) REFERENCES dm_user(id);


--
-- TOC entry 2544 (class 2606 OID 103357)
-- Dependencies: 243 247 2426
-- Name: server-servertype-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_server
    ADD CONSTRAINT "server-servertype-fk" FOREIGN KEY (typeid) REFERENCES dm_servertype(id);


--
-- TOC entry 2545 (class 2606 OID 103362)
-- Dependencies: 2420 243 245
-- Name: serveraccess-server-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_serveraccess
    ADD CONSTRAINT "serveraccess-server-fk" FOREIGN KEY (serverid) REFERENCES dm_server(id);


--
-- TOC entry 2546 (class 2606 OID 103367)
-- Dependencies: 262 2438 245
-- Name: serveraccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_serveraccess
    ADD CONSTRAINT "serveraccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2547 (class 2606 OID 103372)
-- Dependencies: 246 211 2363
-- Name: serversinenv-envid-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_serversinenv
    ADD CONSTRAINT "serversinenv-envid-fk" FOREIGN KEY (envid) REFERENCES dm_environment(id);


--
-- TOC entry 2548 (class 2606 OID 103377)
-- Dependencies: 2420 246 243
-- Name: serversinenv-serverid-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_serversinenv
    ADD CONSTRAINT "serversinenv-serverid-fk" FOREIGN KEY (serverid) REFERENCES dm_server(id);


--
-- TOC entry 2549 (class 2606 OID 103382)
-- Dependencies: 243 248 2420
-- Name: servervars-server-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_servervars
    ADD CONSTRAINT "servervars-server-fk" FOREIGN KEY (serverid) REFERENCES dm_server(id);


--
-- TOC entry 2550 (class 2606 OID 103387)
-- Dependencies: 2351 254 205
-- Name: taskcreateversion-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_taskcreateversion
    ADD CONSTRAINT "taskcreateversion-domain-fk" FOREIGN KEY (targetdomain) REFERENCES dm_domain(id);


--
-- TOC entry 2551 (class 2606 OID 103392)
-- Dependencies: 250 2430 254
-- Name: taskcreateversion-task-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_taskcreateversion
    ADD CONSTRAINT "taskcreateversion-task-fk" FOREIGN KEY (id) REFERENCES dm_task(id);


--
-- TOC entry 2552 (class 2606 OID 103397)
-- Dependencies: 205 260 2351
-- Name: user-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_user
    ADD CONSTRAINT "user-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2553 (class 2606 OID 103402)
-- Dependencies: 261 2434 260
-- Name: useraccess-engine-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_useraccess
    ADD CONSTRAINT "useraccess-engine-fk" FOREIGN KEY (userid) REFERENCES dm_user(id);


--
-- TOC entry 2554 (class 2606 OID 103407)
-- Dependencies: 262 2438 261
-- Name: useraccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_useraccess
    ADD CONSTRAINT "useraccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2555 (class 2606 OID 103412)
-- Dependencies: 2351 205 262
-- Name: usergroup-domain-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_usergroup
    ADD CONSTRAINT "usergroup-domain-fk" FOREIGN KEY (domainid) REFERENCES dm_domain(id);


--
-- TOC entry 2556 (class 2606 OID 103417)
-- Dependencies: 260 263 2434
-- Name: usergroupaccess-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_usergroupaccess
    ADD CONSTRAINT "usergroupaccess-usergroup-fk" FOREIGN KEY (usergroupid) REFERENCES dm_user(id);


--
-- TOC entry 2557 (class 2606 OID 103422)
-- Dependencies: 262 2438 263
-- Name: usergroupaccess-usrgrp-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_usergroupaccess
    ADD CONSTRAINT "usergroupaccess-usrgrp-fk" FOREIGN KEY (usrgrpid) REFERENCES dm_usergroup(id);


--
-- TOC entry 2558 (class 2606 OID 103427)
-- Dependencies: 2434 264 260
-- Name: usersingroup-user-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_usersingroup
    ADD CONSTRAINT "usersingroup-user-fk" FOREIGN KEY (userid) REFERENCES dm_user(id);


--
-- TOC entry 2559 (class 2606 OID 103432)
-- Dependencies: 2438 264 262
-- Name: usersingroup-usergroup-fk; Type: FK CONSTRAINT; Schema: dm; Owner: postgres
--

ALTER TABLE dm_usersingroup
    ADD CONSTRAINT "usersingroup-usergroup-fk" FOREIGN KEY (groupid) REFERENCES dm_usergroup(id);


-- Completed on 2014-07-15 14:53:29

--
-- PostgreSQL database dump complete
--
