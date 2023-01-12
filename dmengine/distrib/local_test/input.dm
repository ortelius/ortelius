action Test
{
  echo "Test Suite execution started $(now())";
  echo "Deployment id is ${DEPLOY_ID}; pid is $$";

  if(1 = 1) {
    //echo '1 = 1';
  }

  //echo "DMHOME = '${DMHOME}'";
  //echo "TRIDM_PLATFORM = '${TRIDM_PLATFORM}'";

  //for(n = 1; $n <= 4; n = $n + 1) {
  //  echo "TRIFIELD${n} = '${TRIFIELD$n}'";
  //}
  //foreach(field: $TRIFIELD) {
  //  echo "TRIFIELD['${field}'] = '${TRIFIELD[$field]}'";
  //}
  //echo "logical.name = '${logical.name}'";

  //set -i nocasevar = 'hello';

  // Global test data
  set -g myglobalvar1 = 'somevalue1';
  set -g myglobalarray = { 'one' => 'ONE', 'two' => 'TWO' };

  // Run the tests
  //dump;
  //notify_tests;
  //sms_tests;
  //abort;

  //deploy_tests;
  //abort;

  //script_tests;
  //abort;

  //iisplugin_tests;
  //abort;

  //remote_tests;
  //abort;

  //modify_tests;
  //abort;

  //graphical_tests;
  //abort;

  //echo "before sleep $(now())";
  //sleep(delay: 10);
  //echo "After sleep $(now())";

  //set rand = random(100);
  //echo "rand = $rand";
  //abort;

  /*if($TRIDM_PLATFORM = 'Windows') {
    if(rtrim(pingable_win('localhost')) = 'alive') {
      echo 'localhost is alive';
    } else {
      echo 'localhost is not responding';
    }
  }*/

  //checkout(repository: 'perforce', dropzone: 'perforce1', recursive: true, pattern: {'*.sql', '*.war'}) {	//  pattern: '*/*'
  //  post {
  //    dump(what: $dropzone);
  //  }
  //}
  //abort;

  //swarm_add_review_comment(server: 'swarm', review: 15, body: 'We have rested and we shall continue');
  //set temp = swarm_get_reviews('swarm');
  //abort;

  set_tests;
  scope_tests;
  operator_tests;
  flow_tests;
  call_tests;
  function_tests;
  array_tests;
  metadata_tests;
  objectref_tests;
  loop_tests;
  stream_tests;
  date_tests;
  query_tests;
  localscript_tests;
  dbscript_tests;
  eval_tests;
  trilogy_tests;
  pathname_tests;
  component_tests;
  graphical_tests;
  //task_tests;
  //populate_combo3;

  // Simple switch
  test(name: 'switch test #1') {
    set val = 1;
    switch($val) {
      case 0:  set res = 'zero'; break;
      case 1:  set res = 'one'; break;
      default: set res = 'other'; break;
    }
    assert(str1: $res, str2: 'one');
  }

  // Simple switch with fall thru
  test(name: 'switch test #2') {
    set val = 1;
    switch($val) {
      case 0:  set res = 'zero'; break;
      case 1:  set res = 'one'; // fall thru...
      default: set res = 'other'; break;
    }
    assert(str1: $res, str2: 'other');
  }

  if(${global.test_failures} != 0) {
    echo "***** Test report: ${global.test_failures} out of ${global.test_total} test(s) failed!";
  } else {
    echo "***** Test report: ${global.test_total} tests executed.  All tests passed!";
  }

  try {
    splat(fred: 'jim') { sheila; }
  } catch(e) {
    echo "Caught exception: ${e}";
    echo ${e.stacktrace};
  }

  try {
    assert(expr: 0);
  } catch(e) {
    echo "Caught exception: ${e}";
  }

  echo "Test Suite execution finished $(now())";
}

action show_repository_details
{
  echo "Deploy repository is '${deploy.repository}'; Pattern is '${deploy.pattern}'; Repository type is '${repository.type}'";

  switch(${repository.type}) {
  case 'harvest': echo "Broker: '${repository.broker}'; Project: '${repository.project}'; State: '${repository.state}'"; break;
  case 'svn':     echo "URL: '${repository.url}${repository.path}'"; break;
  }
}


action deploy_tests
{
  // Try to access a repository in a different domain - should appear not to exist.
  test(name: 'deploy test #1') {
    try {
      checkout(dropzone: 'invisible', repository: 'dm_demo', viewpath: @'\sql', target: 'sql_files');
      assert(expr: false);
    } catch(e) {
      assert(str1: $e, str2: "Repository 'dm_demo' not found");
    }
  }

//  try {
//    // test, test_svn or noaccess - dm_demo_svn
////using dropzone 'jim' {
//    deploy(repository: 'test', /*process: 'Check Out for Browse',*/ /*path: 'sql',*/ url: 'hcjfhj',  pattern: '*.java', project: 'unused') {		// TODO: project: $mycmdlinevar, state: $state
//      pre {
//      	/**/show_repository_details;
//      	echo "Deploying to ${server.name}...";
//      }
//      post {
//      	echo "Checked out $TRIDM_DEP_SUCCESS/$TRIDM_DEP_TOTAL from ${deploy.path}";
//      	//abort(msg: 'abort deploy');
//      	//dump(what: $dropzone);
//      }
//    }
////dump(what: $dropzone);
////}
//  } catch(e) {
//    echo $e;
//  }

  try {
    using dropzone 'fred' {
      checkout(repository: 'dm_demo_svn', path: 'sql', target: 'sql_files') {
        post {
          echo "Checked out $TRIDM_DEP_SUCCESS/$TRIDM_DEP_TOTAL from ${deploy.path}";
        }
      }
      checkout(repository: 'dm_demo_svn', path: 'target', revision: '1') {
        post {
          echo "Checked out $TRIDM_DEP_SUCCESS/$TRIDM_DEP_TOTAL from ${deploy.path}";
        }
      }

      jar(options: 'xf', jarfile: "dmdemo.war", files: 'WEB-INF/web.xml');

      // WEB-INF/web.xml created
      dump(what: $dropzone);

      modify(modifier: 'xml', file: 'WEB-INF/web.xml') {
        add_element(xpath: '/web-app', pos: 'inside',  value: "<wibble />");
      }

      // WEB-INF/web.xml changed size
      dump(what: $dropzone);

      modify(modifier: 'xml', file: 'WEB-INF/web.xml', serverspecific: true);

      delete(file: 'WEB-INF/web.xml');

      rename(from: 'WEB-INF/web.xml', to: "WEB-INF/web_${server.name}.xml", serverspecific: true);

      modify(modifier: 'text', file: 'sql_files/rollback_dob.sql', serverspecific: true) {
      	text_replace(find: 'users', replace: 'dmdemo.users');
      	text_replace(find: '(.)e', replace: '\1EE');
      }

      delete(file: 'sql_files/rollback_dob.sql');

      //retrieve;

      //harvest_createpackage(repository: 'test', name: 'mypackage');

      checkin(repository: 'test', source: 'WEB-INF', viewpath: @"logs\${server.name}",
              package: 'foobar', createpackage: true, options: 'ne');

      dump(what: $dropzone);

      set flist = "";
      set sqlfiles = ${dropzone.find('*.sql')};
      foreach(file: $sqlfiles) {
        set flist = "${flist}${flist:+', '}${file}";
        set fileobj = $sqlfiles[$file];
        echo "${fileobj.dzpath} - ctime: ${fileobj.ctime}";
        echo "${fileobj.dzpath} - mtime: ${fileobj.mtime}";
      }
      echo "fred.find('*.sql'): ${flist}";

      set size = ${sqlfiles.sum('${value.size}')};
      echo "sqlfiles.size: ${size} bytes";

      set flist = "";
      set xmlfiles = ${dropzone.find('*.xml')};
      foreach(file: ${xmlfiles}) {
        set flist = "${flist}${flist:+', '}${file}";
      }
      echo "fred.find('*.xml'): ${flist}";

      set size = ${xmlfiles.sum('${value.size}')};
      echo "xmlfiles.size: ${size} bytes";

      if(${server.name} = 'server2') {
        transfer(target: 'subdir');
      }
    }
  } catch(e) {
    echo $e;
    echo ${e.stacktrace};
  }


  /*try {
    parallel {
      deploy(repository: 'test_svn') {
      	pre {
      	  //show_repository_details;
      	}
        post {
          abort(msg: 'abort #1');
        }
      }
      deploy(repository: 'test') {
        post {
          abort(msg: 'abort #2');
        }
      }
    }
  } catch(e) {
    echo $e;
  }*/

  //abort;
}


