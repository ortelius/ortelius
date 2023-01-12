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

public class TaskMove
	extends Task
{
	private static final long serialVersionUID = 1327862378913381548L;
	private Domain m_domain;
	private Application m_application;
	String m_text;
	CommandLine m_cmd;
	String noengine_output = null;


	public TaskMove() {
	}

	public TaskMove(DMSession sess, int id, String name) {
		super(sess, id, name);
	}

	public boolean updateTask(Hashtable<String, String> changes) {
		// TODO Auto-generated method stub
		return false;
	}

	public Domain getTargetDomain() { return m_domain; }
	public void setTargetDomain(Domain domain) { m_domain = domain; }

	public Application getApplication() { return m_application; }
	public void setApplication(Application application) { m_application = application; }

	public String getText()  { return m_text; }
	public void setText(String text)  { m_text = text; }


	@Override
	public boolean run()
	{
		System.out.println("Running TaskMove");

  if (getPreAction() == null && getPostAction() == null && getSuccessTemplate() == null && getFailureTemplate() == null)
  {
   String approved = m_session.getApprovalStatus(getApplication().getId());

   if (approved == null)
   {
    boolean doMove = true;
    boolean approvalNeeded = m_session.getApprovalNeeded(getTargetDomain().getId());

    if (approvalNeeded)
     doMove = false;

    if (doMove)
    {
     if (m_session.MoveApplication(getApplication(),getTargetDomain(),m_text).length() == 0)
     {
      noengine_output = "Application moved to " + getTargetDomain().getName();
      return true;
     }
    }
   }
   else if (approved.equals("Y"))
   {
    if (m_session.MoveApplication(getApplication(),getTargetDomain(), m_text).length() == 0)
    {
     noengine_output = "Application moved to " + getTargetDomain().getName();
     return true;
    }
   }
   noengine_output = "Application has not been approved for " + getTargetDomain().getName();
   return false;
  }

		Domain domain = getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		if(engine == null) {
			System.err.println("Engine was null");
			return false;
		}
		m_cmd = engine.doMoveCopyRequest(this, m_application, m_aps);
		return (m_cmd.runWithTrilogy(true, m_text + "\n") == 0);
	}

 @Override
 public IJSONSerializable getSummaryJSON() {
	 NotifyTemplate x = getSuccessTemplate();
	 System.out.println("x="+x);
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
  ds.addProperty(SummaryField.TASK_MOVE_TO_DOMAIN, "Move to Domain", ((getTargetDomain() != null) ? getTargetDomain().getFullDomain() : null));
  ds.addProperty(SummaryField.SUCCESS_TEMPLATE, "Success Notification Template", ((getSuccessTemplate() != null) ? getSuccessTemplate().getLinkJSON() : null));
  ds.addProperty(SummaryField.FAILURE_TEMPLATE, "Failure Notification Template", ((getFailureTemplate() != null) ? getFailureTemplate().getLinkJSON() : null));
  return ds.getJSON();
 }
	public String getOutput() {
	 if (noengine_output != null)
	  return noengine_output;
	 else
		 return m_cmd.getOutput();
	}
}
