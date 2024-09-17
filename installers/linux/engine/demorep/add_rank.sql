alter table users add column rank integer;

update users set rank=1 where surname='Gates';
update users set rank=2 where surname='Ellison';
update users set rank=3 where surname='McNealy';
update users set rank=4 where surname='Torvalds';
update users set rank=5 where surname='Jobs';
