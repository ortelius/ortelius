action preface
{
  echo 'The following packages will be deployed:';
  harvest_packageloop {
    echo "\t$TRIDM_PACKAGE";
  }
}


action run_anthill
{
  echo "Running Anthill for $app";
  trilogy(jobname: "RUN_ANTHILL", broker: "linux-gnh2", showoutput: true,	// custom3: "trilogy"
          params: { $row['VERSIONOBJID'], "app=${app}", "release=${release}",
                   "environmentType=${environmentType}", "site=${site}",
                   "dyces=${dyces}", "oes=${oes}", "searchNSend=${searchNSend}",
                   "wssg=${wssg}", "apollo=${apollo}", "jiraNumber=${jiraNumber}",
                   "product=${product}", "deployScriptVersion=${deployScriptVersion}",
                   "jobName=${jobName}" }) {
    post {
      if(${TRIDM_EXIT_CODE} != 0) {
        echo "AntHill Pro execution failed! Demoting package $TRIDM_PACKAGE...";
        echo "Exit code was ${TRIDM_EXIT_CODE}";
      } else {
	echo "Package $TRIDM_PACKAGE deployed successfully.";
	set -g state=$nextstate;
      }
    }
  }
}


action deploy_Apollo_SDP
{
  deploy_Apollo;
  deploy_SDP;
  parallel {
    deploy_NavApp_ShopApp_Portal;
    deploy_WSSG;
  }
}


action deploy_Legacy_Midtier
{
  deploy_Legacy;
  deploy_MidTier;
}


action deploy_NavApp_ShopApp_Portal
{
  deploy_NavApp;
  deploy_ShopApp;
  deploy_Portal;
}


action deploy_Apollo
{
  run_anthill(app: "Apollo");
}


action deploy_SDP
{
  run_anthill(app: "SDP");
}


action deploy_Legacy
{
  run_anthill(app: "Legacy");
}


action deploy_MidTier
{
  run_anthill(app: "MidTier");
}


action deploy_NavApp
{
  run_anthill(app: "NavApp");
}


action deploy_ShopApp
{
  run_anthill(app: "ShopApp");
}


action deploy_Portal
{
  run_anthill(app: "Portal");
}


action deploy_WSSG
{
  run_anthill(app: "WSSG");
}


action MacysDeployOld
{
  harvest_setpackages(repository: 'dm_demo', packages: {'DMDemo-2.0'});

  preface;

  parallel {
    deploy_Apollo_SDP;
    deploy_Legacy_Midtier;
  //}

  deploy_SDP;

  //parallel {
    deploy_NavApp_ShopApp_Portal;
    deploy_WSSG;
  }

  harvest_packageloop {
    echo "Promoting package $TRIDM_PACKAGE to ${nextstate}...";
    query(datasource: 'harvest', sql: "SELECT v.versionobjid FROM harversions v, harpackage p, harenvironment e WHERE e.environmentname='$TRIDM_PROJECT' AND p.envobjid=e.envobjid AND p.packagename='$TRIDM_PACKAGE' AND v.packageobjid=p.packageobjid AND v.itemtype=1 and v.versionobjid in(select max(x.versionobjid) from harversions x where x.packageobjid = p.packageobjid and x.itemobjid = v.itemobjid)", result: "row") {
      echo "Running Anthill on ${row[VERSIONOBJID]";
    }
  }
}

action MacysDeploy
{
  parallel {
    sequential { //deploy_Apollo_SDP
      echo "start deploy_Apollo_SDP";
      deploy_Apollo;
      deploy_SDP;
      parallel {
        sequential { // deploy_NavApp_ShopApp_Portal;
          echo "start deploy_NavApp_ShopApp_Portal 1";
          deploy_NavApp;
          deploy_ShopApp;
          deploy_Portal;
          echo "end deploy_NavApp_ShopApp_Portal 1";
        }
        deploy_WSSG;
      }
      echo "end deploy_Apollo_SDP";
    }
    sequential { //deploy_Legacy_Midtier
      echo "start deploy_Legacy_Midtier";
      deploy_Legacy;
      deploy_MidTier;
      echo "end deploy_Legacy_Midtier";
    }
  //}

  //parallel {
    sequential { // deploy_NavApp_ShopApp_Portal;
      echo "start deploy_NavApp_ShopApp_Portal 2";
      deploy_NavApp;
      deploy_ShopApp;
      deploy_Portal;
      echo "end deploy_NavApp_ShopApp_Portal 2";
    }
    deploy_WSSG;
  }
}
