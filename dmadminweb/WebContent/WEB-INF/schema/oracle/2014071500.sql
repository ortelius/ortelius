--
-- Oracle Create schema user
--

--

CREATE USER dm IDENTIFIED BY dm;
ALTER USER dm QUOTA UNLIMITED ON system;
GRANT create session TO dm;
GRANT create table TO dm;
GRANT create view TO dm;
GRANT create any trigger TO dm;
GRANT create any procedure TO dm;
GRANT create sequence TO dm;
GRANT create synonym TO dm;
ALTER SESSION SET current_schema=dm;


