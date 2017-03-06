 
ALTER TABLE dm.dm_type ADD domainid integer;
ALTER TABLE dm.dm_type ADD status character varying(1);
ALTER TABLE dm.dm_type ADD ownerid integer;
ALTER TABLE dm.dm_type ADD creatorid integer;
ALTER TABLE dm.dm_type ADD created integer;
ALTER TABLE dm.dm_type ADD modifierid integer;
ALTER TABLE dm.dm_type ADD modified integer;

ALTER TABLE ONLY dm.dm_type
    ADD CONSTRAINT "type-pk" PRIMARY KEY (id);
    
ALTER TABLE ONLY dm.dm_typeaccess
    ADD CONSTRAINT "typeaccess-pk" PRIMARY KEY (comptypeid, usrgrpid);
    

ALTER TABLE ONLY dm.dm_typeaccess
    ADD CONSTRAINT "typeaccess-comptype-fk" FOREIGN KEY (comptypeid) REFERENCES dm.dm_type (id) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE NO ACTION;   
      
ALTER TABLE ONLY dm.dm_typeaccess
    ADD CONSTRAINT "typeaccess-usergroup-fk" FOREIGN KEY (usrgrpid) REFERENCES dm.dm_usergroup (id) MATCH SIMPLE ON UPDATE NO ACTION ON DELETE NO ACTION);

    ALTER TABLE ONLY dm.dm_type
    ADD CONSTRAINT "type-creator-fk" FOREIGN KEY (creatorid) REFERENCES dm_user(id);

ALTER TABLE ONLY dm.dm_type
    ADD CONSTRAINT "type-modifier-fk" FOREIGN KEY (modifierid) REFERENCES dm_user(id);

  
update dm.dm_type set domainid = 1;
update dm.dm_type set status = 'N';
update dm.dm_type set creatorid = 1;
update dm.dm_type set modifierid = 1;

insert into dm_privileges values (2,67) ;
   
