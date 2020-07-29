update dm.dm_actionarg set name = 'serviceaction' where name = 'action' and actionid = 3;
update dm.dm_fragmenttext set data = 'servicectl(serviceaction: ''stop'', servicename: "@servicename@");' where fragmentid = 80;
update dm.dm_fragmenttext set data = 'servicectl(serviceaction: ''start'', servicename: "@servicename@");' where fragmentid = 90;
update dm.dm_fragmentattrs set atname = 'servicename' where atname = 'Service Name' and typeid = 80;
update dm.dm_fragmentattrs set atname = 'servicename' where atname = 'Service Name' and typeid = 90;