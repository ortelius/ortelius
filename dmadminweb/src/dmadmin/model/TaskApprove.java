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

//import java.util.HashMap;
import java.util.Hashtable;
//import java.util.Map;

import dmadmin.DMSession;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryField;
import dmadmin.json.BooleanField;
import dmadmin.json.IJSONSerializable;
import dmadmin.util.CommandLine;
//import dmadmin.PropertyDataSet;

public class TaskApprove
	extends Task
{
	private static final long serialVersionUID = 1327862378913381548L;
	
	private Application m_app;
	private boolean m_approved;
	private String m_text;
	private Domain m_approvalDomain;
	private CommandLine m_cmd;
 String noengine_output = null;
	
	public TaskApprove() {
	}
	
	public TaskApprove(DMSession sess, int id, String name) {
		super(sess, id, name);
		System.out.println("In constructor for TaskApprove");
	}
	
	public Application getApplication()  { return m_app; }
	public void setApplication(Application app)  { m_app = app; }
	
	public boolean isApproved()  { return m_approved; }
	public void setApproved(boolean approved)  { m_approved = approved; }

	public String getText()  { return m_text; }
	public void setText(String text)  { m_text = text; }
	
	public Domain getApprovalDomain() { return m_approvalDomain; }
	public void setApprovalDomain(Domain domain) { m_approvalDomain = domain; }
	
	
	@Override
	public boolean run() {
		if(m_app == null) {
			System.err.println("Application not set");
			return false;
		}
		//return m_app.approve(m_approvalDomain, m_approved, m_text);
  
  if (getPreAction() == null && getPostAction() == null && getSuccessTemplate() == null && getFailureTemplate() == null)
  {
   boolean res = m_session.approveApplication(getApplication(), getApprovalDomain(), m_approved, m_text);
   
   if (res)
   {
    if (m_approved)
     noengine_output = "Application has been approved for " + getApprovalDomain().getName();
    else
     noengine_output = "Application has been rejected for " + getApprovalDomain().getName();
   } 
   else
    noengine_output = "Application could not be Approved/Rejected for " + getApprovalDomain().getName();
   
   return res;
  }  
  
		Domain domain = getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		if(engine == null) {
			System.err.println("Engine was null");
			return false;
		}
	
		m_cmd = engine.doApprove(this, m_app, m_approved, m_aps);
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
  ds.addProperty(SummaryField.TASK_APPROVAL_DOMAIN, "Approval Domain", ((getApprovalDomain() != null) ? getApprovalDomain().getFullDomain() : null));
  ds.addProperty(SummaryField.SUCCESS_TEMPLATE, "Approved Notify Template", ((getSuccessTemplate() != null) ? getSuccessTemplate().getLinkJSON() : null));
  ds.addProperty(SummaryField.FAILURE_TEMPLATE, "Rejected Notify Template", ((getFailureTemplate() != null) ? getFailureTemplate().getLinkJSON() : null));
  return ds.getJSON();
 }
 
	public String getOutput() {
  if (noengine_output != null)
   return noengine_output;
  else
		 return m_cmd.getOutput();
	}
	
	public boolean updateTask(Hashtable<String, String> changes) {
		// TODO Auto-generated method stub
		return false;
	}
}