action set_tests
{
  test(name: 'set test #1') {
    set test1 = 1;
    assert(expr1: $test1, expr2: 1);
  }

  test(name: 'set test #2') {
    set test2 = 'hello';
    assert(str1: $test2, str2: 'hello');
  }

  test(name: 'set test #3') {
    set test3 = 'hello world';
    try {
      assert(str1: $test3, str2: 'hello');
    } catch(e) {
      set exmsg = $e;
    }
    assert(str1: ${exmsg}, str2: 'value was \'hello world\' expecting \'hello\'');
  }

  test(name: 'set test #4') {
    set test4 = 'hello';
    assert(str1: upper($test4), str2: 'HELLO');
    assert(expr1: length($test4), expr2: 5);
    assert(str1: substr($test4,1,3), str2: 'ell');
  }

  // #5: String literal concatenation 1
  test(name: 'set test #5') {
    set test5 = "Hello" " " "World" "!";
    assert(str1: ${test5}, str2: "Hello World!");
  }

  // #6: String literal concatenation 2
  test(name: 'set test #6') {
    set test6 = 'Hello' ' ' 'World' '!';
    assert(str1: ${test6}, str2: "Hello World!");
  }

  // #7: Long string literal
  test(name: 'set test #7') {
    set test7 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyz0123456789";
    assert(expr1: length($test7), expr2: 258);
    assert(str1: ${test7}, str2: '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwzyz0123456789');
  }

  // #8: String functions - rtrim
  test(name: 'set test #8') {
    set string1 = rtrim('string \t \r\n');
    assert(expr1: length($string1), expr2: 6);
    assert(str1: ${string1}, str2: 'string');
  }

  // #9: Boolean type
  test(name: 'set test #9') {
    set bvar1 = true;
    set bvar2 = false;
    assert(expr1: $bvar1, expr2: true);
    assert(expr1: $bvar2, expr2: false);
    assert(str1: $bvar1, str2: '1');
    assert(str1: $bvar2, str2: '0');
  }

  // #10: String append
  test(name: 'set test #10') {
    set str1 = 'hello';
    set -a str1 = ' ';
    set -a str1 = 'world';
    assert(str1: $str1, str2: 'hello world');
  }

  // #11: set with increment
  test(name: 'set test #11') {
    set op1 = 1;
    set op1 += 1;
    assert(expr1: $op1, expr2: 2);
  }

  // #12: set with decrement
  test(name: 'set test #12') {
    set op2 = 2;
    set op2 -= 1;
    assert(expr1: $op2, expr2: 1);
  }

  // #13: set with multiply
  test(name: 'set test #13') {
    set op3 = 2;
    set op3 *= 2;
    assert(expr1: $op3, expr2: 4);
  }

  // #14: set with increment
  test(name: 'set test #14') {
    set inc1 = 1;
    incr inc1;
    assert(expr1: $inc1, expr2: 2);
  }

  // #15: set with decrement
  test(name: 'set test #12') {
    set dec1 = 2;
    decr dec1;
    assert(expr1: $dec1, expr2: 1);
  }

  // #16: substr (object version)
  test(name: 'set test #16') {
    set test16 = 'hello';
    assert(str1: ${test16.upper()}, str2: 'HELLO');
    assert(expr1: ${test16.length()}, expr2: 5);
    assert(str1: ${test16.substr(1,3)}, str2: 'ell');
  }

  // #17: String functions - rtrim (object version)
  test(name: 'set test #17') {
    set string1 = 'string \t \r\n';
    set string2 = ${string1.rtrim()};
    assert(expr1: ${string2.length()}, expr2: 6);
    assert(str1: $string2, str2: 'string');
  }

  // #1: Test expansion of variable names
  test(name: 'expansion test #1') {
    set BAR = 'FIE';
    set BAT = 'FUM';
    set FOO_FIE_FUM = 'FEE';
    assert(str1: ${FOO_${BAR}_${BAT}}, str2: 'FEE');
  }

  // #2: Test expansion of variables names - result longer than spec
  test(name: 'expansion test #2') {
    set ROLE = 'some_big_long_role_name';
    set APP_app_ROLE_some_big_long_role_name = 'Y';
    assert(str1: ${APP_app_ROLE_${ROLE}}, str2: 'Y');
  }

  // Test data for next few tests
  set pkg = 'Application_v1.23';
  set base1 = @'D:\temp.tmp';
  set base1r = @'D:\\temp.tmp';

  // #1: Regular expresion substitutions - extract first section
  test(name: 'regexp test #1') {
    set app = regsub($pkg, '^([A-Za-z]+)_v([0-9]+.[0-9]+)', '\1');
    assert(str1: ${app}, str2: 'Application');
  }

  // #2: Regexp - extract second section
  test(name: 'regexp test #2') {
    set numbers = regsub($pkg, '^([A-Za-z]+)_v([0-9]+.[0-9]+)', '\2');
    assert(str1: ${numbers}, str2: '1.23');
  }

  // #3: Regexp - pull out section after variable
  test(name: 'regexp test #3') {
    set path1 = @"${base1}\1.23\wibble";
    set numbers2 = regsub($path1, @"^${base1r}\\([0-9]+.[0-9]+)\\.*\$", '\1');
    assert(str1: ${numbers2}, str2: '1.23');
  }

  // #4: Regexp - no match - returns original string
  test(name: 'regexp test #4') {
    //set path1 = @"${base1}\wibble";
    set nomatch = regsub($pkg, @"^${base1r}\\([0-9]+.[0-9]+).*\$", '\1');
    assert(str1: ${nomatch}, str2: $pkg);
  }

  // #5: Regexp - spaces
  test(name: 'regexp test #5') {
    set regexp5 = regsub('@some value@', '^@([A-Za-z ]+)@$', '\1');
    assert(str1: ${regexp5}, str2: 'some value');
  }

  // #6: Regexp - multiple occurences
  test(name: 'regexp test #6') {
    set regexp6 = regsub('foo bar bat', 'ba(.)', 'ca\1t');
    assert(str1: ${regexp6}, str2: 'foo cart catt');
  }

  // #7: Regexp - extract second section - object syntax
  test(name: 'regexp test #7') {
    set numbers2 = ${pkg.regsub('^([A-Za-z]+)_v([0-9]+.[0-9]+)', '\2')};
    assert(str1: ${numbers2}, str2: '1.23');
  }

  // #1: Global var using name/value set
  test(name: 'global test #1') {
    assert(str1: ${myglobalvar1}, str2: 'somevalue1');
  }

  // #2: Global var using child elements set
  //test(name: 'global test #2') {
  //      assert(str1: ${myglobalvar2}" str2="somevalue2" />
  //}

  // #3: Global array using array set
  test(name: 'global test #3') {
    assert(str1: ${myglobalarray}, str2: '"ONE" "TWO"');
    assert(str1: ${myglobalarray['one']}, str2: 'ONE');
  }

  // #1: Array on a server
  test(name: 'server array test #1') {
    set -g arrcount1 = 0;
    psloop {
      if(${server.name} = 'server1') {
      	set -g arrcount1 = $arrcount1 + 1;
        assert(str1: ${myphysicalarray['one']}, str2: 'ONE');
      }
    }
    assert(expr1: $arrcount1, expr2: 1);
  }

  // #2: No case array on a server
  test(name: 'server array test #2') {
    set -g arrcount2 = 0;
    psloop {
      if(${server.name} = 'server1') {
      	set -g arrcount2 = $arrcount2 + 1;
        assert(str1: ${myphysicalarray2['OnE']}, str2: 'ONE');
      }
    }
    assert(expr1: $arrcount2, expr2: 1);
  }

  // #3: Array on a server - using old name
  test(name: 'server array test #1') {
    set -g arrcount3 = 0;
    psloop {
      if(${physical.name} = 'server1') {
      	set -g arrcount3 = $arrcount3 + 1;
        assert(str1: ${myphysicalarray['one']}, str2: 'ONE');
      }
    }
    assert(expr1: $arrcount3, expr2: 1);
  }

  // #1: String compares - default case sensitive
  test(name: 'strcmp test #1') {
    set string1 = 'STRING';
    set string2 = 'string';
    assert(expr: ${string1} != ${string2});
  }

  // #2: String compares - convert to lower
  test(name: 'strcmp test #2') {
    set string1 = lower('STRING');
    set string2 = 'string';
    assert(expr: ${string1} = ${string2});
  }

  // #3: String compares - convert to upper
  test(name: 'strcmp test #3') {
    set string1 = 'STRING';
    set string2 = upper('string');
    assert(expr: ${string1} = ${string2});
  }

  // #4: String compares - convert to upper (object version)
  test(name: 'strcmp test #4') {
    set string1 = 'STRING';
    set string2 = 'string';
    assert(expr: ${string1} = ${string2.upper()});
  }

  // #1: Case sensitivity - simple variables
  test(name: 'case sens test #1') {
    set varcs1 = 'one';
    set varcs2 = 'TWO';
    set -i varcs3 = 'one';
    set -i varcs4 = 'TWO';
    assert(expr: ${varcs1} = 'one');
    assert(expr: ${varcs1} != 'ONE');
    assert(expr: ${varcs2} = 'TWO');
    assert(expr: ${varcs2} != 'two');
    assert(expr: ${varcs3} = 'ONE');
    assert(expr: ${varcs3} = 'one');
    assert(expr: ${varcs4} = 'TWO');
    assert(expr: ${varcs4} = 'two');
  }

  // #2: Case sensitivity - array indices
  test(name: 'case sens test #2') {
    set arraycs1 = { 'one' => 'ONE', 'TWO' => 'two' };
    set -i arraycs2 = { 'one' => 'ONE', 'TWO' => 'two' };
    assert(str1: ${arraycs1['one']}, str2: 'ONE');
    assert(str1: ${arraycs1['ONE']}, str2: '');
    assert(str1: ${arraycs1['two']}, str2: '');
    assert(str1: ${arraycs1['TWO']}, str2: 'two');
    assert(str1: ${arraycs2['one']}, str2: 'ONE');
    assert(str1: ${arraycs2['ONE']}, str2: 'ONE');
    assert(str1: ${arraycs2['two']}, str2: 'two');
    assert(str1: ${arraycs2['TWO']}, str2: 'two');
  }

  // #3: Case sensitivity - global with case insens
  test(name: 'case sens test #3') {
    set -g -i myglobalstr1 = 'Hello';
    assert(expr: ${global.myglobalstr1} = 'hello');
  }

  // #1: String expansion - simple variables
  test(name: 'string expansion test #1') {
    set var1 = 'foo';
    set var2 = 'bar';
    assert(str1: "$var1 is $var2", str2: 'foo is bar');
  }

  // #2: String expansion - simple variables in braces
  test(name: 'string expansion test #2') {
    set var1 = 'foo';
    set var2 = 'bar';
    assert(str1: "${var1} is ${var2}", str2: 'foo is bar');
  }

  // #3: String expansion - arrays
  test(name: 'string expansion test #3') {
    set array1 = { 'one' => 'ONE', 'TWO' => 'two' };
    set two = 'TWO';
    assert(str1: "${array1['one']} is ${array1[$two]}", str2: 'ONE is two');
  }

  // #4: String expansion - objects
  test(name: 'string expansion test #4') {
    assert(str1: "${environment.name} is ${application.name}", str2: 'envA is app2');
  }

  // #5: String expansion - escaped characters
  test(name: 'string expansion test #5') {
    set var1 = 'foo';
    assert(str1: "\$var1 $var1 \$var1 ${var1} \${var1}", str2: '$var1 foo $var1 foo \foo');
  }

  // #6: String expansion - windows filenames
  test(name: 'string expansion test #6') {
    set var1 = 'foo';
    assert(str1: "C:\\Temp\\${var1}\\$var1", str2: @'C:\Temp\foo$var1');
  }

  // #7: String expansion - windows filenames 2
  test(name: 'string expansion test #7') {
    set var1 = 'foo';
    assert(str1: @"C:\Temp\${var1}\$var1", str2: @'C:\Temp\foo$var1');
  }

  // #8: String expansion - abutted variables
  test(name: 'string expansion test #8') {
    set var1 = 'foo';
    set var2 = 'bar';
    assert(str1: "$var1$var2", str2: 'foobar');
  }

  // #9: String expansion - abutted variables in braces
  test(name: 'string expansion test #9') {
    set var1 = 'foo';
    set var2 = 'bar';
    assert(str1: "${var1}${var2}", str2: 'foobar');
  }

  // #10: String expansion - old objects
  test(name: 'string expansion test #10') {
    assert(str1: "${logical.name} is ${application.name}", str2: 'envA is app2');
  }

  // #11: String expansion - simple expression
  test(name: 'string expansion test #11') {
    assert(str1: "1 + 1 = $(1+1)", str2: '1 + 1 = 2');
  }

  // #12: String expansion - expression involving functions
  test(name: 'string expansion test #12') {
    set var1 = 'foo';
    set var2 = 'bar';
    assert(str1: "$(upper($var1)) is $(upper($var2))", str2: 'FOO is BAR');
  }

  // #1: Command-line specified variables
  test(name: 'cmd line vars #1') {
    assert(str1: ${cmdln_var}, str2: 'testvalue');
  }

  // #2: Command-line specified arguments
  test(name: 'cmd line vars #2') {
    assert(expr1: ${ARGC}, expr2: 2);
    assert(str1: ${ARGV[0]}, str2: 'argv1');
    assert(str1: ${ARGV[1]}, str2: 'argv2');
  }

  // #1: Trifield variables - simple field
  test(name: 'trifield vars #1') {
    assert(str1: ${TRIFIELD1}, str2: 'envA');
    assert(str1: ${TRIFIELD['environments']}, str2: 'envA');
  }

  // #2: Trifield variables - listbox
  test(name: 'trifield vars #2') {
    assert(str1: ${TRILISTBOX[1]['PACKAGENAME']}, str2: 'MyPackage1');
    assert(str1: ${TRILISTBOX[1]['CREATED_BY']},  str2: 'fred');
    assert(str1: ${TRILISTBOX[2]['PACKAGENAME']}, str2: 'MyPackage2');
    assert(str1: ${TRILISTBOX[2]['CREATED_BY']},  str2: 'jim');
  }
}


action scope_tests
{
  // #1: local scope is visible from test scope
  set scope1 = 'hello';
  test(name: 'scope test #1') {
    set scope1 = 'goodbye';
    set scope1a = "${scope1} world";
    assert(str1: $scope1a, str2: 'goodbye world');
  }

  // #2: test scope is not visible from local scope
  test(name: 'scope test #2') {
    set scope2 = "${scope1} world - ${scope1a}";
    assert(str1: $scope2, str2: 'hello world - ');
  }

  // #3: Set within psloop should set local variable not server variable
  test(name: 'scope test #3') {
    set scope3 = 1;
    psloop {
      set scope3 += 1;
    }
    assert(expr1: $scope3, expr2: 3);
  }

  // #4: Set within psloop should set local array variable not server variable
  test(name: 'scope test #4') {
    set scope4 = { 'one' => 'ONE', 'two' => 'TWO' };
    psloop {
      set scope4a = $scope4;
    }
    assert(str1: $scope4a, str2: '"ONE" "TWO"');
  }

  // #5: Set within psloop should set local exception variable not server variable
  test(name: 'scope test #5') {
    psloop {
      try {
      	abort;
      } catch(scope5) {
      }
    }
    assert(str1: ${scope5.kind}, str2: 'AbortException');
  }

  // #6: Set within psloop should set local object variable not server variable
  test(name: 'scope test #6') {
    psloop {
      set scope6 = $server;
    }
    assert(str1: ${scope6.name}, str2: 'server2');
  }

  // #7: Set within psloop should create local array variable not server variable
  test(name: 'scope test #7') {
    psloop {
      echo 'hello world' >> $stream7;
      set scope7 = $stream7;
    }
    assert(str1: $scope7, str2: 'hello world');
  }

  // #8: Set within psloop should set local object variable not server variable
  test(name: 'scope test #8') {
    psloop {
      set -a scope8 = ${server.name};
    }
    assert(str1: ${scope8}, str2: 'server1server2');
  }

  // #9: Set within psloop should create local array variable not server variable
  test(name: 'scope test #9') {
    psloop {
      set scope9 = { 'one' => 'ONE', 'two' => 'TWO' };
    }
    assert(str1: $scope9, str2: '"ONE" "TWO"');
  }
}


action operator_tests
{
  // #1: addition operator
  test(name: 'operator test #1') {
    assert(expr1: 1 + 1, expr2: 2);
  }

  // #2: subtraction operator
  test(name: 'operator test #2') {
    assert(expr1: 2 - 1, expr2: 1);
  }

  // #3: multiplication operator
  test(name: 'operator test #3') {
    assert(expr1: 2 * 2, expr2: 4);
  }

  // #4: integer division operator
  test(name: 'operator test #4') {
    assert(expr1: 5 / 2, expr2: 2);
  }

  // #5: modulo operator
  test(name: 'operator test #5') {
    assert(expr1: 5 % 2, expr2: 1);
  }

  // #6: operator precedence test - taken from daft internet example
  test(name: 'operator test #6') {
    assert(expr1: 6 - 1 * 0 + 2 / 2, expr2: 7);
  }

  // #7: less than operator
  test(name: 'operator test #7') {
    assert(expr1: 2 < 3, expr2: true);
    assert(expr1: 3 < 2, expr2: false);
  }

  // #8: greater than operator
  test(name: 'operator test #8') {
    assert(expr1: 3 > 2, expr2: true);
    assert(expr1: 2 > 3, expr2: false);
  }

  // #9: less than equal operator
  test(name: 'operator test #9') {
    assert(expr1: 2 <= 3, expr2: true);
    assert(expr1: 2 <= 2, expr2: true);
    assert(expr1: 3 <= 2, expr2: false);
  }

  // #10: greater than equal operator
  test(name: 'operator test #10') {
    assert(expr1: 3 >= 2, expr2: true);
    assert(expr1: 2 >= 2, expr2: true);
    assert(expr1: 2 >= 3, expr2: false);
  }

  // #11: not operator
  test(name: 'operator test #11') {
    assert(expr1: !false, expr2: true);
    assert(expr1: !true, expr2: false);
  }

  // #12: and operator
  test(name: 'operator test #12') {
    assert(expr1: false & false, expr2: false);
    assert(expr1: false & true, expr2: false);
    assert(expr1: true & false, expr2: false);
    assert(expr1: true & true, expr2: true);
  }

  // #13: or operator
  test(name: 'operator test #13') {
    assert(expr1: false | false, expr2: false);
    assert(expr1: false | true, expr2: true);
    assert(expr1: true | false, expr2: true);
    assert(expr1: true | true, expr2: true);
  }
}


