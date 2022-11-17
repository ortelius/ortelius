
CREATE TABLE dm.dm_typeaccess (
  comptypeid integer NOT NULL,
  usrgrpid integer NOT NULL,
  viewaccess character(1),
  updateaccess character(1),
  readaccess character(1),
  writeaccess character(1)
);

