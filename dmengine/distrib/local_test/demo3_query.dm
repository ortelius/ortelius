action demo3
{
  query(datasource: 'harvest', sql: 'select * from haruser', result: 'row') {
    echo "${row['username']}, ${row['realname']}";
  }
}