action flow_tests
{
  // #1: Simple for loop with space separated values and default ifs
  test(name: 'foreach string loop test #1') {
    foreach(foo: 'one two three') {
      set list = ${list}${list:+','}${foo};
    }
    assert(str1: ${list}, str2: 'one,two,three');
  }

  // #2: Simple for loop with pipe separated values and pipe ifs
  test(name: 'foreach string loop test #2') {
    set IFS = '|';
    foreach(bar: 'alpha|beta|gamma|delta') {
      set list = ${list}${list:+','}${bar};
    }
    assert(str1: ${list}, str2: 'alpha,beta,gamma,delta');
  }

  // #3: Nested for loop with space and pipe separated values and default and pipe ifs
  test(name: 'foreach string loop test #3') {
    foreach(foo: 'one|two|three four|five|six seven|eight|nine') {
      set IFS = '|';
      foreach(bar: ${foo}) {
        set list = ${list}${list:+','}${bar};
      }
    }
    assert(str1: ${list}, str2: 'one,two,three,four,five,six,seven,eight,nine');
  }

  // #4: For loop with quoted strings
  // Note: filelist is set in two parts so as to avoid having to escape slahses
  test(name: 'foreach string loop test #4') {
    set filelist = @'C:\foo.bat "C:\Space In Path\bar.bat" C:\more\than\one\slash.gif';
    set -a filelist = @"'C:\More Spaces\bat.jpg'";
    set DEQUOTE = true;
    foreach(file: ${filelist}) {
      if(${file} ~ '\.bat$') {
        set b = $b + 1;
      } else {
        set o = $o + 1;
      }
    }
    assert(expr1: $b, expr2: 2);
    assert(expr1: $o, expr2: 2);
  }

  // #5: Simple for loop with space separated values and default ifs using alternate syntax to please Phil
  test(name: 'foreach string loop test #5') {
    foreach foo in 'one two three' {
      set list = ${list}${list:+','}${foo};
    }
    assert(str1: ${list}, str2: 'one,two,three');
  }

  // #6: Simple for loop with space separated values and default ifs and break
  test(name: 'foreach string loop test #6') {
    foreach(foo: 'one two three') {
      set list = ${list}${list:+','}${foo};
      if($foo = 'two') { break; }
    }
    assert(str1: ${list}, str2: 'one,two');
  }

  // #7: Simple for loop with space separated values and default ifs and continue
  test(name: 'foreach string loop test #7') {
    foreach(foo: 'one two three') {
      if($foo = 'two') { continue; }
      set list = ${list}${list:+','}${foo};
    }
    assert(str1: ${list}, str2: 'one,three');
  }

  // #1: While loop with counter
  test(name: 'while loop test #1') {
    set n = 0;
    while($n < 10) {
      set list = ${list}${list:+','}${n};
      set n = $n + 1;
    }
    // echo ${list};
    assert(str1: ${list}, str2: '0,1,2,3,4,5,6,7,8,9');
  }

  // #2: While loop with counter and break
  test(name: 'while loop test #2') {
    set n = 0;
    while($n < 6) {
      set list = ${list}${list:+','}${n};
      if($n = 3) { break; }
      set n = $n + 1;
    }
    assert(str1: ${list}, str2: '0,1,2,3');
  }

  // #3: While loop with counter and continue
  test(name: 'while loop test #3') {
    set n = 0;
    while($n < 6) {
      set n = $n + 1;
      if($n = 3) { continue; }
      set list = ${list}${list:+','}${n};
    }
    assert(str1: ${list}, str2: '1,2,4,5,6');
  }

  // Test for interaction between then and else - 1.25 onwards should pass this
  test(name: 'if/then/else test') {
    set match = 'hostname';
    if(${server.hostname} ~ ${match}) {
      set match = 'not_hostname';
      psloop {
      	set -g matched = ${matched}${matched:+' '}${server.name};
      }
    } else {
      psloop {
        set -g not_matched = ${not_matched}${not_matched:+' '}${server.name};
      }
    }
    //echo "Then Matched: ${matched}";
    //echo "Else Matched: ${not_matched}";
    assert(str1: ${matched}, str2: 'server1 server2');
    assert(str1: ${not_matched}, str2: '');
  }

  // #1: For loop with array iterating over keys
  test(name: 'foreach array test #1') {
    set arr = { 'a' => '1', 'b' => '2', 'c' => '3', 'd' => '4', 'e' => '5' };
    foreach(k: $arr) {
      set list = ${list}${list:+','}${k};
      set list2 = ${list2}${list2:+','}${arr[$k]};
    }
    assert(str1: ${list}, str2: 'd,e,a,b,c');
    assert(str1: ${list2}, str2: '4,5,1,2,3');
  }

  // #2: For loop with array iterating over values
  test(name: 'foreach array test #2') {
    set arr = { 'a' => '1', 'b' => '2', 'c' => '3', 'd' => '4', 'e' => '5' };
    foreach(v: flatten($arr)) {
      set list = ${list}${list:+','}${v};
    }
    assert(str1: ${list}, str2: '"4","5","1","2","3"');
  }

  // #3: For loop with array iterating over keys with break
  test(name: 'foreach array test #3') {
    set arr = { 'a' => '1', 'b' => '2', 'c' => '3', 'd' => '4', 'e' => '5' };
    foreach(k: $arr) {
      set list = ${list}${list:+','}${k};
      if($k = 'a') { break; }
    }
    assert(str1: ${list}, str2: 'd,e,a');
  }

  // #4: For loop with array iterating over keys with continue
  test(name: 'foreach array test #4') {
    set arr = { 'a' => '1', 'b' => '2', 'c' => '3', 'd' => '4', 'e' => '5' };
    foreach(k: $arr) {
      if($k = 'a') { continue; }
      set list = ${list}${list:+','}${k};
    }
    assert(str1: ${list}, str2: 'd,e,b,c');
  }

  // #1: C-style for loop with a simple counter
  test(name: 'for loop test #1') {
    for(n = 0; $n < 6; n = $n + 1) {
      set list = ${list}${list:+','}${n};
    }
    assert(str1: ${list}, str2: '0,1,2,3,4,5');
  }

  // #2: C-style for loop with a simple counter and break
  test(name: 'for loop test #2') {
    for(n = 0; $n < 6; n = $n + 1) {
      set list = ${list}${list:+','}${n};
      if($n = 3) { break; }
    }
    assert(str1: ${list}, str2: '0,1,2,3');
  }

  // #3: C-style for loop with a simple counter and continue
  test(name: 'for loop test #3') {
    for(n = 0; $n < 6; n = $n + 1) {
      if($n = 3) { continue; }
      set list = ${list}${list:+','}${n};
    }
    assert(str1: ${list}, str2: '0,1,2,4,5');
  }

  // #1: try/catch/finally - exception not raised
  test(name: 'try/catch test #1') {
    try {
      set a = 1;
    } catch(e) {
      set b = 2;
    } finally {
      set c = 3;
    }
    assert(expr: $a = 1 -a $b = 0 -a $c = 3);
  }

  // #2: try/catch/finally - exception was raised
  test(name: 'try/catch test #2') {
    try {
      set a = 1;
      abort(msg: 'exception raised');
    } catch(e) {
      set b = 2;
    } finally {
      set c = 3;
    }
    assert(expr: $a = 1 -a $b = 2 -a $c = 3);
  }

  // #3: try/catch/finally interaction with break
  test(name: 'try/catch test #3') {
    foreach(foo: 'one two three') {
      try {
        set list = ${list}${list:+','}${foo};
        if($foo = 'two') { break; }
      } catch(e) {
      	assert(expr: false);
      } finally {
        set list2 = ${list2}${list2:+','}${foo};
      }
    }
    assert(str1: ${list}, str2: 'one,two');
    assert(str1: ${list2}, str2: 'one,two');
  }

  // #4: try/catch/finally interaction with continue
  test(name: 'try/catch test #4') {
    foreach(foo: 'one two three') {
      try {
        if($foo = 'two') { continue; }
        set list = ${list}${list:+','}${foo};
      } catch(e) {
      	assert(expr: false);
      } finally {
        set list2 = ${list2}${list2:+','}${foo};
      }
    }
    assert(str1: ${list}, str2: 'one,three');
    assert(str1: ${list2}, str2: 'one,two,three');
  }

  // #5: try/catch - exception properties
  test(name: 'try/catch test #5') {
    try {
      abort(msg: 'exception raised');
    } catch(e) {
      set msg = ${e.message};
      set kind = ${e.kind};
    }
    assert(str1: $msg, str2: 'exception raised');
    assert(str1: $kind, str2: 'AbortException');
  }
}


action subaction
{
  set -g subaction_result = "$arg1 $arg2";
  set arg1 = 'goodbye';
  set myvar = 'local2';
}


action subaction2
{
  set -g subaction2_one = 'one';
  return;
  set -g subaction2_two = 'two';
}


action subaction3
{
  try {
    set -g subaction3_one = 'one';
    /**/stacktrace;
    return;
    set -g subaction3_two = 'two';
  } catch(e) {
    set -g subaction3_three = 'three';
  } finally {
    set -g subaction3_four = 'four';
  }
}


action subaction4
{
  set -g subaction4_result = ${obj.name};
}


action subaction5
{
  set -g subaction5_result = $arr[1];
}


action subaction6
{
  set -g subaction6_result = ${obj.myglobalvar1};
}


action subaction7
{
  set -g subaction7_result = "${test7_var1} ${test7_var2}";
}


action subaction8
{
  psloop {
    return;
  }
}


action call_tests
{
  // #1: call a subaction to make sure args passed and stack frame created
  test(name: 'call test #1') {
    set myvar = 'local1';
    subaction(arg1: 'hello', arg2: 'world');
    assert(str1: $subaction_result, str2: 'hello world');
    assert(str1: $myvar, str2: 'local1');
  }

  // #2: call a subaction which returns early
  test(name: 'call test #2') {
    subaction2;
    assert(str1: $subaction2_one, str2: 'one');
    assert(str1: $subaction2_two, str2: '');
  }

  // #3: call a subaction with a try/catch which returns early
  test(name: 'call test #3') {
    subaction3;
    assert(str1: $subaction3_one, str2: 'one');
    assert(str1: $subaction3_two, str2: '');
    assert(str1: $subaction3_three, str2: '');
    assert(str1: $subaction3_four, str2: 'four');
  }

  // #4: call a subaction with an object argument
  test(name: 'call test #4') {
    subaction4(obj: $environment);
    assert(str1: $subaction4_result, str2: 'envA');
  }

  // #5: call a subaction with an array argument
  test(name: 'call test #5') {
    set testarr = { 'one', 'two', 'three' };
    subaction5(arr: $testarr);
    assert(str1: $subaction5_result, str2: 'two');
  }

  // #6: call a subaction with an array argument
  test(name: 'call test #6') {
    subaction6(obj: ${global});
    assert(str1: $subaction6_result, str2: 'somevalue1');
  }

  // #7: call a subaction and ensure local vars stay private
  set test7_var1 = 'hello';
  test(name: 'call test #7') {
    set test7_var2 = 'world';
    subaction7;
    assert(str1: $subaction7_result, str2: ' ');
  }

  // #8: call an action in another domain that we shouldn't be able to see
  test(name: 'call test #8') {
    try {
      Deploy;
      assert(expr: false);
    } catch(e) {
      assert(str1: $e, str2: "Unrecognised statement 'Deploy'");
    }
  }

  // #9: call an action that returns from with a psloop - previously rasied a syntax error
  test(name: 'call test #9') {
    try {
      subaction8;
    } catch(e) {
      assert(expr: false);
    }
  }
}


function testfunc()
{
  return 'value';
}


function testfunc2(arg1, arg2)
{
  return "$arg1 $arg2";
}


function testfunc3(pkg)
{
  return regsub($pkg, '^([A-Za-z]+)_v([0-9]+.[0-9]+)', '\2');
}


function testfunc4(obj)
{
  return ${obj.name};
}


function testfunc5(arr)
{
  return $arr[1];
}


function testfunc6(obj)
{
  return ${obj.myglobalvar1};
}


function testfunc7()
{
  set funcarray1 = { 'one' => 'hello', 'two' => 'world' };
  return $funcarray1;
}


function testfunc8()
{
  using stream $funcstream1 {
    echo 'Hello functional world!';
  }
  return $funcstream1;
}


function testfunc9()
{
  psloop {
    return 1;
  }
}

