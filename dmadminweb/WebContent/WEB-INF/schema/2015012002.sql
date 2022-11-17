-- modify dm_fragmenttext to fix quoting issue
UPDATE dm_fragmenttext SET data='using component "@component@" {
  deploy;
}' WHERE fragmentid=10;