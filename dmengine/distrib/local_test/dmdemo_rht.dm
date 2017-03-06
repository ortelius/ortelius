//<?xml version="1.0" encoding="ISO-8859-1"?>
//<dm>
//  <globals>
//    <set name="agentport" value="9501" />
//  </globals>
//  
//  <servers>
//    <physical>
//      <server name="webapp" hostname="harvest-5a013dc" type="windows" protocol="agent" port="${agentport}" custom2="Tomcat">
//        <basedir>C:\Program Files\Apache Software Foundation\Tomcat 5.5\webapps</basedir>
//        <set name="web" value="1" />
//      </server>
//      
//      <server name="database" hostname="harvest-5a013dc" type="windows" protocol="agent" port="${agentport}" custom2="Oracle">
//        <basedir>C:\Temp\Deploy\server2</basedir>
//        <set name="sql" value="1" />
//      </server>
//    </physical>
//    
//    <logical>
//      <server name="envA" custom3="envA">
//        <physical>webapp</physical>
//        <physical>database</physical>
//        <state>Development</state>
//        <state>Test</state>
//        <state>Release</state>
//        <!-- usergroup>admin</usergroup -->
//        <password filepath="${TRILOGYHOME}/DM/dmdemo.dfo" />
//      </server>
//      
//      <server name="envB">
//        <physical>webapp</physical>
//        <state>Development</state>
//        <state>Test</state>
//        <!-- usergroup>admin</usergroup -->
//      </server>
//      
//      <server name="envC">
//        <physical>database</physical>
//        <state>Development</state>
//        <state>Test</state>
//        <!-- usergroup>admin</usergroup -->
//      </server>
//    </logical>
//  </servers>
//  
//  <applications>
//    <application name="DMDemo" PackageFilter="Y">
//      <!-- usergroup>admin</usergroup -->
//      <server>envA</server>
//      <server>envB</server>
//      <server>envC</server>
//      <set name="deploy_action" value="APPLY" />
//      <5>deploy</action>
//    </application>
//
//    <application name="DMDemo (Rollback)" PackageFilter="Y">
//      <!-- usergroup>admin</usergroup -->
//      <server>envA</server>
//      <server>envB</server>
//      <server>envC</server>
//      <set name="deploy_action" value="ROLLBACK" />
//      <action>deploy</action>
//    </application>
//  </applications>
//  
//  <actions>


action Preface
{
  echo "The following packages will be deployed:";
  harvest_packageloop {
    echo ${package.name};
    echo "package '${package.name}' has creator '${package.creator.name}' and assignee '${package.assignee.name}'";
    echo "package '${package.name}' has creator email '${package.creator.email}' and assignee email '${package.assignee.email}'";
    foreach(contributor: ${package.contributors}) {
    	echo "  contributor ${package.contributors[$contributor].name}";
    }	
  }
  
  echo "\nTo the following servers:";
  psloop {
    echo "${server.name} (${server.hostname}) web=\"${web:-0}\" sql=\"${sql:-0}\" basedir=\"${server.basedir}\"";
  }
}


function Percent(obj)
{
  if(${obj.dep.total} = 0) {
  	return 0;
  }
  return ${obj.dep.success} * 100 / ${obj.dep.total};
}


action Percentages
{
  echo "\n===================================";
  echo "Total deployments ${global.dep.success} / ${global.dep.total} ($(Percent($global))%)";

  psloop {
    echo "Deployments to $TRIDM_PHYSICAL_SERVER ${server.dep.success} / ${server.dep.total} ($(Percent($server))%)";
  }
  harvest_packageloop {
    echo "Deployments of ${package.name} ${package.dep.success} / ${package.dep.total} ($(Percent($package))%)";
  }

  echo "===================================";
}


action UpdateWebConfig
{
  using dropzone $dropzone { 
    jar(options: 'xf', jarfile: ${file}, files: 'WEB-INF/web.xml');
    
    modify(modifier: 'xml', file: 'WEB-INF/web.xml') {
      add_element(xpath: '/web-app', pos: 'inside',
                  value: "<context-param>"
                         "<param-name>deploymentTime</param-name>"
                         "<param-value>$(now())</param-value>"
                         "<description>time the application was deployed</description>"
                         "</context-param>");
    }
    
    jar(options: 'uf', jarfile: ${file}, files: 'WEB-INF/web.xml');
    
    delete(dir: 'WEB-INF');
  }
}


