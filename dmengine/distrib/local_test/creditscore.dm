action DeployCreditScore
{
  // avloop

  /**
   * Starting point - base application and V3 should be on stack
   */
  test(name: "Application stack") {
    assert(str1: ${application.name}, str2: 'CreditScore;3');
    assert(str1: $av_base, str2: 'base');	// base variable
    assert(str1: $av, str2: '3');		// V3 variable
    assert(str1: $av_3, str2: '3');		// V3 only variable
    psloop {
      set slist = "${slist}${slist:+','}${server.name}";
    }
    assert(str1: $slist, str2: 'server1,server2');
  }

  /**
   * Forwards - using selected target $application
   */
  test(name: "Rollforward from 'CreditScore;1' to '${application.name}'") {
    avloop(from: 'CreditScore;1') {
      //echo "\t${application.name}";
      set -g favlist1 = "${favlist1}${favlist1:+','}${av}";	// should not need -g - is AV not closed?
    }
    assert(str1: $favlist1, str2: '1,2,3');
  }

  /**
   * Make sure stack looks the same afterwards
   */
  test(name: "Application stack 2") {
    assert(str1: ${application.name}, str2: 'CreditScore;3');
    assert(str1: $av_base, str2: 'base');	// base variable
    assert(str1: $av, str2: '3');		// V3 variable
    assert(str1: $av_3, str2: '3');		// V3 only variable
  }

  /*
   * Forwards using two fixed versions - test AV scopes being pushed
   */
  test(name: "Rollforward from 'CreditScore;1' to 'CreditScore;3'") {
    avloop(from: 'CreditScore;1', to: 'CreditScore;3') {
      //echo "\t${application.name}";
      set -g favlist2 = "${favlist2}${favlist2:+','}${av}";
      if(${application.name} != 'CreditScore;3') {
      	assert(str1: $av_3, str2: '');
      } else {
      	assert(str1: $av_3, str2: '3');
      }
      psloop {
      	//echo "${av}:${server.name}";
        set -g slist2 = "${slist2}${slist2:+','}${av}:${server.name}";	// should not need -g - is AV not closed?
      }
    }
    assert(str1: $favlist2, str2: '1,2,3');
    assert(str1: $slist2, str2: '1:server1,1:server2,2:server1,2:server2,3:server1,3:server2');
  }

  /*
   * Forwards down other path using two fixed versions - test first and last
   */
  test(name: "Rollforward from 'CreditScore;1' to 'CreditScore;6'") {
    avloop(from: 'CreditScore;1', to: 'CreditScore;6') {
      //echo "\t${application.name}";
      set -g favlist3 = "${favlist3}${favlist3:+','}${av}";
      assert(str1: ${avloop.first.name}, str2: 'CreditScore;1');
      assert(str1: ${avloop.last.name}, str2: 'CreditScore;6');
      assert(expr: ${avloop.first} < ${avloop.last});
    }
    assert(str1: $favlist3, str2: '1,4,5,6');
  }

  /*
   * Forwards from nothing to target $application
   */
  test(name: "Rollforward from nothing to '${application.name}'") {
    avloop(from: null) {
      //echo "\t${application.name}";
      set -g favlist4 = "${favlist4}${favlist4:+','}${av}";
    }
    assert(str1: $favlist4, str2: '1,2,3');
  }

  /*
   * Forwards from what is on server to target $application
   * DB should contain CreditScore;1 as current version
   */
  test(name: "Rollforward from nothing to '${application.name}'") {
    avloop {
      //echo "\t${application.name}";
      set -g favlist5 = "${favlist5}${favlist5:+','}${av}";
    }
    assert(str1: $favlist5, str2: '1,2,3');
  }

  test(name: "Rollback from 'CreditScore;3' to 'CreditScore;1'") {
    avloop(from: 'CreditScore;3', to: 'CreditScore;1') {
      //echo "\t${application.name}";
      set -g bavlist1 = "${bavlist1}${bavlist1:+','}${av}";
      assert(expr: ${avloop.first} > ${avloop.last});
    }
    assert(str1: $bavlist1, str2: '3,2,1');
  }

  test(name: "Rollsideways from 'CreditScore;3' to 'CreditScore;6'") {
    try {
      avloop(from: 'CreditScore;3', to: 'CreditScore;6') {
        //echo "\t${application.name}";
      }
      assert(expr: false);
    } catch(e) {
      //echo $e;
      assert(str1: $e, str2: "There is no path between 'CreditScore;3' and 'CreditScore;6'");
    }
  }

  // AV arithmetic
  test(name: "Predecessor tests - without avloop") {
    assert(str1: ${application.predecessor.name}, str2: 'CreditScore;2');
    assert(str1: ${application.predecessor.predecessor.name}, str2: 'CreditScore;1');
    set cs2 = ${application.predecessor};
    set cs1 = ${application.predecessor.predecessor};
    assert(obj1: $application - 1, obj2: $cs2);
    assert(obj1: $application - 2, obj2: $cs1);
    assert(obj1: $cs2 - 1, obj2: $cs1);
    assert(obj1: $cs1 - 1, obj2: null);
    assert(obj1: $cs2 + 1, obj2: $application);
    try {
      set temp = $cs1 + 1;
      assert(expr: false);
    } catch(e) {
      assert(str1: $e, str2: 'Successor to application version 303 is ambiguous');
    }
  }

  test(name: "Predecessor tests - within avloop") {
    avloop(from: 'CreditScore;1', to: 'CreditScore;3') {
      switch(${application.name}) {
      case 'CreditScore;1':
        assert(obj1: ${application.predecessor}, obj2: null);
        assert(obj1: ${avloop.last} - 2, obj2: ${application});
        break;
      case 'CreditScore;2':
      	assert(str1: ${application.predecessor.name}, str2: 'CreditScore;1');
      	assert(obj1: ${application.predecessor}, obj2: ${avloop.first});
        assert(obj1: ${avloop.last}  - 1, obj2: ${application});
        assert(obj1: ${avloop.first} + 1, obj2: ${application});	// this is ambiguous outside avloop
      	break;
      case 'CreditScore;3':
      	assert(str1: ${application.predecessor.name}, str2: 'CreditScore;2');
        assert(obj1: ${application.predecessor}, obj2: ${avloop.last.predecessor});
        assert(obj1: ${application.predecessor} + 1, obj2: ${application});
        assert(obj1: ${avloop.first} + 2, obj2: ${application});	// this is ambiguous outside avloop
        break;
      }
    }
  }

  // comploop

  test(name: "Comploop on 'CreditScore;3' no filter") {
    comploop {
      //echo "\t${application.name} - ${component.name} (${component.rollup}, ${component.rollback})";
      set -g clist1 = "${clist1}${clist1:+','}${component.name}";
    }
    assert(str1: $clist1, str2: 'CreditScoreDB;3');
  }

  test(name: "Comploop on 'CreditScore;3' rollup") {
    comploop(rollback: false) {
      //echo "\t${application.name} - ${component.name} (${component.rollup}, ${component.rollback})";
      set -g clist2 = "${clist2}${clist2:+','}${component.name}";
    }
    assert(str1: $clist2, str2: 'CreditScoreDB;3');
  }

  test(name: "Comploop on 'CreditScore;3' rollback") {
    comploop(rollback: true) {
      //echo "\t${application.name} - ${component.name} (${component.rollup}, ${component.rollback})";
      set -g clist3 = "${clist3}${clist3:+','}${component.name}";
    }
    assert(str1: $clist3, str2: 'CreditScoreDB;3');
  }

  test(name: "Comploop on rollup from nothing to 'CreditScore;3'") {
    avloop(from: null, to: 'CreditScore;3') {
      comploop {
        //echo "\t${application.name} - ${component.name} (${component.rollup}, ${component.rollback})";
        set -g avlist6 = "${avlist6}${avlist6:+','}${av}";
        set -g clist4 = "${clist4}${clist4:+','}${component.name}";
      }
    }
    assert(str1: $avlist6, str2: '1,2,3,3');
    assert(str1: $clist4, str2: 'CreditScoreDB;1,CreditScoreDB_rollup,CreditScoreDB;3,CreditScoreUI;3');
  }

  test(name: "Comploop on rollup from 'CreditScore;1' to 'CreditScore;3'") {
    avloop(from: 'CreditScore;1', to: 'CreditScore;3') {
      comploop {
        //echo "\t${application.name} - ${component.name} (${component.rollup}, ${component.rollback})";
        set -g avlist7 = "${avlist7}${avlist7:+','}${av}";
        set -g clist5 = "${clist5}${clist5:+','}${component.name}";
      }
    }
    assert(str1: $avlist7, str2: '2,3,3');
    assert(str1: $clist5, str2: 'CreditScoreDB_rollup,CreditScoreDB;3,CreditScoreUI;3');
  }

  test(name: "Comploop on rollback from 'CreditScore;3' to 'CreditScore;1'") {
    avloop(from: 'CreditScore;3', to: 'CreditScore;1') {
      comploop {
        //echo "\t${application.name} - ${component.name} (${component.rollup}, ${component.rollback})";
        set -g avlist8 = "${avlist8}${avlist8:+','}${av}";
        set -g clist6 = "${clist6}${clist6:+','}${component.name}";
      }
    }
    assert(str1: $avlist8, str2: '3,2,1');
    assert(str1: $clist6, str2: 'CreditScoreDB;3,CreditScoreDB_rollback,CreditScoreUI;1');
  }

  test(name: "Comploop on rollup from 'CreditScore;1' to 'CreditScore;3' with filterlast true") {
    avloop(from: 'CreditScore;1', to: 'CreditScore;3') {
      comploop(filterlast: true) {
        //echo "\t${application.name} - ${component.name} (${component.rollup}, ${component.rollback})";
        set -g avlist9 = "${avlist9}${avlist9:+','}${av}";
        set -g clist7 = "${clist7}${clist7:+','}${component.name}";
      }
    }
    assert(str1: $avlist9, str2: '2,3');
    assert(str1: $clist7, str2: 'CreditScoreDB_rollup,CreditScoreDB;3');
  }

  // Component Items

  test(name: "Component Items for rollup from nothing to 'CreditScore;3'") {
    avloop(from: null, to: 'CreditScore;3') {
      comploop {
      	foreach(ci: ${component.items}) {
      	  set item = ${component.items}[$ci];
          /**/echo "\t${application.name} - ${component.name} - ${item.name} (${item.rollup}, ${item.rollback})";
          set -g cilist1 = "${cilist1}${cilist1:+','}${item.name}";
        }
      }
    }
    assert(str1: $cilist1, str2: 'SQL Files;1,Apply Script;1,SQL Rollup Files;2,Apply Script;3,SQL Rollback Files;3,SQL Rollup Files;3,WAR File;3');
  }

  test(name: "Deploy rollup from 'CreditScore;1' to 'CreditScore;3'") {
    avloop(from: null) {
      comploop {
        deploy;
      }
    }
  }

  if(${global.test_failures} != 0) {
    echo "***** Test report: ${global.test_failures} out of ${global.test_total} test(s) failed!";
  } else {
    echo "***** Test report: ${global.test_total} tests executed.  All tests passed!";
  }
}


