update dm.dm_action set filepath='${DMHOME}\scripts\extractzip.${EXT}' where name='extractzip';
update dm.dm_fragment_categories set categoryid=22 where id = (select id from dm.dm_fragments where name='Extract Zip');
