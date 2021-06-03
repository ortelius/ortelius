CREATE TABLE IF NOT EXISTS dm_textfile
(
 compid integer primary key,
 filetype varchar(20),
 lineno integer,
 base64str varchar(80)
);