action function_tests
{
  // #1: hex
  test(name: 'builtin func test #1') {
    assert(str1: hex(255), str2: 'ff');
  }

  //// #2: basename
  //test(name: 'builtin func test #2') {
  //  assert(str1: basename(@'C:\Temp\file.txt', 'windows'), str2: 'file.txt');
  //}

  //// #3: dirname
  //test(name: 'builtin func test #3') {
  //  assert(str1: dirname(@'C:\Temp\file.txt', 'windows'), str2: @'C:\Temp');
  //}

  // #4: dropzone
  test(name: 'builtin func test #4') {
    if($TRIDM_PLATFORM = 'Windows') {
      assert(str1: dropzone('fred'), str2: @"${TEMP}\tdm.${DEPLOY_ID}.fred");
    } else {
      assert(str1: dropzone('fred'), str2: "/tmp/tdm.${DEPLOY_ID}.fred");
    }
  }

  // #1: hex (object version)
  test(name: 'object func test #1') {
    set int1 = 255;
    assert(str1: ${int1.hex()}, str2: 'ff');
  }

  // #2: basename (object version)
  test(name: 'object func test #2') {
    set -g objfncount2 = 0;
    psloop {
      //echo "server type: ${server.type}";
      set -g objfncount2 = $objfncount2 + 1;
      if(${server.type} = 'windows') {
        assert(str1: ${server.dirname(@'C:\Temp\file.txt')}, str2: @'C:\Temp');
      } else {
        assert(str1: ${server.dirname('/tmp/file.txt')}, str2: '/tmp');
      }
    }
    assert(expr1: $objfncount2, expr2: 2);
  }

  // #3: dirname (object version)
  test(name: 'object func test #3') {
    set -g objfncount3 = 0;
    psloop {
      //echo "server type: ${server.type}";
      set -g objfncount3 = $objfncount3 + 1;
      if(${server.type} = 'windows') {
        assert(str1: ${server.basename(@'C:\Temp\file.txt')}, str2: 'file.txt');
      } else {
        assert(str1: ${server.basename('/tmp/file.txt')}, str2: 'file.txt');
      }
    }
    assert(expr1: $objfncount3, expr2: 2);
  }

  // #4: length of string (object version)
  // Second syntax is correct - both should work
  test(name: 'object func test #4') {
    set str = 'hello';
    assert(expr1: ${$str.length()}, expr2: 5);
    assert(expr1: ${str.length()}, expr2: 5);
  }

  // #5: length of array (object version)
  // Second syntax is correct - both should work
  test(name: 'object func test #5') {
    set arr = { 'a', 'b', 'c', 'd' };
    assert(expr1: ${$arr.length()}, expr2: 4);
    assert(expr1: ${arr.length()}, expr2: 4);
  }

  // #6: flatten array (object version)
  // Second syntax is correct - both should work
  test(name: 'object func test #6') {
    set arr = { 'a', 'b', 'c', 'd' };
    assert(str1: ${$arr.flatten()}, str2: '"c" "d" "a" "b"');
    assert(str1: ${arr.flatten()}, str2: '"c" "d" "a" "b"');
  }

  // #1: simple function invocation
  test(name: 'function test #1') {
    assert(str1: testfunc(), str2: 'value');
  }

  // #2: function invocation with arguments
  test(name: 'function test #2') {
    assert(str1: testfunc2('hello', 'world'), str2: 'hello world');
  }

  // #3: more complex function invocation with arguments
  test(name: 'function test #3') {
    set pkg = 'Application_v1.23';
    assert(str1: testfunc3($pkg), str2: '1.23');
  }

  // #4: function with object argument
  test(name: 'function test #4') {
    assert(str1: testfunc4($environment), str2: 'envA');
  }

  // #5: function with array argument
  test(name: 'function test #5') {
    set testarr = { 'one', 'two', 'three' };
    assert(str1: testfunc5($testarr), str2: 'two');
  }

  // #6: function with scope object argument
  test(name: 'function test #6') {
    assert(str1: testfunc6($global), str2: 'somevalue1');
  }

  // #7: function which returns an array
  test(name: 'function test #7') {
    set arr7 = testfunc7();
    assert(str1: $arr7['one'], str2: 'hello');
    assert(str1: $arr7['two'], str2: 'world');
  }

  // #8: function which returns a stream
  test(name: 'function test #8') {
    assert(str1: testfunc8(), str2: 'Hello functional world!');
  }

  // #9: library function which operates on integers
  test(name: 'function test #9') {
    assert(expr1: libfunc(2), expr2: 4);
  }

  // #10: undefined function
  test(name: 'function test #10') {
    set okay = true;
    try {
      set x = undefined();
      set okay = false;
    } catch(e) {
      assert(str1: $e, str2: "Unrecognised function 'undefined'");
    }
    assert(expr: $okay);
  }

  // #11: undefined function
  test(name: 'function test #11') {
    try {
      set x = testfunc9();
      assert(expr1: $x, expr2: 1);
    } catch(e) {
      assert(expr: false);
    }
  }
}


action array_tests
{
  set array[1] = 'one';
  set array[2] = 'two';

  // #1: Array to string conversion
  test(name: 'array test #1') {
    // echo "array = $array";
    assert(str1: ${array}, str2: '"two" "one"');
  }

  // #2: Array element access with constant subscript
  test(name: 'array test #2') {
    // echo "array[2] = $array[2]";
    assert(str1: $array[2], str2: 'two');
  }

  // #3: Array element access with variable subscript
  test(name: 'array test #3') {
    set subscript = 1;
    //echo "array[$subscript] = $array[$subscript]";
    assert(str1: $array[$subscript], str2: 'one');
  }

  // #4: For loop through result of array to string conversion
  test(name: 'array test #4') {
    set DEQUOTE = true;
    foreach(element: flatten($array)) {
      // echo ${element};
      set array1 = ${array1}${array1:+' '}${element};
    }
    assert(str1: ${array1}, str2: 'two one');
  }

  // #5: For loop through array keys
  test(name: 'array test #5') {
    foreach(key: $array) {
      // echo "array[${key}] = $array[${key}]";
      set array2 = ${array2}${array2:+' '}${key};
      set array3 = ${array3}${array3:+' '}$array[${key}];
    }
    assert(expr: (${array2} = '2 1') -a (${array3} = 'two one'));
  }

  // #6: For loop through result of array to string conversion with spaces in elements
  test(name: 'array test #6') {
    set arrayb[1] = 'one two';
    set arrayb[2] = 'three four';
    set DEQUOTE = true;
    foreach(element: flatten($arrayb)) {
      // echo ${element};
      set array4 = ${array4}${array4:+','}${element};
    }
    assert(str1: ${array4}, str2: 'three four,one two');
  }

  // Data for tests #7 and #8
  set subsa = 'ONE';
  set subsb = 'TWO';
  set arrayc[$subsa] = 'one two';
  set arrayc[$subsb] = 'three four';

  // #7: For loop through result of array to string conversion with spaces in elements and string subscripts
  test(name: 'array test #7') {
    set DEQUOTE = true;
    foreach(element: flatten($arrayc)) {
      // echo ${element};
      set array7 = ${array7}${array7:+','}${element};
    }
    assert(str1: ${array7}, str2: 'three four,one two');
  }

  // #8: For loop through array keys with spaces in elements and string subscripts
  test(name: 'array test #8') {
      foreach(key: $arrayc) {
      // echo $arrayc[${key}];
      set array8 = ${array8}${array8:+','}$arrayc[${key}];
    }
    assert(str1: ${array8}, str2: 'three four,one two');
  }

  // #9: Test appending of array elements
  test(name: 'array test #9') {
    set SEPARATOR = ', ';
    set -a arrayd[1] = 'ONE';
    set -a arrayd[1] = 'TWO';
    set -a arrayd[1] = 'THREE';
    assert(str1: $arrayd[1], str2: 'ONE, TWO, THREE');
  }

  // #10: New set array syntax to set serveral array elements in one go
  test(name: 'array test #10') {
    set index = 'two';
    set arraye = { 'one' => 'val one', 'two' => 'val two' };
    assert(str1: $arraye['one'], str2: 'val one');
    assert(str1: $arraye[${index}], str2: 'val two');
  }

  // #11: New set array syntax in conjunction with name expansion to set serveral array elements in one go
  test(name: 'array test #11') {
    set stub = 'foo';
    set index = 'two';
    set arrayf_foo = { 'one' => 'val one', 'two' => 'val two' };
    assert(str1: ${arrayf_${stub}['one']}, str2: 'val one');
    assert(str1: ${arrayf_${stub}[${index}]}, str2: 'val two');
  }

  // #12: Use xml variables in array subscripts
  test(name: 'array test #12') {
    psloop {
      set -g arrayg[${server.name}] = "bar${war}";
    }
    assert(str1: ${arrayg['server1']}, str2: 'bar1');
    assert(str1: ${arrayg['server2']}, str2: 'bar');
  }

  // #13: Use xml variables in array names
  test(name: 'array test #13') {
    psloop {
      set -g arrayh_${server.name}['foo'] = "bar${war}";
    }
    assert(str1: ${arrayh_server1['foo']}, str2: 'bar1');
    assert(str1: ${arrayh_server2['foo']}, str2: 'bar');
  }

  // #14: Use xml variables in array names
  test(name: 'array test #14') {
    psloop {
      foreach(key: ${arrayh_${server.name}}) {					// TODO: was array="arrayh_${physical.name}"
        set -g arrayh_${server.name}_foo = ${arrayh_${server.name}[$key]};
      }
    }
    assert(str1: ${arrayh_server1_foo}, str2: 'bar1');
    assert(str1: ${arrayh_server2_foo}, str2: 'bar');
  }

  // #15: Array subscripted by array element
  test(name: 'array test #15') {
    set arrayi[1] = 'one';
    set arrayj['one'] = 'two';
    assert(str1: $arrayj[$arrayi[1]], str2: 'two');
    assert(str1: ${arrayj[${arrayi[1]}]}, str2: 'two');
  }

  // #16: Array subscripted by array element
  test(name: 'array test #16') {
    set subsk = 'spacey name';
    set arrayk['spacey name'] = 'one';
    assert(str1: $arrayk['spacey name'], str2: 'one');
    assert(str1: $arrayk[$subsk], str2: 'one');
  }

  // #17: Array filtering
  test(name: 'array test #17') {
    set arrayl = { 'fred' => 'fred', 'jim' => 'jim', 'sheila' => 'sheila' };
    set arrayl2 = ${arrayl.filter('e')};
    assert(str1: $arrayl2['fred'], str2: 'fred');
    assert(str1: $arrayl2['jim'], str2: '');
    assert(str1: $arrayl2['sheila'], str2: 'sheila');
  }

  // #18: Array addition
  test(name: 'array test #18') {
    set arraym = { 'fred' => 'fred', 'jim' => 'jim' } + { 'sheila' => 'sheila' };
    assert(str1: $arraym['fred'], str2: 'fred');
    assert(str1: $arraym['jim'], str2: 'jim');
    assert(str1: $arraym['sheila'], str2: 'sheila');
  }

  // #19: Array of arrays
  test(name: 'array test #19') {
    set arrayn = {
      'fred'   => { 'one' => 'fred' },
      'jim'    => { 'two' => 'jim' },
      'sheila' => { 'three' => 'sheila' }
    };
    assert(str1: $arrayn['fred']['one'], str2: 'fred');
    assert(str1: $arrayn['jim']['two'], str2: 'jim');
    assert(str1: $arrayn['sheila']['three'], str2: 'sheila');
  }

  // #20: Sum of array values
  test(name: 'array test #20') {
    set arrayo = {
      'fred'   => 1,
      'jim'    => 2,
      'sheila' => 4
    };
    assert(str1: ${arrayo.sum('$value')}, str2: 7);
  }

  // #1: New set list syntax to set several list elements in one go
  test(name: 'list test #1') {
    set index = 1;
    set lista = { 'val one', 'val two' };
    assert(str1: $lista[0], str2: 'val one');
    assert(str1: $lista[${index}], str2: 'val two');
  }

  // #2: Append a list to make sure indexing works
  test(name: 'list test #2') {
    set index = 3;
    set listb = { 'val one', 'val two' };
    set -a listb = { 'val three', 'val four' };
    assert(str1: $listb[0], str2: 'val one');
    assert(str1: $listb[2], str2: 'val three');
    assert(str1: $listb[${index}], str2: 'val four');
  }

  // #3: Append a list to make sure indexing works
  test(name: 'list test #3') {
    set index = 3;
    set listc = { 'val one', 'val two' };
    set listc2 = { 'val three', 'val four' };
    set -l listc = $listc2;
    assert(str1: $listc[0], str2: 'val one');
    assert(str1: $listc[2], str2: 'val three');
    assert(str1: $listc[${index}], str2: 'val four');
  }

  // #4: List addition
  test(name: 'list test #4') {
    set listd = { 'val one', 'val two' } + { 'val three', 'val four' };
    assert(str1: $listd[0], str2: 'val one');
    assert(str1: $listd[2], str2: 'val three');
    assert(str1: $listd[3], str2: 'val four');
  }

  // #5: List of lists
  test(name: 'list test #5') {
    set liste = {
      { 'val one', 'val two' }, { 'val three', 'val four' }
    };
    assert(str1: $liste[0][0], str2: 'val one');
    assert(str1: $liste[0][1], str2: 'val two');
    assert(str1: $liste[1][0], str2: 'val three');
    assert(str1: $liste[1][1], str2: 'val four');
  }

  // #1: JSON Object syntax - maps to DM array
  test(name: 'json test #1') {
    set index = 'two';
    set jsonobj1 = {
        "one": "val one",
        "two": "val two",
        "three": 3,
        "four": true
    };
    assert(str1: $jsonobj1['one'], str2: 'val one');
    assert(str1: $jsonobj1[${index}], str2: 'val two');
    assert(expr1: $jsonobj1['three'], expr2: 3);
    assert(expr1: $jsonobj1['four'], expr2: true);
  }

  // #2: JSON Array syntax - maps to DM list
  test(name: 'json test #2') {
    set index = 1;
    set jsonarr1 = [ "val one", "val two", 3, true ];
    assert(str1: $jsonarr1[0], str2: 'val one');
    assert(str1: $jsonarr1[${index}], str2: 'val two');
    assert(expr1: $jsonarr1[2], expr2: 3);
    assert(expr1: $jsonarr1[3], expr2: true);
  }
}


