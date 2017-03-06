function preCreateVersion()
{
  echo "Task name: '${task.name}'";
  echo "Task target: '${task.target.name}'";
  echo "Task predecessor: '${task.predecessor.name}'";
  echo "Task predecessor domain: '${task.predecessor.domain.name}'";
  
  foreach(dom: ${task.predecessor.approvals}) {
    echo "$dom => ${task.predecessor.approvals[$dom]}";
  }
	
  // Is the proposed predecessor already an unapproved version in the target
  // domain?  If so run the post-action directly to update the revision
  // attribute passing in the proposed predecessor and then abort to prevent
  // the create version task running
  if((${task.predecessor} != null)
      & (${task.predecessor.domain} = ${task.target})
      & (length(${task.predecessor.approvals}) = 0)) {
    postCreateVersion(newav: ${task.predecessor});
    return 1;
  }
  
  return 0;
}

action postCreateVersion
{
  // newav is either the parameter passed in or an attribute of the task
  if(!$newav) {
    abort(msg: "newav not set - unable to update attributes");
  }
  echo "newav is ${newav.id} '${newav.name}'";
  
  // This is just a dummy revision number that will increment over time
  set cmdline_revision = 123456 + ${newav.id};

  alter(object: $newav, attributes: {'revision' => $cmdline_revision});
  
  //admin(query: "UPDATE APPLICATION SET 'revision' = '$cmdline_revision' WHERE id = ${newav.id}");
}
