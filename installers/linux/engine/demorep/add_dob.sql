alter table users add column dob date;

update users set dob=to_date('1955-10-28','YYYY-MM-DD') where surname='Gates';
update users set dob=to_date('1944-08-17','YYYY-MM-DD') where surname='Ellison';
update users set dob=to_date('1954-11-13','YYYY-MM-DD') where surname='McNealy';
update users set dob=to_date('1969-12-28','YYYY-MM-DD') where surname='Torvalds';
update users set dob=to_date('1955-02-24','YYYY-MM-DD') where surname='Jobs';
