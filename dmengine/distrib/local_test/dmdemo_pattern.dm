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
        checkout(dropzone: 'war', repository: 'dm_demo', state: $cmdln_harstate, viewpath: "target",
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
      checkout(dropzone: 'war', repository: 'dm_demo', state: "Release", viewpath: "target",
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
echo "TRIDM_DEP_FILELIST = '${TRIDM_DEP_FILELIST}'";
            //set -g sqllist = ${TRIDM_DEP_FILELIST};
echo "sqllist = '${dropzone.find('*.sql')}'";
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
    transfer(dropzone: 'sql') {
      post {
        echo "TRIDM_DEP_FILELIST = '${TRIDM_DEP_FILELIST}'";
        echo "dep.files = ${dep.files}";
        set filtered = ${dep.files.filter(@'\.sql$')};
        echo "dep.files.filtered = $filtered";
      }
    }

    // Run the script to apply the sql to the db
    // "/tmp/Deploy/fixed/applysql.sh"
    remotescript(filepath: append(${server.basedir}, 'applysql.sh'), shell: "/bin/sh",
                 params: { $deploy_action } + ${dep.files.filter(@'\.sql$')},
                 showoutput: true, custom1: "applysql") {
      post {
        echo "Exit code was ${TRIDM_EXIT_CODE}";
      }
    }
  }
}      