action Wibble
{
  // Deploy rollup/rollback type changes
  avloop(from: 'CreditScore;1') {
    comploop(filterlast: true) {	// true/false*
      deploy(filterlast: true);
    }
  }

  // Deploy new version
  comploop(rollback: true) {		// true/false*
    deploy(rollback: true);		// true/false*
  }
}



/*
  deploy(application: $application);
  using application ?

  if(${component.rollup}) {} - DONE
  avloop(rollback: false) - don't allow rollback
  avloop(rollback: true) - don't allow rollup
  avloop(nofilter: true) - don't automatically filter comploops or comp items
  avloop and comploop will have to have a stack frames like action - no, use context - DONE
        context should contain direction (rollback true/false) and islast flag (true/false)
        plus look at whether avloop on stack to see if islast should be ignored
  naming of branches

CreditScore;1    CreditScore;2
WAR;1		 WAR;2
SQL;1		 SQL;3

component: 'SQL'

Component Filtering:
ROLLBACK - only include if rolling back
ROLLUP   - only include if rolling up
NOT SET  - include in all deploys
What if you want to include in BOTH an ordinary deploy AND a rollup deploy?


Component Item Filtering:
ROLLBACK - only include if rolling back
ROLLUP   - only include if rolling up
NOT SET  - just use component filter

$av1 > $av2
$application == ${avloop.first}

set backout = ${avloop.first} - 1

set next = ${avloop.first} + 1   - this may result in an exception if there is a branch (safe in an avloop as it would select the branch for us)

for loop to be av aware

for(av = ${application.parent.versions['CreditScore;1']}; av <= $application; av = $av + 1) {
}

application.predecessors

environment.applications - apps allowed in env
environment.appvers - current app vers in env


*/
