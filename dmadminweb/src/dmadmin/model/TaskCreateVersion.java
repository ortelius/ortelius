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

import dmadmin.DMSession;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryField;
import dmadmin.json.BooleanField;
import dmadmin.json.IJSONSerializable;
import dmadmin.util.CommandLine;

public class TaskCreateVersion
	extends Task
{
	private static final long serialVersionUID = -4345762234192681139L;
	private Domain m_domain;
	private Application m_application;
	private Application m_predecessor;
	private boolean m_useLatest;
	private CommandLine m_cmd;
	
	public TaskCreateVersion() {
	}
	
	public TaskCreateVersion(DMSession sess, int id, String name) {
		super(sess, id, name);
	}
	
	public Domain getTargetDomain() { return m_domain; }
	public void setTargetDomain(Domain domain) { m_domain = domain; }
	
	public Application getApplication() { return m_application; }
	public void setApplication(Application application) { m_application = application; }
	
	public Application getPredecessor() { return m_predecessor; }
	public void setPredecessor(Application application) { m_predecessor = application; }
	
	public boolean getUseLatest() { return m_useLatest; }
	public void setUseLatest(boolean useLatest) { m_useLatest = useLatest; }
	
	@Override
	public boolean run()
	{
		System.out.println("Running TaskCreateVersion");

		Domain domain = getDomain();
		Engine engine = (domain != null) ? domain.findNearestEngine() : null;
		if(engine == null) {
			System.err.println("Engine was null");
			return false;
		}
		m_cmd = engine.doNewAppVer(this, m_application, m_useLatest, m_predecessor, m_aps);
		return (m_cmd.runWithTrilogy(true, null) == 0);
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
  ds.addProperty(SummaryField.TASK_CREATEVERSION_DOMAIN, "Create Version in Domain", ((getTargetDomain() != null) ? getTargetDomain().getFullDomain() : null));
  return ds.getJSON();
 }
 
	public String getOutput() {
		return m_cmd.getOutput();
	}
}
