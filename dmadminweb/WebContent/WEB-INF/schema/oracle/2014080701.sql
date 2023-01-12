insert into dm_category(id,name) values(20,'Database');

insert into dm_action(id,name,filepath,summary,domainid,function,graphical,ownerid,status,copy,kind,categoryid)
        values(14,'RunSQLScript','${DMHOME}\scripts\applysql.${EXT}','Runs one or more SQL scripts',
		1,'N','N',1,'N','Y',4, 20);

insert into dm_fragments(id,name,summary,categoryid,exitpoints,drilldown,actionid)
	values (295,'Run SQL Script (Postgres)','Runs one or more SQL scripts against a Postgres Database',20,1,'N',14);

insert into dm_fragmentattrs(id, typeid, attype, atname, inherit, atorder)
	values(2951, 295, 'entry', 'SQLScript', 'N', 1);

insert into dm_actionarg(actionid,name,type,outpos,required,switch,pad,inpos,switchmode,negswitch)
	values(14,'arg1','false',1,'N','postgres','Y',NULL,'A','');

insert into dm_actionarg(actionid,name,type,outpos,required,switch,pad,inpos,switchmode,negswitch)
	values(14,'arg2','false',2,'N','${POSTGRES_HOME}','Y',NULL,'A','');

insert into dm_actionarg(actionid,name,type,outpos,required,switch,pad,inpos,switchmode,negswitch)
	values(14,'arg3','false',3,'N','${POSTGRES_DB}','Y',NULL,'A','');

insert into dm_actionarg(actionid,name,type,outpos,required,switch,pad,inpos,switchmode,negswitch)
	values(14,'arg4','false',4,'N','${POSTGRES_USER}','Y',NULL,'A','');

insert into dm_actionarg(actionid,name,type,outpos,required,switch,pad,inpos,switchmode,negswitch)
	values(14,'arg5','false',5,'N','${POSTGRES_PASS}','Y',NULL,'A','');

insert into dm_actionarg(actionid,name,type,outpos,required,switch,pad,inpos,switchmode,negswitch)
	values(14,'SQLScript','Entry',6,'Y',NULL,'N',1,NULL,NULL);