action metadata_tests
{
  // #1: Meta-data - access environment
  test(name: 'meta test #1') {
    set name = ${environment.name};
    assert(str1: ${name}, str2: 'envA');
  }

  // #2: Meta-data - access server
  // Use psloop to push each server onto the stack and then put the name into a
  // global list
  test(name: 'meta test #2') {
    psloop {
      set -g meta2list = ${meta2list}${meta2list:+','}${server.name};
    }
    assert(str1: ${meta2list}, str2: 'server1,server2');
  }

  // #3: Meta-data - access servers via environment
  // Foreach iterates over the keys (the server names) and then we access
  // properties of the server object
  test(name: 'meta test #3') {
    foreach(server: ${environment.servers}) {
      set list = ${list}${list:+','}${server};
      set s = ${environment.servers[$server]};
      set list2 = ${list2}${list2:+','}${s.hostname};
      set list3 = ${list3}${list3:+','}${s.type};
    }
    assert(str1: ${list}, str2: 'server1,server2');
    assert(str1: ${list2}, str2: 'hostname1,hostname2');
    assert(str1: ${list3}, str2: 'windows,unix');
  }

  // #4: Meta-data: environment owner attributes
  test(name: 'meta test #4') {
    assert(str1: ${environment.owner.name}, str2: 'robert');
    assert(str1: ${environment.owner.email}, str2: 'rhthornburrow@trinem.com');
    assert(str1: ${environment.owner.realname}, str2: 'Robert Thornburrow');
  }

  // #5: Meta-data: user groups for user
  test(name: 'meta test #5') {
    foreach(group: ${environment.owner.groups}) {
      set list = ${list}${list:+','}${group};
      set list2 = ${list2}${list2:+','}${environment.owner.groups[$group].name};
    }
    assert(str1: ${list}, str2: 'Trinem_Administrators,EVERYONE,Group1,Group2,Developers');
    assert(str1: ${list2}, str2: 'Trinem_Administrators,EVERYONE,Group1,Group2,Developers');
  }

  // #6: Meta-data: server owner attributes
  test(name: 'meta test #6') {
    foreach(server: ${environment.servers}) {
      set list = ${list}${list:+','}${environment.servers[$server].owner.name};
      set list2 = ${list2}${list2:+','}${environment.servers[$server].owner.email};
    }
    assert(str1: ${list}, str2: 'robert,phil');
    assert(str1: ${list2}, str2: 'rhthornburrow@trinem.com,pgibbs@trinem.com');
  }

  // #7: Meta-data: environment domain attributes
  test(name: 'meta test #7') {
    assert(str1: ${environment.domain.name}, str2: 'TestEnvironments');
    assert(str1: ${environment.domain.owner.name}, str2: 'Group1');
    assert(str1: ${environment.domain.domain.name}, str2: 'Trinem');
    assert(str1: ${environment.domain.domain.owner.name}, str2: 'robert');
  }

  // #8: Meta-data: application domain attributes
  test(name: 'meta test #8') {
    assert(str1: ${application.domain.name}, str2: 'TestApps');
    assert(str1: ${application.domain.owner.name}, str2: 'robert');
    assert(str1: ${application.domain.domain.name}, str2: 'TestEnvironments');
    assert(str1: ${application.domain.domain.domain.name}, str2: 'Trinem');
  }

  // #9: Meta-data: environment domain attributes using object references
  test(name: 'meta test #9') {
    set names = { 1 => 'envA', 2 => 'TestEnvironments', 3 => 'Trinem' };
    set count = 1;
    set current = $environment;
    while(${current.name} != 'Trinem') {
      assert(str1: ${current.name}, str2: $names[$count]);
      set current = ${current.domain};
      set count = $count + 1;
    }
    assert(str1: ${current.name}, str2: $names[$count]);
  }

  // #10: Meta-data: fully qualified domains
  test(name: 'meta data #10') {
    assert(str1: ${environment.fqdomain}, str2: 'Global.Trinem.TestEnvironments');
    assert(str1: ${application.fqdomain}, str2: 'Global.Trinem.TestEnvironments.TestApps');
    foreach(server: ${environment.servers}) {
      set list = ${list}${list:+','}${environment.servers[$server].fqdomain};
    }
    assert(str1: ${list}, str2: 'Global.Trinem.TestEnvironments.TestServers,Global.Trinem.TestEnvironments.TestServers');
  }

  // #11: Meta-data: summaries
  test(name: 'meta data #11') {
    assert(str1: ${environment.summary}, str2: 'environment a');
    assert(str1: ${application.summary}, str2: 'application #2');
    foreach(server: ${environment.servers}) {
      set list = ${list}${list:+','}${environment.servers[$server].summary};
    }
    assert(str1: ${list}, str2: 'test server #1,test server #2');
  }

  // #12: Meta-data - access servers via reference to environment server array
  test(name: 'meta test #12') {
    set servers = ${environment.servers};
    foreach(server: $servers) {
      set list = ${list}${list:+','}${server};
    }
    assert(str1: ${list}, str2: 'server1,server2');
  }

  // #13: Meta-data - access servers via environment using old names
  // Foreach iterates over the keys (the server names) and then we access
  // properties of the server object
  test(name: 'meta test #13') {
    foreach(server: ${logical.physicals}) {
      set list = ${list}${list:+','}${server};
      set s = ${logical.physicals[$server]};
      set list2 = ${list2}${list2:+','}${s.hostname};
      set list3 = ${list3}${list3:+','}${s.type};
    }
    assert(str1: ${list}, str2: 'server1,server2');
    assert(str1: ${list2}, str2: 'hostname1,hostname2');
    assert(str1: ${list3}, str2: 'windows,unix');
  }

  // #14: Meta-data - repository meta-data
  test(name: 'meta test #14') {
    checkout(dropzone: 'meta14', repository: 'test', pattern: '*.java') {
      post {
        set -g meta14_reponame = ${repository.name};
        set -g meta14_repotype = ${repository.type};
        set -g meta14_broker   = ${repository.broker};
        set -g meta14_project  = ${repository.project};
        set -g meta14_state    = ${repository.state};
        set -g meta14_viewpath = ${repository.viewpath};
        set -g meta14_pattern  = ${deploy.pattern};
        set -g meta14_repovar  = ${myrepovar};
        set -g meta14_repovar2 = ${repository.myrepovar};
        set meta14_local = 'local';
      }
    }
    assert(str1: $meta14_reponame, str2: "test");
    assert(str1: $meta14_repotype, str2: "harvest");
    assert(str1: $meta14_broker,   str2: "linux-gnh2");
    assert(str1: $meta14_project,  str2: "Test");
    assert(str1: $meta14_state,    str2: "Development");
    assert(str1: $meta14_viewpath, str2: @"\test");
    assert(str1: $meta14_pattern,  str2: "*.java");
    assert(str1: $meta14_repovar,  str2: "myrepovalue");
    assert(str1: $meta14_repovar2, str2: "myrepovalue");
    assert(str1: $meta14_local, str2: "");
  }

  // #15: Meta-data - repository meta-data with statement args overriding
  test(name: 'meta test #15') {
    checkout(dropzone: 'meta15', repository: 'test', state: "$releaseState", viewpath: 'src', pattern: '*.java') {
      post {
        set -g meta15_reponame = ${repository.name};
        set -g meta15_repotype = ${repository.type};
        set -g meta15_broker   = ${repository.broker};
        set -g meta15_project  = ${repository.project};
        set -g meta15_state    = ${repository.state};
        set -g meta15_viewpath = ${repository.viewpath};
        set -g meta15_pattern  = ${deploy.pattern};
        set -g meta15_repovar  = ${myrepovar};
      }
    }
    assert(str1: $meta15_reponame, str2: "test");
    assert(str1: $meta15_repotype, str2: "harvest");
    assert(str1: $meta15_broker,   str2: "linux-gnh2");
    assert(str1: $meta15_project,  str2: "Test");
    assert(str1: $meta15_state,    str2: "Release");
    assert(str1: $meta15_viewpath, str2: @"\test\src");
    assert(str1: $meta15_pattern,  str2: "*.java");
    assert(str1: $meta15_repovar,  str2: "myrepovalue");
  }

  // #16: Meta-data: domain children attribute
  test(name: 'meta test #16') {
    assert(str1: ${application.domain.domain.children['TestApps'].name}, str2: 'TestApps');
    assert(str1: ${application.domain.domain.children['TestServers'].name}, str2: 'TestServers');
  }
}


action objectref_tests
{
  // #1: Set variable to null
  test(name: 'null #1') {
    set var1 = null;
    assert(obj1: $var1, obj2: null);
  }

  // #2: null comparison
  test(name: 'null #2') {
    set var1 = null;
    assert(expr: $var1 = null);
  }

  // #3: null in array
  test(name: 'null #3') {
    set arr1 = { 'foo' => null };
    assert(expr: $arr1['foo'] = null);
  }

  // #4: null in list
  test(name: 'null #4') {
    set list1 = { null };
    assert(expr: $list1[0] = null);
  }

  // #5: dereference null for property
  test(name: 'null #5') {
    try {
      set var1 = null;
      set var2 = ${var1.name};
      assert(expr: false);
    } catch(e) {
      assert(str1: $e, str2: 'null reference');
    }
  }

  // #6: dereference null for function
  test(name: 'null #6') {
    try {
      set var1 = null;
      set var2 = ${var1.length()};
      assert(expr: false);
    } catch(e) {
      assert(str1: $e, str2: 'null reference');
    }
  }

  // #7: null equality comparison with non-null
  test(name: 'null #7') {
    assert(expr: $application != null);
  }

  // #8: null greater than comparison with non-null
  test(name: 'null #8') {
    assert(expr: $application > null);
  }

  // #9: null less than comparison with non-null
  test(name: 'null #9') {
    assert(expr: null < $application);
  }

  // #10: inequality comparison of two non-nulls
  test(name: 'null #10') {
    assert(expr: $environment != $application);
  }

  // #11: equality comparison of two non-nulls
  test(name: 'null #11') {
    assert(expr: $application = $application);
  }

  // #12: not null
  test(name: 'null #12') {
    assert(expr1: !null, expr2: true);
  }

  // #13: not non-null
  test(name: 'null #13') {
    assert(expr1: !$application, expr2: false);
  }

  // #14: not undefined
  test(name: 'null #14') {
    assert(expr1: !$undefined, expr2: true);
  }
}


action loop_tests
{
  // #1: Psloop within psloop should only iterate over a single server
  test(name: 'psloop test #1') {
    psloop {
      //echo "Outer: ${TRIDM_PHYSICAL_SERVER}";
      set -g outer2 = ${outer2}${outer2:+' '}${TRIDM_PHYSICAL_SERVER};
      psloop {
        //echo "Inner: ${TRIDM_PHYSICAL_SERVER}";
        set -g inner2 = ${inner2}${inner2:+' '}${TRIDM_PHYSICAL_SERVER};
      }
    }
    assert(str1: ${outer2}, str2: ${inner2});
  }
}


action stream_tests
{
  // #1: Write a simple string to a stream and read it back
  test(name: 'stream test #1') {
    echo 'Hello World!' >> $stream1;
    assert(str1: $stream1, str2: 'Hello World!');
  }

  // #2: Write a multi-line message to a stream
  test(name: 'stream test #2') {
    set lines = { 1 => 'Hello World!', 2 => 'Line two.' };
    echo $lines[1] >> $stream2;
    echo $lines[2] >> $stream2;
    //echo "stream contains: '$stream2'";
    set n = 0;
    foreach(line: $stream2) {
    	set n = $n + 1;
    	assert(str1: $line, str2: $lines[$n]);
    }
  }

  // #3: Write a multi-line message to a stream using using
  test(name: 'stream test #3') {
    using stream $stream3 {
      echo 'Hello World!';
      using stream $stream4 {
        echo 'Line for another stream.';
      }
      echo 'Line two.';
    }
    assert(str1: substr($stream3, 0, 12), str2: 'Hello World!');
    assert(str1: substr($stream3, 13, 9), str2: 'Line two.');
    assert(str1: $stream4, str2: 'Line for another stream.');
  }

  // #4: Assign a stream to another local variable - should be deleted when test goes out of scope
  test(name: 'stream test #4') {
    echo 'Hello World!' >> $stream4;
    set stream4b = $stream4;
    assert(str1: $stream4b, str2: 'Hello World!');
  }

  // #5: Assign a stream to a global variable - should be deleted at exit
  test(name: 'stream test #5') {
    echo 'Hello World!' >> $stream5;
    set -g stream5b = $stream5;
    assert(str1: $stream5b, str2: 'Hello World!');
  }

  // #6: Assign a stream to another local variable and append using local var
  test(name: 'stream test #6') {
    echo 'Hello World!' >> $stream6;
    set stream6b = $stream6;
    echo 'Goodbye World!' >> $stream6b;
    assert(str1: $stream6, str2: 'Hello World!\nGoodbye World!');
  }
}


action date_tests
{
  // #1: Create a known date and time
  test(name: 'date test #1') {
    set -g now1 = to_date(2013, 8, 29, 17, 39, 12);
    if($TRIDM_PLATFORM = 'Windows') {
      assert(str1: "$now1", str2: 'Thursday, August 29, 2013 17:39:12');
    } else {
      assert(str1: "$now1", str2: 'Thu Aug 29 17:39:12 2013');
    }
  }

  // #2: Add an offset to a known date and time
  test(name: 'date test #2') {
    set now2 = $now1 + 25*3600;
    if($TRIDM_PLATFORM = 'Windows') {
      assert(str1: "$now2", str2: 'Friday, August 30, 2013 18:39:12');
    } else {
      assert(str1: "$now2", str2: 'Fri Aug 30 18:39:12 2013');
    }
  }

  // #3: Subtract an offset from a known date and time
  test(name: 'date test #3') {
    set now3 = $now1 - 25*3600;
    if($TRIDM_PLATFORM = 'Windows') {
      assert(str1: "$now3", str2: 'Wednesday, August 28, 2013 16:39:12');
    } else {
      assert(str1: "$now3", str2: 'Wed Aug 28 16:39:12 2013');
    }
  }

  // #4: Subtract two known date and times
  test(name: 'date test #4') {
    set -g now4 = to_date(2013, 8, 29, 19, 39, 12);
    set diff4 = $now4 - $now1;
    assert(expr1: $diff4, expr2: 7200);
  }

  // #5: Compare two known date and times using greater than
  test(name: 'date test #5') {
    assert(expr1: $now4 > $now1, expr2: true);
    assert(expr1: $now1 > $now4, expr2: false);
  }

  // #6: Compare two known date and times using less than
  test(name: 'date test #6') {
    assert(expr1: $now1 < $now4, expr2: true);
    assert(expr1: $now4 < $now1, expr2: false);
  }

  // #7: Stringify date and time - default format
  test(name: 'date test #7') {
    if($TRIDM_PLATFORM = 'Windows') {
      assert(str1: to_char($now1), str2: 'Thursday, August 29, 2013 17:39:12');
    } else {
      assert(str1: to_char($now1), str2: 'Thu Aug 29 17:39:12 2013');
    }
  }

  // #8: Stringify date and time - object syntax, default format
  test(name: 'date test #8') {
    if($TRIDM_PLATFORM = 'Windows') {
      assert(str1: ${now1.to_char()}, str2: 'Thursday, August 29, 2013 17:39:12');
    } else {
      assert(str1: ${now1.to_char()}, str2: 'Thu Aug 29 17:39:12 2013');
    }
  }

  // #9: Stringify date and time - user-defined format
  test(name: 'date test #9') {
    assert(str1: to_char($now1, '%Y%m%d %H%M%S'), str2: '20130829 173912');
  }

  // #10: Stringify date and time - object syntax, user-defined format
  test(name: 'date test #10') {
    assert(str1: ${now1.to_char('%Y%m%d %H%M%S')}, str2: '20130829 173912');
  }

  // #11: Stringify date and time - user-defined format with offset (PST)
  test(name: 'date test #11') {
    assert(str1: to_char($now1, '%Y%m%d %H%M%S', -8), str2: '20130829 093912');
  }

  // #12: Stringify date and time - object syntax, user-defined format with offset (PST)
  test(name: 'date test #12') {
    assert(str1: ${now1.to_char('%Y%m%d %H%M%S', -8)}, str2: '20130829 093912');
  }
}


