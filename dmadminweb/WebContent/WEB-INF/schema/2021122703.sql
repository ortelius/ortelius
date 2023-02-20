CREATE TABLE IF NOT EXISTS dm_textfile
(
compid integer,
filetype varchar(20),
lineno integer,
base64str varchar(80),
primary key(compid, filetype, lineno)
);