action Deploy
{
  harvest_setpackages(packages: $ARGV /*$cmdln_harpackages*/, project: $cmdln_harproject);
  
  Preface;
                          
  // For web servers
  set -g webflag = false;
  if($web = 1)
  {
    echo "\n-------------------------------------------------------------";
    echo "Checking out [*.war] to web servers in environment $TRIFIELD1";
    echo "-------------------------------------------------------------\n";
    
    // Are we rolling back?
    if(${deploy_action} != 'ROLLBACK')
    {
      // No, so deploy new war file
      harvest_packageloop {
      	// viewpath appends to give "\dmdemo\target"
        /*deploy*/ checkout(dropzone: 'war', repository: 'dm_demo', state: $cmdln_harstate, viewpath: "target",
               package: ${package.name}, pattern: "*.war",
               options: "pc", showfeedback: "none", custom1: "war1") {
          pre {
            echo "Deploying [${deploy.pattern}] onto remote server ${server.name} (${server.hostname})...";
          }
          post {
            set -g webflag = ($TRIDM_DEP_SUCCESS > 0);
            echo "Deployed ${TRIDM_DEP_SUCCESS:-0} / ${TRIDM_DEP_TOTAL:-0} [${deploy.pattern}] onto remote server ${server.name} (${server.hostname})...";
          }
        }
      }
    }
    else
    {
      // Yes, so deploy old war file
      // viewpath appends to give "\dmdemo\target"
      /*deploy*/ checkout(dropzone: 'war', repository: 'dm_demo', state: "Release", viewpath: "target",
             pattern: "*.war", options: "pc", showfeedback: "none",
             IgnorePackageFilter: "Y", custom1: "war2") {
        pre {
          echo "Deploying [${deploy.pattern}] onto remote server ${server.name} (${server.hostname})...";
        }
        post {
          set -g webflag = ($TRIDM_DEP_SUCCESS > 0);
          echo "Deployed ${TRIDM_DEP_SUCCESS:-0} / ${TRIDM_DEP_TOTAL:-0} [${deploy.pattern}] onto remote server ${server.name} (${server.hostname})...";
        }
      }
    }
  
    UpdateWebConfig(dropzone: 'war', file: 'dmdemo.war');
  
    transfer(dropzone: 'war');
  }
  
  // For database servers
  set -g sqlflag = false;
  if($sql = 1)
  {
    echo "\n-------------------------------------------------------------";
    echo "Checking out [*.sql] to database servers in environment $TRIFIELD1";
    echo "-------------------------------------------------------------\n";

    harvest_packageloop {
      // viewpath appends to give "\dmdemo\sql"
      checkout(dropzone: 'sql', repository: 'dm_demo', state: $cmdln_harstate, viewpath: "sql",
             target: "sql", package: ${package.name}, pattern: "*.sql",
             options: "pc", showfeedback: "pre", custom1: "sql") {
        post {
          if(${TRIDM_DEP_SUCCESS} > 0) {
            set -g sqlflag = true;
            set -g sqllist = ${TRIDM_DEP_FILELIST};
          }
          echo "Deployed ${TRIDM_DEP_SUCCESS:-0} / ${TRIDM_DEP_TOTAL:-0} [${deploy.pattern}] onto remote server ${server.name} (${server.hostname})...";
        }
      }
    }
  }
  
  Percentages;
  
  // Were there any failures?
  if(${global.dep.success} -ne ${global.dep.total}) {
    abort(msg: "Failed to deploy!");
  }
  // Did we deploy anything?
  if(${global.dep.total} -eq 0) {
    abort(msg: "Nothing was deployed!");
  }
    
  // Did we deploy the web component?
  if(($webflag = true) -a ($web = 1)) {
    // Install the war file
    remotescript(filepath: "touch", //shell: "",
                 params: { "${server.basedir}/dmdemo.war" },
                 absolute: true, showoutput: true, custom1: "touch") {
      post {
        echo "Exit code was ${TRIDM_EXIT_CODE}";
      }
    }
  }
  
  // Were there any database changes?
  if(($sqlflag = true) -a ($sql = 1)) {
    set -g scriptflag = false;
    
    // Deploy the sql apply script
    // viewpath appends to give "\dmdemo\scripts"
    // TODO: state should be passed on command line
    checkout(dropzone: 'sql', repository: 'dm_demo', state: 'Development', viewpath: "scripts",
           pattern: "applysql.sh",
           IgnorePackageFilter: true, options: "pc", showfeedback: "pre", custom1: "applysql") {
      post {
        set -g scriptflag = ($TRIDM_DEP_SUCCESS > 0);
        echo "Deployed ${TRIDM_DEP_SUCCESS:-0} / ${TRIDM_DEP_TOTAL:-0} [${deploy.pattern}] onto remote server ${server.name} (${server.hostname})...";
      }
    }
    
    // Did the script checkout successfully?
    if($scriptflag != true) {
      abort(msg: "Failed to checkout applysql script");
    }
    
    // Transfer the database sql changes and apply script to the server
    transfer(dropzone: 'sql');
    
    // Temp fix
    psloop {
      if(${deploy_action} != 'ROLLBACK') {
        set sqllist = "${server.basedir}/sql/add_dob.sql";
      } else {
        set sqllist = "${server.basedir}/sql/rollback_dob.sql";
      }
    }

    // Run the script to apply the sql to the db
    // ${server.basedir}
    remotescript(filepath: "/tmp/Deploy/fixed/applysql.sh", shell: "/bin/sh",
                 params: { $deploy_action, $sqllist },
                 showoutput: true, custom1: "applysql") {
      post {
        echo "Exit code was ${TRIDM_EXIT_CODE}";
      }
    }
  }
}      


action populate_listbox
{
  //echo "PACKAGENAME,ASIGNEE,CREATED,MODIFIED";
  
  // dumy data
  set DEQUOTE = true;
  foreach(pkg: $cmdln_harpackages) {
    echo "$pkg,demo,18/09/2012 15:43,18/09/2012 15:43";
  }
  
  //set str = "SELECT b.packageobjid, b.packagename, a.username, b.creationtime, b.modifiedtime";
  //set -a str = " FROM harallusers a, harpackage b, harenvironment c, harstate d";
  //set -a str = " WHERE d.statename='$cmdln_state' AND c.environmentname='DMDemo'";
  //set -a str = " AND d.envobjid=c.envobjid AND b.stateobjid=d.stateobjid";
  //set -a str = " AND a.usrobjid=b.assigneeid AND b.packagename in ('DMDemo-1.0','DMDemo-2.0')";
  //
  //query(datasource: 'harvest', sql: $str, result: 'row') {
  //  echo "${row['packagename']},${row['username']},${row['creationtime']},${row['modifiedtime']}";
  //}
}


//  </actions>
//  
//  <trilogy>
//    <field number="1" content="servers" />
//    <field number="2" content="username" />
//    <field number="3" content="password" />
//    <field number="4" content="applications" />
//  </trilogy>
//</dm>
