update dm.dm_fragmenttext set data='if (@expression@) {
    @1@
}
#if 2
else {
    @2@
}
#endif' where fragmentid=60;

