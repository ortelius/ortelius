function sieve()
{
  for(iter = 1; $iter <= 10; iter = $iter + 1) {
    set count = 0;

    for(i = 0; $i <= 8190; i = $i + 1) {
      set flags[$i] = true;
    }

    for(i = 0; $i <= 8190; i = $i + 1)  {
      if($flags[$i]) {
        set prime = $i + $i + 3;
        set k = $i + $prime;

        while($k <= 8190) {
          set flags[$k] = false;
          set k = $k + $prime;
        }

        set count = $count + 1;
      }
    }
  }

  return $count;
}


action doSieve
{
  echo "sieve -  Sieve benchmark, \$Revision: 1.1 \$";

  psloop {
    set -g ans = sieve();
  }

  if($ans != 1899) {
    echo "Sieve result wrong, ans = $ans, expected 1899";
    return;
 }
}
