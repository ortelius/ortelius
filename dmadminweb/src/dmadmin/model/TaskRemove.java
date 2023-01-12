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

import dmadmin.DMSession;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryField;
import dmadmin.json.BooleanField;
import dmadmin.json.IJSONSerializable;
//import dmadmin.PropertyDataSet;
import dmadmin.util.CommandLine;

public class TaskRemove
	extends Task
{
	private static final long serialVersionUID = 1327862378913381548L;
	private Environment m_environment;
	private Application m_application;
	private String m_text;
	private CommandLine m_cmd;

	public TaskRemove() {
	}

	public TaskRemove(DMSession sess, int id, String name) {
		super(sess, id, name);
	}

	public boolean updateTask(Hashtable<String, String> changes) {
		// TODO Auto-generated method stub
		return false;
	}

	public Environment getEnvironment() { return m_environment; }
	public void setEnvironment(Environment env) { m_environment = env; }

	public Application getApplication() { return m_application; }
	public void setApplication(Application application) { m_application = application; }

	public String getText()  { return m_text; }
	public void setText(String text)  { m_text = text; }

	@Override
	public boolean run()
	{
		System.out.println("Running TaskRemove");

		Domain domain = getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		if(engine == null) {
			System.err.println("Engine was null");
			return false;
		}
		m_cmd = engine.doRemove(this, m_application,m_environment,m_aps);
		return (m_cmd.runWithTrilogy(true, m_text + "\n") == 0);
	}

 @Override
 public IJSONSerializable getSummaryJSON() {
  PropertyDataSet ds = new PropertyDataSet();
  Domain dom = getDomain();
  if (dom == null)
    ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", "");
  else
   ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", dom.getFullDomain());
  ds.addProperty(SummaryField.NAME, "Name", getName());
  addCreatorModifier(ds);
  ds.addProperty(SummaryField.PRE_ACTION, "Pre-action", ((getPreAction() != null) ? getPreAction().getLinkJSON() : null));
  ds.addProperty(SummaryField.POST_ACTION, "Post-action", ((getPostAction() != null) ? getPostAction().getLinkJSON() : null));
  ds.addProperty(SummaryField.TASK_SHOWOUTPUT, "Show Output", new BooleanField(getShowOutput()));
  ds.addProperty(SummaryField.TASK_AVAILABLE_TO_SUBDOMAINS, "Available in SubDomains", new BooleanField(getSubDomains()));
  return ds.getJSON();
 }

	public String getOutput() {
		return m_cmd.getOutput();
	}
}