action notify_tests
{
  using stream $body {
    echo "This is a test notification";
  }

  //notify(notifier: 'email', to: [ 'rhthornburrow@trinem.com', 'pgibbs@trinem.com' ],
  //       subject: 'Test Notification', from: 'dm2@trinem.com', body: $body, logfile: 'smtp.log');

  notify(notifier: 'email', to: ${environment.owner},
         subject: 'Test Notification', from: 'dm2@trinem.com', body: $body, logfile: 'smtp.log');

  return;

  //using stream $body {
  //  echo "Dear ${environment.owner.name},";
  //  echo '';
  //  echo "This is a test email message to say that the application ${application.name} to environment ${environment.name}";
  //  echo '';
  //  echo 'Best regards,';
  //  echo '';
  //  echo '     Deployment Manager 2';
  //}
  //
  //notify(to: ${environment.owner}, // cc: 'pgibbs@trinem.com',
  //       subject: 'Test Notification', from: 'dm2@trinem.com',
  //       mailserver: ${cmdln_mailserver}, username: ${cmdln_mailuser},
  //       password: ${cmdlin_mailpass}, body: $body, logfile: 'smtp.log');

  using stream $attachment {
    echo 'This is a file attachment.';
  }

  notify(notifier: 'email', //to: ${environment.owner}, // cc: 'pgibbs@trinem.com',
         subject: 'Test Notification', from: 'dm2@trinem.com',
         attachment: $attachment, logfile: 'smtp.log' /*, mailserver: 'unused'*/) {
    echo "Dear ${environment.owner.name},";
    echo '';
    echo "This is a test email message from DM2 on ${TRIDM_PLATFORM} to say";
    echo "that the application ${application.name} has been deployed to ";
    echo "the environment ${environment.name}. mynotifyvar is ${mynotifyvar}.";
    echo '';
    echo 'Best regards,';
    echo '';
    echo '     Deployment Manager 2';
    echo "     (sent via ${notify.type} ${notify.mailserver})";

    attachment(name: 'attachment2.txt', body: $attachment);

    attachment(name: 'attachment3.txt') {
      echo 'This is the third file attachment';
    }

    attachment(name: 'trilogy.log') {
      //trilogy(jobname: 'SIMPLE_DIR');
    }
  }
}


action sms_tests
{
  notify(notifier: 'sms', //to: ${environment.owner},
         from: 'Trinem', logfile: 'sms.log') {
    echo "Application ${application.name} has been deployed to environment ${environment.name}";
  }
}


action trilogy_tests
{
  /*using stream $script_error4 {
    trilogy(jobname: 'LOCAL_SCRIPT_ERROR4');
  }
  echo $?;
  echo $script_error4;*/

  test(name: 'trilogy tests #1') {
    try {
      trilogy;
    } catch(e) {
      set msg = ${e.message};
      set kind = ${e.kind};
    }
    assert(str1: $msg, str2: 'trilogy requires a jobname');
    assert(str1: $kind, str2: 'RuntimeError');
  }
}


action script_tests
{
  //remotescript(filepath: 'ls', params: { '-ld', $pattern } );

  test(name: 'script tests #1') {
    psloop {
      echo "${server.name}:" >> $messages;

      using stream $input {
        remotescript(filepath: '/tmp/arrayout.sh') {
          post {
            echo "Exit code: $?" >> $messages;
          }
        }
        echo "Overall exit code: $?" >> $messages;
      }

      echo "$messages";

      try {
        eval("set arrayout = $input;");
        echo $arrayout;
      } catch(e) {
        echo $e;
      }
    }
  }

  test(name: 'script tests #2') {
    if($sql = 1) {
      try {
        set result = testremotefunc();
        echo "Exit code: $?";
        foreach(s : $result) {
          echo "$s => ${result[$s]}";
        }
      } catch(e) {
        echo $e;
        assert(expr: false);
      }
      assert(str1: $result['server2'], str2: '"ONE" "TWO"');
    }
  }

  test(name: 'script tests #3') {
    psloop {
      try {
        set result = testremotefunc();
        echo "Exit code: $?";
        echo "${server.name} returned $result";
      } catch(e) {
        echo $e;
        assert(expr: false);
      }
      if(${server.name} = "server2") {
        assert(str1: $result, str2: '"ONE" "TWO"');
      } else {
        assert(str1: rtrim($result), str2: 'Permission denied (publickey,keyboard-interactive).');
      }
    }
  }
}


action query_tests
{
  // #1: Connect to database and run simple query putting results in 2D array
  test(name: 'query test #1') {
    try {
      query(datasource: 'dm', sql: 'select * from dm.dm_user', result: 'row') {
        //echo "${row['id']}, ${row['name']}, ${row['email']}";
        set data[${row['id']}] = { 'name' => ${row['name']}, 'email' => ${row['email']} };
        set datasourcevar = $mydatasourcevar;
      }
    } catch(e) {
      echo $e;
      assert(expr: false);
    }
    assert(str1: $data[1]['name'], str2: 'robert');
    assert(str1: $data[2]['name'], str2: 'phil');
    assert(str1: $data[1]['email'], str2: 'rhthornburrow@trinem.com');
    assert(str1: $data[2]['email'], str2: 'pgibbs@trinem.com');
    assert(str1: $datasourcevar, str2: 'mydatasourcevalue');
  }

  // #2: Connect to Harvest database and run simple query putting results in 2D array
  test(name: 'query test #2') {
    try {
      query(datasource: 'harvest', sql: 'select * from haruser', result: 'row') {
        //echo "${row['usrobjid']}, ${row['username']}"; //, ${row['email']}";
        set data[${row['usrobjid']}] = { 'username' => ${row['username']} }; //, 'email' => ${row['email']} };
      }
    } catch(e) {
      echo $e;
      assert(expr: false);
    }
    assert(str1: $data[1]['username'], str2: 'harvest');
    //assert(str1: $data[2]['username'], str2: 'phil');
    //assert(str1: $data[1]['email'], str2: 'rhthornburrow@trinem.com');
    //assert(str1: $data[2]['email'], str2: 'pgibbs@trinem.com');
  }

  // #3: Connect to database and run simple update query - should be denied
  test(name: 'query test #3') {
    try {
      query(datasource: 'dm', sql: 'update public.rht_temp set test = now()', result: 'row');
      assert(expr: false);
    } catch(e) {
      set msg = ${e.message};
      set kind = ${e.kind};
    }
    assert(str1: $msg, str2: "No write access to datasource 'dm'");
    assert(str1: $kind, str2: 'PermissionDeniedException');
  }
}



/**
 * testscript_*  - test.cmd - arg1 required, arg2 required, arg3 padded, arg4 padded
 * testscript2_* - test.cmd - arg1 required with switch -a, arg2 required with switch -b
 * testscript3_* - test.cmd - arg1 required, arg2 required, arg3 padded, arg4 optional
 * testscript4_* - test.cmd - arg1 padded with switch -a, arg2 optional with switch -b, argc is always -c
 * testscript5_* - test.cmd - arg1 optional boolean -a, arg2 optional boolean neg -b, arg3 optional boolean -c/-d, arg4 optional with neg -e
 */
