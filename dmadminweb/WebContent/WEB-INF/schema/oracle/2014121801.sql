ALTER TABLE dm_repositoryaccess DROP CONSTRAINT "repositoryaccess-pk";

ALTER TABLE dm_repositoryaccess
    ADD CONSTRAINT "repositoryaccess-pk" PRIMARY KEY (repositoryid, usrgrpid);

update dm_domainaccess set updateaccess = 'Y' where domainid = 1;

update dm_fragmenttext set data = 'using component "@component@" { deploy; }' where fragmentid = 10;
