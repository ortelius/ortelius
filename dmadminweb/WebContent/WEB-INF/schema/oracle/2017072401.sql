insert into dm.dm_providerdef(id,name,kind,kindstr) values(22,'hipchat',9,'notify');
insert into dm.dm_providerdef(id,name,kind,kindstr) values(23,'slack',9,'notify');

insert into dm.dm_propertydef(defid,name,required,appendable) values(22,'webhook','Y','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(22,'logfile','N','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(22,'HTML','N','N');

insert into dm.dm_propertydef(defid,name,required,appendable) values(23,'webhook','Y','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(23,'channel','Y','N');
insert into dm.dm_propertydef(defid,name,required,appendable) values(23,'logfile','N','N');
