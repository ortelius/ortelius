/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package dmadmin.model;

import java.util.Hashtable;
//import java.util.Map;

import dmadmin.DMSession;
import dmadmin.util.CommandLine;

public class TaskAudit extends Task
{
 private static final long serialVersionUID = 1327862378913381548L;
 private int m_srvid;
 private CommandLine m_cmd;
 private Server server;
 private String home;

 public TaskAudit()
 {
 }

 public TaskAudit(DMSession sess, String home, int serverid)
 {
  super(sess,serverid,"");
  server = sess.getServer(serverid, false);
  m_srvid = serverid;
  this.home = home;
  System.out.println("In constructor for TaskAudit");
 }

 @Override
 public boolean run()
 {
  Domain domain = server.getDomain();
  Engine engine = (domain != null) ? domain.findNearestEngine() : null;
  if (engine == null)
  {
   System.err.println("Engine was null");
   return false;
  }
  m_cmd = engine.auditServer(home, m_srvid);

  return (m_cmd.run(true) == 0);
 }

 public String getOutput()
 {
  return m_cmd.getOutput();
 }

 public boolean updateTask(Hashtable<String, String> changes)
 {
  // TODO Auto-generated method stub
  return false;
 }
}
