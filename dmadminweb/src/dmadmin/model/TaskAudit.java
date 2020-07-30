/*
 *
 *  Ortelius for Microservice Configuration Mapping
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