action localscript_tests
{
  // #1: call a local script passing two required arguments
  test(name: 'localscript test #1') {
    using stream $localscript1 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript_win(arg1: 'hello', arg2: 'world');
      } else {
        testscript_unix(arg1: 'hello', arg2: 'world');
      }
    }
    assert(str1: rtrim($localscript1),
           str2: '%1 = [hello] %2 = [world] %3 = [] %4 = []'
                 ' ARG1 = [hello] ARG2 = [world] ARG3 = [] ARG4 = []');
  }

  // #2: call a local script passing two required and one padded argument
  test(name: 'localscript test #2') {
    using stream $localscript2 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript_win(arg1: 'goodbye', arg2: 'cruel', arg3: 'world');
      } else {
        testscript_unix(arg1: 'goodbye', arg2: 'cruel', arg3: 'world');
      }
    }
    assert(str1: rtrim($localscript2),
           str2: '%1 = [goodbye] %2 = [cruel] %3 = [world] %4 = []'
                 ' ARG1 = [goodbye] ARG2 = [cruel] ARG3 = [world] ARG4 = []');
  }

  // #3: call a local script omitting a required argument - should error
  test(name: 'localscript test #3') {
    try {
      // arg1 is missing
      testscript_win(arg2: 'hello', arg3: 'world');
      assert(expr: false);
    } catch(e) {
      assert(str1: $e, str2: "Required arg 'arg1' must be specified for action 'testscript_win'");	// was localscript
    }
  }

  // #4: call a local script with a missing optional argument - should pad
  test(name: 'localscript test #4') {
    using stream $localscript4 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript_win(arg1: 'goodbye', arg2: 'cruel', arg4: 'world');
      } else {
        testscript_unix(arg1: 'goodbye', arg2: 'cruel', arg4: 'world');
      }
    }
    assert(str1: rtrim($localscript4),
           str2: '%1 = [goodbye] %2 = [cruel] %3 = [] %4 = [world]'
                 ' ARG1 = [goodbye] ARG2 = [cruel] ARG3 = [] ARG4 = [world]');
  }

  // #5: call a local script passing arguments with switches
  test(name: 'localscript test #5') {
    using stream $localscript5 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript2_win(arg1: 'hello', arg2: 'world');
      } else {
        testscript2_unix(arg1: 'hello', arg2: 'world');
      }
    }
    assert(str1: rtrim($localscript5),
           str2: '%1 = [-a] %2 = [hello] %3 = [-b] %4 = [world]'
                 ' ARG1 = [hello] ARG2 = [world] ARG3 = [] ARG4 = []');
  }

  // #6: call a local script function passing two arguments
  test(name: 'localscript test #6') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = funcscript_win('hello', 'world');
    } else {
      set result = funcscript_unix('hello', 'world');
    }
    assert(str1: rtrim($result),
           str2: '%1 = [hello] %2 = [world]'
                 ' ARG1 = [hello] ARG2 = [world]');
  }

  if($TRIDM_PLATFORM = 'Windows') {
    // #7: call a local VBScript passing two required arguments
    test(name: 'localscript test #7') {
      using stream $localscript7 {
        testactionvbs(arg1: 'hello', arg2: 'world');
      }
      assert(str1: rtrim($localscript7),
             str2: '%1 = [hello] %2 = [world] %3 = [] %4 = []'
                   ' ARG1 = [hello] ARG2 = [world] ARG3 = [] ARG4 = []');
    }

    // #8: call a local script function passing two arguments
    test(name: 'localscript test #8') {
      set result = testfuncvbs('hello', 'world');
      assert(str1: rtrim($result),
             str2: '%1 = [hello] %2 = [world]'
                   ' ARG1 = [hello] ARG2 = [world]');
    }
  }

  // #9: call a local script function passing two args which returns text
  test(name: 'localscript test #9') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = testexprfunc1_win('hello', 'world');
    } else {
      set result = testexprfunc1_unix('hello', 'world');
    }
    assert(str1: rtrim($result),
           str2: '{ \'one\' => \'hello\', \'two\' => \'world\' }');
  }

  // #10: call a local script function passing two args which returns an expr
  test(name: 'localscript test #10') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = testexprfunc2_win('hello', 'world');
    } else {
      set result = testexprfunc2_unix('hello', 'world');
    }
    assert(str1: $result['one'], str2: 'hello');
    assert(str1: $result['two'], str2: 'world');
  }

  // #11: call a local script passing two required arguments - padding will make three args
  test(name: 'localscript test #11') {
    using stream $localscript11 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript3_win(arg1: 'hello', arg2: 'world');
      } else {
        testscript3_unix(arg1: 'hello', arg2: 'world');
      }
    }
    assert(str1: rtrim($localscript11),
           str2: '%# = [3] %1 = [hello] %2 = [world] %3 = [] %4 = []'
                 ' ARG1 = [hello] ARG2 = [world] ARG3 = [] ARG4 = []');
  }

  // #12: call a local script passing two required arguments and one padded
  test(name: 'localscript test #12') {
    using stream $localscript12 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript3_win(arg1: 'hello', arg2: 'padded', arg3: 'world');
      } else {
        testscript3_unix(arg1: 'hello', arg2: 'padded', arg3: 'world');
      }
    }
    assert(str1: rtrim($localscript12),
           str2: '%# = [3] %1 = [hello] %2 = [padded] %3 = [world] %4 = []'
                 ' ARG1 = [hello] ARG2 = [padded] ARG3 = [world] ARG4 = []');
  }

  // #13: call a local script passing two required arguments, one padded and one optional
  test(name: 'localscript test #13') {
    using stream $localscript13 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript3_win(arg1: 'hello', arg2: 'padded', arg3: 'world', arg4: 'with options');
      } else {
        testscript3_unix(arg1: 'hello', arg2: 'padded', arg3: 'world', arg4: 'with options');
      }
    }
    assert(str1: rtrim($localscript13),
           str2: '%# = [4] %1 = [hello] %2 = [padded] %3 = [world] %4 = [with options]'
                 ' ARG1 = [hello] ARG2 = [padded] ARG3 = [world] ARG4 = [with options]');
  }

  // #14: call a local script passing two required arguments, and one optional - padding will make 4 args
  test(name: 'localscript test #14') {
    using stream $localscript14 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript3_win(arg1: 'goodbye', arg2: 'padded', arg4: 'world');
      } else {
        testscript3_unix(arg1: 'goodbye', arg2: 'padded', arg4: 'world');
      }
    }
    assert(str1: rtrim($localscript14),
           str2: '%# = [4] %1 = [goodbye] %2 = [padded] %3 = [] %4 = [world]'
                 ' ARG1 = [goodbye] ARG2 = [padded] ARG3 = [] ARG4 = [world]');
  }

  // #15: call a local script passing no arguments - padding and always will make three args
  test(name: 'localscript test #15') {
    using stream $localscript15 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript4_win;
      } else {
        testscript4_unix;
      }
    }
    assert(str1: rtrim($localscript15),
           str2: '%# = [3] %1 = [-a] %2 = [] %3 = [-c] %4 = []'
                 ' ARG1 = [] ARG2 = [] ARG3 = [] ARG4 = []');
  }

  // #16: call a local script passing padded arg - always will make three args
  test(name: 'localscript test #16') {
    using stream $localscript16 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript4_win(arg1: 'hello');
      } else {
        testscript4_unix(arg1: 'hello');
      }
    }
    assert(str1: rtrim($localscript16),
           str2: '%# = [3] %1 = [-a] %2 = [hello] %3 = [-c] %4 = []'
                 ' ARG1 = [hello] ARG2 = [] ARG3 = [] ARG4 = []');
  }

  // #17: call a local script passing optional arg - padding and always will make four args
  test(name: 'localscript test #17') {
    using stream $localscript17 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript4_win(arg2: 'world');
      } else {
        testscript4_unix(arg2: 'world');
      }
    }
    assert(str1: rtrim($localscript17),
           str2: '%# = [4] %1 = [-a] %2 = [] %3 = [-bworld] %4 = [-c]'
                 ' ARG1 = [] ARG2 = [world] ARG3 = [] ARG4 = []');
  }

  // #18: try to pass an arg with the same name at the always arg
  test(name: 'localscript test #18') {
     try {
      testscript4_win(arg3: 'error');
      assert(expr: false);
    } catch(e) {
      assert(str1: $e, str2: "Arg 'arg3' is unknown for action 'testscript4_win'");
    }
  }

  // #19: boolean args and neg switches - no args passed - neg switches should output
  test(name: 'localscript test #19') {
    using stream $localscript19 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript5_win;
      } else {
        testscript5_unix;
      }
    }
    assert(str1: rtrim($localscript19),
           str2: '%# = [1] %1 = [-e] %2 = [] %3 = [] %4 = []'
                 ' ARG1 = [] ARG2 = [] ARG3 = [] ARG4 = []');
  }

  // #20: boolean args and neg switches - bool args all true - optional given
  test(name: 'localscript test #20') {
    using stream $localscript20 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript5_win(arg1: true, arg2: true, arg3: true, arg4: 'value');
      } else {
        testscript5_unix(arg1: true, arg2: true, arg3: true, arg4: 'value');
      }
    }
    assert(str1: rtrim($localscript20),
           str2: '%# = [3] %1 = [-a] %2 = [-c] %3 = [value] %4 = []'
                 ' ARG1 = [1] ARG2 = [1] ARG3 = [1] ARG4 = [value]');
  }

  // #21: boolean args and neg switches - bool args all false - optional not given
  test(name: 'localscript test #21') {
    using stream $localscript21 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript5_win(arg1: false, arg2: false, arg3: false);
      } else {
        testscript5_unix(arg1: false, arg2: false, arg3: false);
      }
    }
    assert(str1: rtrim($localscript21),
           str2: '%# = [3] %1 = [-b] %2 = [-d] %3 = [-e] %4 = []'
                 ' ARG1 = [0] ARG2 = [0] ARG3 = [0] ARG4 = []');
  }

  // #22: call a local script function passing two required arguments - padding will make three args
  test(name: 'localscript test #22') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = funcscript2_win('hello', 'world');
    } else {
      set result = funcscript2_unix('hello', 'world');
    }
    assert(str1: rtrim($result),
           str2: '%# = [3] %1 = [hello] %2 = [world] %3 = [] %4 = []'
                 ' ARG1 = [hello] ARG2 = [world] ARG3 = [] ARG4 = []');
  }

  // #23: call a local script function passing two required arguments and one padded
  test(name: 'localscript test #23') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = funcscript2_win('hello', 'padded', 'world');
    } else {
      set result = funcscript2_unix('hello', 'padded', 'world');
    }
    assert(str1: rtrim($result),
           str2: '%# = [3] %1 = [hello] %2 = [padded] %3 = [world] %4 = []'
                 ' ARG1 = [hello] ARG2 = [padded] ARG3 = [world] ARG4 = []');
  }

  // #24: call a local script function passing two required arguments, one padded and one optional
  test(name: 'localscript test #24') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = funcscript2_win('hello', 'padded', 'world', 'with options');
    } else {
      set result = funcscript2_unix('hello', 'padded', 'world', 'with options');
    }
    assert(str1: rtrim($result),
           str2: '%# = [4] %1 = [hello] %2 = [padded] %3 = [world] %4 = [with options]'
                 ' ARG1 = [hello] ARG2 = [padded] ARG3 = [world] ARG4 = [with options]');
  }

  // #25: call a local script function passing one of two required arguments
  test(name: 'localscript test #25') {
    try {
      set result = funcscript2_win('goodbye');
      assert(expr: false);
    } catch(e) {
      assert(str1: $e, str2: "Function 'funcscript2_win' requires at least 2 arguments");
    }
  }

  // #26: call a local script function passing no arguments - padding and always will make three args
  test(name: 'localscript test #26') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = funcscript3_win();
    } else {
      set result = funcscript3_unix();
    }
    assert(str1: rtrim($result),
           str2: '%# = [3] %1 = [-a] %2 = [] %3 = [-c] %4 = []'
                 ' ARG1 = [] ARG2 = [] ARG3 = [] ARG4 = []');
  }

  // #27: call a local script function passing padded arg - always will make four args
  test(name: 'localscript test #27') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = funcscript3_win('world', 'hello');
    } else {
      set result = funcscript3_unix('world', 'hello');
    }
    assert(str1: rtrim($result),
           str2: '%# = [4] %1 = [-a] %2 = [hello] %3 = [-bworld] %4 = [-c]'
                 ' ARG1 = [world] ARG2 = [hello] ARG3 = [] ARG4 = []');
  }

  // #28: call a local script function passing optional arg - padding and always will make four args
  test(name: 'localscript test #28') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = funcscript3_win('hello');
    } else {
      set result = funcscript3_unix('hello');
    }
    assert(str1: rtrim($result),
           str2: '%# = [4] %1 = [-a] %2 = [] %3 = [-bhello] %4 = [-c]'
                 ' ARG1 = [hello] ARG2 = [] ARG3 = [] ARG4 = []');
  }

  // #29: try to pass an extra arg
  test(name: 'localscript test #29') {
    try {
      set result = funcscript3_win('hello', 'cruel', 'world');
      assert(expr: false);
    } catch(e) {
      assert(str1: $e, str2: "Argument 3 to function 'funcscript3_win' is unexpected");
    }
  }

  // #30: boolean args and neg switches - no args passed - neg switches should output
  test(name: 'localscript test #30') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = funcscript4_win();
    } else {
      set result = funcscript4_unix();
    }
    assert(str1: rtrim($result),
           str2: '%# = [1] %1 = [-e] %2 = [] %3 = [] %4 = []'
                 ' ARG1 = [] ARG2 = [] ARG3 = [] ARG4 = []');
  }

  // #31: boolean args and neg switches - bool args all true - optional given
  test(name: 'localscript test #31') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = funcscript4_win(true, true, true, 'value');
    } else {
      set result = funcscript4_unix(true, true, true, 'value');
    }
    assert(str1: rtrim($result),
           str2: '%# = [3] %1 = [-a] %2 = [-c] %3 = [value] %4 = []'
                 ' ARG1 = [1] ARG2 = [1] ARG3 = [1] ARG4 = [value]');
  }

  // #32: boolean args and neg switches - bool args all false - optional not given
  test(name: 'localscript test #32') {
    if($TRIDM_PLATFORM = 'Windows') {
      set result = funcscript4_win(false, false, false);
    } else {
      set result = funcscript4_unix(false, false, false);
    }
    assert(str1: rtrim($result),
           str2: '%# = [3] %1 = [-b] %2 = [-d] %3 = [-e] %4 = []'
                 ' ARG1 = [0] ARG2 = [0] ARG3 = [0] ARG4 = []');
  }

  // #33: boolean args and neg switches - bool args all true - optional given
  test(name: 'localscript test #33') {
    using stream $localscript33 {
      if($TRIDM_PLATFORM = 'Windows') {
        testscript6_win(arg1: true, arg2: true, arg3: true, arg4: 'value');
      } else {
        testscript6_unix(arg1: true, arg2: true, arg3: true, arg4: 'value');
      }
    }
    assert(str1: rtrim($localscript33),
           str2: '%# = [4] %1 = [-foo] %2 = [-aA] %3 = [-c] %4 = [value]'
                 ' ARG1 = [1] ARG2 = [1] ARG3 = [1] ARG4 = [value]');
  }
}


/*function control_paths_1()
{
  abort(msg: 'wibble');
}


function control_paths_2()
{
  try {
    return true;
  } catch(e) {}
}


function control_paths_3()
{
  try {
    abort(msg: 'wibble');
  } catch(e) {}
}


function control_paths_4()
{
  for(n = 0; $n < 6; n = $n + 1) {
    return true;
  }
}


function control_paths_5()
{
  for(n = 0; $n < 6; n = $n + 1) {
    if(true) {
      return true;
    } else {
      return false;
    }
  }
}


function control_paths_6()
{
  for(n = 0; $n < 6; n = $n + 1) {
    if(true) {
      return true;
    }
  }
}*/


action dbscript_tests
{
  // #1: call a stored action in the database
  test(name: 'dbscript test #1') {
    using stream $dbscript1 {
      dbscript(arg1: 'hello', arg2: 'world');
    }
    assert(str1: $dbscript1, str2: 'arg1 = [hello] arg2 = [world]');
  }

  // #2: call a stored action in the database containing a syntax error
  test(name: 'dbscript test #2') {
    try {
      dbscript2(arg1: 'hello', arg2: 'world');
      assert(expr: false);
    } catch(e) {
      set msg = ${e.message};
      set kind = ${e.kind};
    }
    assert(str1: $msg, str2: '>>>123<<<;\nsyntax error, unexpected integer value');
    assert(str1: $kind, str2: 'SyntaxError');
  }

  // #3: call a stored function in the database
  test(name: 'dbscript test #3') {
    assert(str1: dbfunc('hello'), str2: 'prefix_hello');
  }

  // #1: if/then/else - all control paths return a value
  test(name: 'control path returns #1') {
    set okay = false;
    try {
      set x = dbfunc2();
      set okay = true;
    } catch(e) {
      assert(expr: false);
    }
    assert(expr1: $okay, expr2: true);
    assert(expr1: $x, expr2: true);
  }

  // #2: if/then - all control paths return a value
  test(name: 'control path returns #2') {
    set okay = false;
    try {
      set x = dbfunc3();
      set okay = true;
    } catch(e) {
      assert(expr: false);
    }
    assert(expr1: $okay, expr2: true);
    assert(expr1: $x, expr2: true);
  }

  // #3: if/then - not all control paths return a value
  test(name: 'control path returns #3') {
    set okay = true;
    try {
      set x = dbfunc4();
      set okay = false;
    } catch(e) {
      set msg = ${e.message};
      set kind = ${e.kind};
    }
    assert(expr1: $okay, expr2: true);
    assert(str1: $msg, str2: 'Not all control paths return a value');
    assert(str1: $kind, str2: 'SyntaxError');
  }

  // #4: if/then - function with a return that doesn't specify a value
  test(name: 'control path returns #4') {
    set okay = true;
    try {
      set x = dbfunc5();
      set okay = false;
    } catch(e) {
      set msg = ${e.message};
      set kind = ${e.kind};
    }
    assert(expr1: $okay, expr2: true);
    assert(str1: $msg, str2: 'function return must specify a value');
    assert(str1: $kind, str2: 'SyntaxError');
  }
}


action eval_tests
{
  // #1: simple eval test to run an echo
  test(name: 'eval test #1') {
    using stream $eval1 {
      eval("echo 'hello world';");
    }
    assert(str1: $eval1, str2: 'hello world');
  }

  // #2: simple eval test to defer expansion of variables
  //     string expands to "$y", so output should be contents of 'y'
  test(name: 'eval test #2') {
    set y = "foo";
    set x = "y";
    using stream $eval2 {
      eval("echo \"\$$x\";");
    }
    assert(str1: $eval2, str2: 'foo');
  }

  // #3: simple eval to set a variable
  //     string expands to "set foo = $bar", so $foo should contain 'bat'
  test(name: 'eval test #3') {
    set x = "foo";
    set y = "bar";
    set bar = "bat";
    eval("set $x = \$$y;");
    assert(str1: $foo, str2: 'bat');
  }

  // #4: eval of a stream
  test(name: 'eval test #4') {
    set x = "y";
    echo "set y = \"foo\";"
         "echo \"\$$x\";" >> $eval4a;
    using stream $eval4b {
      eval($eval4a);
    }
    assert(str1: $eval4b, str2: 'foo');
  }
}


