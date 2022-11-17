-- modify dm_fragmenttext to fix quoting issue
UPDATE dm_fragmenttext SET data='using component "@component@" {' || chr(10) ||
'  deploy;' || chr(10) ||
'}' WHERE fragmentid=10;