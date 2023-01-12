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
  using dropzone $dz {
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
  // , project: $cmdln_harproject
  harvest_setpackages(repository: 'dm_demo', packages: $ARGV /*$cmdln_harpackages*/);

  Preface;

//parallel {
  // For web servers
  set -g webflag = false;
  using component "dmdemoweb_${deploy_action}" {
    using dropzone 'war' {
      echo "\n-------------------------------------------------------------";
      echo "Checking out [*.war] to web servers in environment $TRIFIELD1";
      echo "-------------------------------------------------------------\n";

      // Are we rolling back?
      if(${deploy_action} != 'ROLLBACK')
      {
        // No, so deploy new war file
        harvest_packageloop {
        	// viewpath appends to give "\dmdemo\target"
          checkout(options: "pc", showfeedback: "none", custom1: "war1") {
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
        checkout(options: "pc", showfeedback: "none",
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

      UpdateWebConfig(dz: ${dropzone.name}, file: 'dmdemo.war');

      transfer;
    }
  }

  // For database servers
  echo "\n-------------------------------------------------------------";
  echo "Checking out [*.sql] to database servers in environment $TRIFIELD1";
  echo "-------------------------------------------------------------\n";

  set -g sqlflag = false;
  harvest_packageloop {
    deploy(component: 'dmdemosql', options: "pc", showfeedback: "pre", custom1: "sql") {
      post {
        // was TRIDM_DEP_SUCCESS - but this lands in repository scope
        if(${deploy.dep.success} > 0) {
          set -g sqlflag = (${dropzone.find('*.sql').length()} > 0);
          set -g scriptflag = (${dropzone.find('applysql.sh').length()} > 0);
echo "TRIDM_DEP_FILELIST = '${TRIDM_DEP_FILELIST}'";
echo "sqllist = '${dropzone.find('*.sql')}'";
        }
        echo "Deployed ${TRIDM_DEP_SUCCESS:-0} / ${TRIDM_DEP_TOTAL:-0} [${deploy.pattern}] onto remote server ${server.name} (${server.hostname})...";
      }
    }
  }
//} // end parallel

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
  if($webflag = true) {
    using component 'dmdemoweb' {
      // Install the war file
      remotescript(filepath: "touch", //shell: "",
                   params: { append(${server.basedir}, 'dmdemo.war') },
                   absolute: true, showoutput: true, custom1: "touch") {
        post {
          echo "Exit code was $?";
        }
      }
    }
  }

  // Were there any database changes?
  if($sqlflag = true) {
    if($scriptflag != true) {
      abort(msg: "Failed to checkout applysql script");
    }

    using component 'dmdemosql' {
      // Run the script to apply the sql to the db
      // "/tmp/Deploy/fixed/applysql.sh"
      remotescript(filepath: append(${server.basedir}, 'applysql.sh'), shell: "/bin/sh",
                   params: { $deploy_action } + ${dep.files.filter(@'\.sql$')},
                   showoutput: true, custom1: "applysql") {
        post {
          echo "Exit code was $?";
        }
      }
    }
  }
}
