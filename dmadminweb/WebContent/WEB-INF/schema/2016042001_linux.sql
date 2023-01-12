update dm.dm_server set typeid=1,protocol='sftp',basedir=replace(basedir,'\','/') where id in (1,2);
update dm.dm_repositoryprops set value=replace(value,'\','/') where repositoryid=1 and name='filepath';