action modify_tests
{
  set id_map = { 'a' => 'one', 'b' => 'two', 'c' => 'three', 'd' => 'four' };
  set myvalue = 'stem1';
  set mytext = 'This is a leaf';

  using dropzone 'fred' {
    echo "dropzone 'fred' is at: ${dropzone.path}";

    checkout(/*dropzone: 'fred',*/ repository: 'local', pattern: 'testxmlinput.xml');

    echo "fred contains: ${dropzone.files}";
    echo "fred.find('*.sql'): ${dropzone.find('*.sql')}";
    echo "fred.find('*.xml'): ${dropzone.find('*.xml')}";
    foreach(file : ${dropzone.find('*.xml')}) {
      echo "  $file";
    }

    checkout(dropzone: 'jim', repository: 'local', pattern: 'testxmlinput.xml.1');

    echo "fred now contains: ${dropzone.files}";
    using dropzone 'jim' {
      echo "jim contains: ${dropzone.files}";
    }

    abort;

//    modify(/*dropzone: 'fred',*/ modifier: 'xml',
//           file: 'testxmlinput.xml', outfile: 'testxmlinput.xml.1') {
//      set_attribute(xpath: '/root/trunk[@attr]/@attr', value: 'hello world');	// was ${physical.hostname}
//      set_attribute(xpath: '/root/trunk/@attr2', value: 'two');
//      remove_element(xpath: '/root/trunk/branch[2]');
//      remove_element(xpath: '/root/trunk/branch[@id=3]');
//      set_text(xpath: '/root/trunk/branch', value: 'goodbye');		// was ${physical.name}
//      add_element(xpath: '/root/trunk/branch', pos: 'before',
//        value: "<stem id=\"${myvalue}\">"
//               "<!-- comment ${mycomment} -->"
//               "<leaf>${mytext}</leaf>"
//               "</stem>");
//      foreach(id : $id_map) {
//        set_attribute(xpath: "/root/trunk/knot[@id=${id}]/@attr", value: ${id_map[$id]});
//      }
//    }
//
//    using stream $mod1 {
//      diff(options: '-w', left: 'testxmlinput.xml', right: 'testxmlinput.xml.1');
//    }
//
//    modify(/*dropzone: 'fred',*/ modifier: 'xml',
//           file: 'testxmlinput.xml', outfile: 'testxmlinput.xml.1', serverspecific: true) {
//      set_attribute(xpath: '/root/trunk[@attr]/@attr', value: ${server.hostname});
//      set_attribute(xpath: '/root/trunk/@attr2', value: 'two');
//      remove_element(xpath: '/root/trunk/branch[2]');
//      remove_element(xpath: '/root/trunk/branch[@id=3]');
//      set_text(xpath: '/root/trunk/branch', value: ${server.name});
//      foreach(id : $id_map) {
//        set_attribute(xpath: "/root/trunk/knot[@id=${id}]/@attr", value: ${id_map[$id]});
//      }
//    }
//
//    //transfer/*(dropzone: 'fred')*/;
  }

  test(name: 'modify test #1') {
    set mod1ref = {
      '3,15c3,18',
      '<   <trunk name="wibble">',
      '<   </trunk>',
      '<   <trunk attr="@replace@">',
      '<     <branch id="1">',
      '<       <twig></twig>',
      '<     </branch>',
      '<     <branch id="2" />',
      '<     <branch id="3" />',
      '<     <branch id="4" />',
      '<     <knot id="a" attr="@replace@" />',
      '<     <knot id="b" attr="@replace@" />',
      '<     <knot id="c" attr="@replace@" />',
      '<     <knot id="d" attr="@replace@" />',
      '---',
      '>     <trunk name="wibble" attr2="two" />',
      '>     <trunk attr="hello world" attr2="two">',
      '>         <stem id="stem1">',
      '>             <!-- comment  -->',
      '>             <leaf>This is a leaf</leaf>',
      '>         </stem>',
      '>         <branch id="1">goodbye</branch>',
      '>         <stem id="stem1">',
      '>             <!-- comment  -->',
      '>             <leaf>This is a leaf</leaf>',
      '>         </stem>',
      '>         <branch id="4">goodbye</branch>',
      '>         <knot id="a" attr="one" />',
      '>         <knot id="b" attr="two" />',
      '>         <knot id="c" attr="three" />',
      '>         <knot id="d" attr="four" />'
    };

    set ln = 0;
    foreach(line : $mod1) {
      assert(str1: $line, str2: $mod1ref[$ln]);
      incr ln;
    }
  }
}


action pathname_tests
{
  // UNIX
  // #1: basename - two levels
  test(name: 'unix pathname #1') {
    assert(str1: basename('/tmp/file.txt', 'unix'), str2: 'file.txt');
  }

  // #2: basename - one level
  test(name: 'unix pathname #2') {
    assert(str1: basename('/tmp', 'unix'), str2: 'tmp');
  }

  // #3: basename - root
  test(name: 'unix pathname #3') {
    assert(str1: basename('/', 'unix'), str2: '/');
  }

  // #4: dirname - two levels
  test(name: 'unix pathname #4') {
    assert(str1: dirname('/tmp/file.txt', 'unix'), str2: '/tmp');
  }

  // #5: dirname - one level
  test(name: 'unix pathname #5') {
    assert(str1: dirname('/tmp', 'unix'), str2: '/');
  }

  // #6: dirname - root
  test(name: 'unix pathname #6') {
    assert(str1: dirname('/', 'unix'), str2: '/');
  }

  // #7: append - one level with filename
  test(name: 'unix pathname #7') {
    assert(str1: append('/tmp', 'file.txt', 'unix'), str2: '/tmp/file.txt');
  }

  // #8: stdpath - two levels
  test(name: 'unix pathname #8') {
    assert(str1: stdpath('/tmp/file.txt', 'unix'), str2: '/tmp/file.txt');
  }

  // #9: appendstd - one level with relative filename
  test(name: 'unix pathname #9') {
    assert(str1: appendstd('/tmp', 'folder/file.txt', 'unix'), str2: '/tmp/folder/file.txt');
  }

  // WINDOWS
  // #1: basename - two levels
  test(name: 'windows pathname #1') {
    assert(str1: basename(@'C:\Temp\file.txt', 'windows'), str2: 'file.txt');
  }

  // #2: basename - one level
  test(name: 'windows pathname #2') {
    assert(str1: basename(@'C:\Temp', 'windows'), str2: 'Temp');
  }

  // #3: basename - two levels
  test(name: 'windows pathname #3') {
    assert(str1: basename(@'C:\', 'windows'), str2: @'C:\');
  }

  // #4: dirname - two levels
  test(name: 'windows pathname #4') {
    assert(str1: dirname(@'C:\Temp\file.txt', 'windows'), str2: @'C:\Temp');
  }

  // #5: dirname - one level
  test(name: 'windows pathname #5') {
    assert(str1: dirname(@'C:\Temp', 'windows'), str2: @'C:\');
  }

  // #6: dirname - root
  test(name: 'windows pathname #6') {
    assert(str1: dirname(@'C:\', 'windows'), str2: @'C:\');
  }

  // #7: append - one level with filename
  test(name: 'windows pathname #7') {
    assert(str1: append(@'C:\Temp', 'file.txt', 'windows'), str2: @'C:\Temp\file.txt');
  }

  // #8: stdpath - two levels
  test(name: 'windows pathname #8') {
    assert(str1: stdpath(@'C:\Temp\file.txt', 'windows'), str2: '/Temp/file.txt');
  }

  // #9: appendstd - one level with relative filename
  test(name: 'windows pathname #9') {
    assert(str1: appendstd(@'C:\Temp', 'folder/file.txt', 'windows'), str2: @'C:\Temp\folder\file.txt');
  }

  // VMS
  test(name: 'vms pathname #1') {
    assert(str1: basename('$2$DKA200:[FERDU01.TEST]file.txt', 'vms'), str2: 'file.txt');
  }

  // #2: basename - one level
  test(name: 'vms pathname #2') {
    assert(str1: basename('$2$DKA200:[FERDU01.TEST]', 'vms'), str2: 'TEST');
  }

  // #3: basename - two levels
  test(name: 'vms pathname #3') {
    assert(str1: basename('$2$DKA200:[FERDU01]', 'vms'), str2: '$2$DKA200:[FERDU01]');
  }

  // #4: dirname - two levels
  test(name: 'vms pathname #4') {
    assert(str1: dirname('$2$DKA200:[FERDU01.TEST]file.txt', 'vms'), str2: '$2$DKA200:[FERDU01.TEST]');
  }

  // #5: dirname - one level
  test(name: 'vms pathname #5') {
    assert(str1: dirname('$2$DKA200:[FERDU01.TEST]', 'vms'), str2: '$2$DKA200:[FERDU01]');
  }

  // #6: dirname - root
  test(name: 'vms pathname #6') {
    assert(str1: dirname('$2$DKA200:[FERDU01.TEST]', 'vms'), str2: '$2$DKA200:[FERDU01]');
  }

  // #7: append - one level with filename
  test(name: 'vms pathname #7') {
    assert(str1: append('$2$DKA200:[FERDU01]', 'file.txt', 'vms'), str2: '$2$DKA200:[FERDU01]file.txt');
  }

  // #8: stdpath - two levels
  test(name: 'vms pathname #8') {
    assert(str1: stdpath('$2$DKA200:[FERDU01.TEST]file.txt', 'vms'), str2: '/FERDU01/TEST/file.txt');
  }

  // #9: appendstd - one level with relative filename
  test(name: 'vms pathname #9') {
    assert(str1: appendstd('$2$DKA200:[FERDU01]', 'TEST/file.txt', 'vms'), str2: '$2$DKA200:[FERDU01.TEST]file.txt');
  }

  // Implicit server type tests
  // #1: basename - two levels - no server on stack
  test(name: 'implicit type pathname #1') {
    try {
      set bn = basename(@'C:\Temp\file.txt');
      assert(expr: false);
    } catch(e) {
      assert(expr: true);
    }
  }

  // #2: basename - two levels - server on stack
  test(name: 'implicit type pathname #2') {
    psloop {
      if(${server.type} = 'windows') {
        assert(str1: basename(@'C:\Temp\file.txt'), str2: 'file.txt');
      } else {
        assert(str1: basename('/tmp/file.txt'), str2: 'file.txt');
      }
    }
  }

  // #3: appendstd - one level with relative filename - server on stack
  test(name: 'implicit type pathname #3') {
    psloop {
      if(${server.type} = 'windows') {
        assert(str1: appendstd(@'C:\Temp', 'folder/file.txt'), str2: @'C:\Temp\folder\file.txt');
      } else {
        assert(str1: appendstd('/tmp', 'folder/file.txt'), str2: '/tmp/folder/file.txt');
      }
    }
  }
}


action component_tests
{
  // #1: using component - component name
  test(name: 'components #1') {
    using component 'testcomp' {
      assert(str1: ${component.name}, str2: 'testcomp');
    }
  }

  // #2: using component - server subset
  test(name: 'components #2') {
    using component 'testcomp' {
      psloop {
        set -g compsrvlist = ${compsrvlist}${compsrvlist:+','}${server.name};
      }
      assert(str1: ${compsrvlist}, str2: 'server1,server2');
    }
  }

  // #3: using component - component variables
  test(name: 'components #3') {
    using component 'testcomp' {
      assert(str1: $compvar, str2: 'component1');
    }
  }

  // #4: component metadata
  test(name: 'components #4') {
    using component 'testcomp' {
      foreach(k: ${component.servers}) {
        set compsrvlist2 = ${compsrvlist2}${compsrvlist2:+','}${component.servers[$k].name};
      }
      assert(str1: ${compsrvlist2}, str2: 'server1,server2');
    }
  }

  // #5: component item metadata
  test(name: 'components #5') {
    using component 'testcomp' {
      foreach(k: ${component.items}) {
        set comprepolist = ${comprepolist}${comprepolist:+','}${component.items[$k].repository};
        set comptgtlist = ${comptgtlist}${comptgtlist:+','}${component.items[$k].target};
      }
      assert(str1: ${comprepolist}, str2: 'local');
      assert(str1: ${comptgtlist}, str2: 'test');
    }
  }
}


action graphical_tests
{
  test(name: 'graphical #1') {
    try {
      graphtest;
    } catch(e) {
      echo $e;
      assert(expr: false);
    }
  }

  test(name: 'graphical #2') {
    try {
      graphtest2;
    } catch(e) {
      echo $e;
      assert(expr: false);
    }
  }

  test(name: 'graphical #3') {
    try {
      graphtest3;
    } catch(e) {
      echo $e;
      assert(expr: false);
    }
  }

  test(name: 'graphical #4') {
    try {
      graphtest4;
    } catch(e) {
      echo $e;
      assert(expr: false);
    }
  }

  test(name: 'graphical #5') {
    try {
      graphtest5;
    } catch(e) {
      echo $e;
      assert(expr: false);
    }
  }

  test(name: 'graphical #6') {
    try {
      graphtest6;
    } catch(e) {
      echo $e;
      assert(expr: false);
    }
  }
}


action iisplugin_tests
{
  // #1: iis_getversion function
  test(name: 'iisplugin tests #1') {
    assert(str1: iis_getversion(), str2: '1.23');
  }

  // #2: iis_getsites statement
  test(name: 'iisplugin tests #2') {
    iis_getsites;
  }
}


action task_tests
{
  runtask(task: 'Approve', approve: true,
  	note: "${task.name}: The application ${application.name} has been ${task.status} for use in the domain ${task.target.name}");
  //runtask(task: 'Approve', application: ${application}, approve: true, note: 'My note');
}


action remote_tests
{
  //set ds = diskspace(@'C:\', @'D:\');

  //extractzip(folder: '${basedir}', file: 'foo.zip');
/*
  set my = memory();
  echo "memory returned '$my'";

  psloop {
    set ps = processes();
    foreach(p: $ps) {
    	echo "processes[$p] = '${ps[$p].name}'";
    }
  }
*/
  psloop {
    set ps = processes('explorer.exe');
    foreach(p: $ps) {
    	echo "processes[$p] = '${ps[$p].name}'";
    }
  }
/*
  servicectl(reason: 'stop', service: 'Apache Archiva');

  set ss = services();	// optional service name
  echo "services returned '$ss'";
*/
}


action populate_combo3
{
  echo "option 1";
  echo "option 2";
  echo "option 3";
}
