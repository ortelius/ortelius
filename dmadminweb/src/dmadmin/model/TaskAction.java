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
import dmadmin.util.CommandLine;

public class TaskAction
	extends Task
{
	private static final long serialVersionUID = 1327862378913381548L;

	private String m_text;
	CommandLine m_cmd;
	private Action m_action;

	public TaskAction() {
	}

	public TaskAction(DMSession sess, int id, String name) {
		super(sess, id, name);
	}

	@Override
	public boolean run() {
		System.out.println("Running TaskAction");

		Domain domain = getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		if(engine == null) {
			System.err.println("Engine was null");
			return false;
		}
		m_cmd = engine.doRunAction(this,m_aps);
		return (m_cmd.runWithTrilogy(true, m_text + "\n") == 0);
	}

	public String getOutput() {
		return m_cmd.getOutput();
	}

	public Action getAction() {
		System.out.println("getAction returns "+m_action);
		return m_action;
	}

	public void setAction(Action action) {
		m_action = action;
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
  ds.addProperty(SummaryField.TASK_ACTION, "Action to Run", ((getAction() != null) ? getAction().getLinkJSON() : null));
  ds.addProperty(SummaryField.TASK_SHOWOUTPUT, "Show Output", new BooleanField(getShowOutput()));
  ds.addProperty(SummaryField.TASK_AVAILABLE_TO_SUBDOMAINS, "Available in SubDomains", new BooleanField(getSubDomains()));
  ds.addProperty(SummaryField.SUCCESS_TEMPLATE, "Success Notification Template", ((getSuccessTemplate() != null) ? getSuccessTemplate().getLinkJSON() : null));
  ds.addProperty(SummaryField.FAILURE_TEMPLATE, "Failure Notification Template", ((getFailureTemplate() != null) ? getFailureTemplate().getLinkJSON() : null));
  return ds.getJSON();
 }
	public boolean updateTask(Hashtable<String, String> changes) {
		// TODO Auto-generated method stub
		return false;
	}
}
