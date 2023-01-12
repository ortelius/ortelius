action dmdemo_binrepo_custom
{
  avloop {
    comploop {
      echo "\t${application.name} - ${component.name} (${component.rollup}, ${component.rollback})";
      deploy(showoutput: true) {
        post {
          echo flatten(${dep.files});
        }
      }
    }
  }
}
