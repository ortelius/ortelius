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

import java.util.List;

import dmadmin.ACDChangeSet;
import dmadmin.DMSession;
import dmadmin.ObjectType;
import dmadmin.PropertyDataSet;
import dmadmin.SummaryChangeSet;
import dmadmin.SummaryField;
import dmadmin.json.BooleanField;
import dmadmin.json.IJSONSerializable;
import dmadmin.json.LinkField;

public class Action
	extends DMObject
{
	private static final long serialVersionUID = 1338224772084535307L;
	
	private ActionKind m_kind = ActionKind.UNCONFIGURED;
	private Category m_category;
	private String m_fragname;
	private String m_filepath;
	private String m_interpreter;
	private Repository m_repository;
	private boolean m_function;
	private boolean m_resultIsExpr;
	private boolean m_copyToRemote;
	private int m_parentid;
	private boolean m_useTTY;
	
	public Action() {
	}
	
	public Action(DMSession sess, int id, String name) {
		super(sess, id, name);
	}
	
	public Action(DMSession sess, int id, String name, int domainid) {
		super(sess, id, name);
		setDomainId(domainid);
	}
	
	@Override
	public IJSONSerializable getLinkJSON() {
		System.out.println("in getLinkJSON for ACTION object");
		System.out.println("action kind is "+m_kind);
		Domain d = m_session.getDomain(getDomainId());
		String name = m_name;
		if (d!=null) {
			String fd = d.getFullDomain();
			if (fd.length()>0) name = fd+"."+m_name;
		}
		boolean showLink=m_session.ValidDomain(getDomainId());
		return new LinkField(getObjectType(), m_id, name , showLink, m_kind);
	}
	
	public ActionKind getKind()  { return m_kind; }
	public void setKind(ActionKind kind)  { m_kind = kind; }
	
	public int getParentId() { return m_parentid; }
	public void setParentId(int parentid) { m_parentid = parentid; }
	
	public String getKindAsString() {
		String fn = "";
		  
		 if (m_function)
		    fn = "Function";
		 else if (m_kind == ActionKind.GRAPHICAL)
		  fn = "Action";
		 else
		  fn = "Procedure";
		
		if(m_kind != null) {
			switch(m_kind) {
			default:
			case UNCONFIGURED: 		return "unconfigured";
			case SCRIPT:    		return "DMScript " + fn + " in repository";
			case IN_DB:     		return "DMScript in Database";
			case LOCAL_EXTERNAL:  	return "Local Script";
			case REMOTE_EXTERNAL:  	return "Endpoint Script";
			case PLUGIN:    		return fn + " provided by plugin";
			case GRAPHICAL: 		return "Graphical deployment flow";
			}
		}
		return "<UNKNOWN>";
	}
	
	public boolean isKindUnconfigured() {
		return (m_kind == ActionKind.UNCONFIGURED);
	}
	
	public boolean getShowDefinition() {
		return (m_kind == ActionKind.IN_DB);
	}
	
	public boolean getShowArgs() {
		return ((m_kind == ActionKind.IN_DB) && m_function)
				|| (m_kind == ActionKind.LOCAL_EXTERNAL)
				|| (m_kind == ActionKind.REMOTE_EXTERNAL);
	}
	
	public boolean getShowArgsOutput() {
		return (m_kind == ActionKind.LOCAL_EXTERNAL) || (m_kind == ActionKind.REMOTE_EXTERNAL);
	}
	
	@Override
	public String getActOrFuncText()  { return m_function ? "Function" : "Action"; }
	
	public Category getCategory()  { return m_category; }
	public void setCategory(Category cat)  { m_category = cat; }
	
	public String getFragmentName()  { return m_fragname; }
	public void setFragmentName(String fragname)  { m_fragname = fragname; }
	
	public String getFilepath()  { return m_filepath; }
	public void setFilepath(String filepath)  { m_filepath = filepath; }

	public String getInterpreter()  { return m_interpreter; }
	public void setInterpreter(String interpreter)  { m_interpreter = interpreter; }
 
	public Repository getRepository()  { return m_repository; }
	public void setRepository(Repository repository)  { m_repository = repository; }
	
	public boolean isFunction()  { return m_function; }
	public void setFunction(boolean fn)  { m_function = fn; }
	
	public boolean isResultIsExpr()  { return m_resultIsExpr; }
	public void setResultIsExpr(boolean rie)  { m_resultIsExpr = rie; }
	
	public boolean isUseTTY() { return m_useTTY; }
	public void setUseTTY(boolean usetty) { m_useTTY = usetty; }
	
	public boolean isRemote()  { return (m_kind == ActionKind.REMOTE_EXTERNAL); }
	
	public boolean isCopyToRemote()  { return m_copyToRemote; }
	public void setCopyToRemote(boolean copy)  { m_copyToRemote = copy; }
	
	public String getText() {
		return m_session.getActionText(this);
	}
	
	public String getTextWithBreaks() {
		String text = getText();
		System.out.println(text);
		return (text != null) ? text.replace("\n", "<br/>")
			.replace(" ", "&nbsp;")
			.replace("\t", "&nbsp;&nbsp;&nbsp;&nbsp;")
			: null;

	}
	
	public List<ActionArg> getInputArgs() {
		return m_session.getActionArgsForInput(this);
	}
	
	public List<ActionArg> getPaletteArgs() {
		return m_session.getActionArgsForPalette(this);
	}
	
	public List<ActionArg> getOutputArgs() {
		return m_session.getActionArgsForOutput(this);
	}
	
	@Override
	public ObjectType getObjectType() {
		System.out.println("getObjectType in");
		System.out.println("m_function="+m_function);
		System.out.println("m_kind="+m_kind);
		ObjectType ret;
		if (m_function)
		    ret = ObjectType.FUNCTION;
		else if (m_kind == ActionKind.GRAPHICAL)
			ret = ObjectType.ACTION;
		else
			ret = ObjectType.PROCEDURE;
		System.out.println("getObjectType ret="+ret);
		return ret;
	}

	@Override
	public String getDatabaseTable() {
		return "dm_action";
	}

	@Override
	public String getForeignKey() {
		return "actionid";
	}

	@Override
	public IJSONSerializable getSummaryJSON() {
		System.out.println("Getting summary data for action "+m_id);
		PropertyDataSet ds = new PropertyDataSet();
  Domain dom = getDomain();
  if (dom == null)
    ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", "");
  else
   ds.addProperty(SummaryField.DOMAIN_FULLNAME, "Full Domain", dom.getFullDomain());
		ds.addProperty(SummaryField.NAME, "Name", getName());
		ds.addProperty(SummaryField.SUMMARY, "Summary", getSummary());
		if (m_category != null) {
			System.out.println("Adding category "+m_category.getName()+" ("+m_category.getId()+")");
		} else {
			System.out.println("Adding category NO_CATEGORY");
		}
		
  ds.addProperty(SummaryField.ACTION_CATEGORY, "Category",
    ((m_category != null) ? m_category : Category.NO_CATEGORY).getLinkJSON());

  ds.addProperty(SummaryField.OWNER, "Owner", (m_owner != null) ? m_owner.getLinkJSON()
    : ((m_ownerGroup != null) ? m_ownerGroup.getLinkJSON() : null));
  
		addCreatorModifier(ds);
		
		ds.addProperty(SummaryField.ACTION_KIND, "Kind", new LinkField(ObjectType.ACTION_KIND,
			(m_kind != null) ? m_kind.value() : 0, getKindAsString()));

		ds.addProperty(SummaryField.ACTION_FRAGNAME, "Display Name", m_fragname);
							
		if (m_kind == ActionKind.SCRIPT || m_kind == ActionKind.UNCONFIGURED) {
			// DMScript in a repository (or nothing configured yet)
			ds.addProperty(SummaryField.ACTION_REPO, "Repository",
				((m_kind == ActionKind.SCRIPT) && (m_repository != null))
					? m_repository.getLinkJSON()
					: null);
		}
		
		ds.addProperty(SummaryField.ACTION_FILEPATH, "Filepath", m_filepath);
		ds.addProperty(SummaryField.ACTION_INTERPRETER, "Command Line Interpreter", m_interpreter);
		ds.addProperty(SummaryField.ACTION_USETTY, "Allocate Terminal", new BooleanField(m_useTTY));
		
		if(m_function && ((m_kind == ActionKind.LOCAL_EXTERNAL) || (m_kind == ActionKind.REMOTE_EXTERNAL))) {
			ds.addProperty(SummaryField.ACTION_RESISEXPR, "Result Is Expr", new BooleanField(m_resultIsExpr));
		}
		
		ds.addProperty(SummaryField.ACTION_COPYTOREM, "Copy to Remote", new BooleanField(m_copyToRemote));
		return ds.getJSON();
	}

	@Override
	public boolean updateSummary(SummaryChangeSet changes) {
		return m_session.updateAction(this, changes);
	}

	public boolean updateArgs(ACDChangeSet<Action.ActionArg> changes) {
		return m_session.updateActionArgs(this, changes);
	}
	
	@Override
	public String getWriteTitle() {
		return "Execute Access";
	}
	
	public enum SwitchMode {
		MODE_NONE(null),
		MODE_SWITCH("S"),
		MODE_PREFIX("P"),
		MODE_ALWAYS("A"),
		MODE_BOOLEAN("B");
		
		private String m_value;
		
		private SwitchMode(String value) {
			m_value = value;
		}
		
		public String value()  { return m_value; }
		
		public static SwitchMode fromString(String str) {
			for(SwitchMode mode : values()) {
				if((mode.m_value != null) && mode.m_value.equals(str)) {
					return mode;
				}
			}
			return MODE_NONE;
		}
	}
	
	public class ActionArg
	{
		private String m_id;			// This is actually the original name, but it serves as an id
		private String m_name;
		private String m_type;
		private int m_inpos;
		private int m_outpos;
		private String m_switch;
		private String m_negswitch;
		private SwitchMode m_mode;
		private boolean m_required;
		private boolean m_pad;

		public ActionArg(String id, String name, String type, boolean reqd) {
			m_id = id;
			m_name = name;
			m_required = reqd;
			m_type = type;
		}

		public ActionArg(
				String id, String name, String type, boolean reqd, boolean pad, int inpos,
				int outpos, SwitchMode mode, String switchstr, String negswitch)
		{
			this(id, name, type, reqd);
			m_pad = pad;
			m_inpos = inpos;
			m_outpos = outpos;
			m_mode = mode;
			m_switch = switchstr;
			m_negswitch = negswitch;
		}
		
		public String getId()  { return m_id; }
		public String getName()  { return m_name; }
		public String getType()  { return m_type; }
		public boolean isRequired()  { return m_required; }
		public boolean isPad()  { return m_pad; }
		public int getInputPosition()  { return m_inpos; }
		public int getOutputPosition()  { return m_outpos; }
		public SwitchMode getSwitchMode()  { return m_mode; }
		public String getSwitch()  { return m_switch; }
		public String getNegSwitch()  { return m_negswitch; }
		
		public boolean isAlways()  { return (m_mode == SwitchMode.MODE_ALWAYS); }
		
		public String getOutputBorderClass()  {
			if(m_required || isAlways()) {
				return "actionarg_required";
			} else if(m_pad) {
				return "actionarg_padded";
			} else {
				return "actionarg_optional";
			}
		}
		
		public String getPaletteBorderClass()  {
			return m_required ? "actionarg_required" : "actionarg_optional";
		}
		
		public String getTextWithMarkup()
		{
			StringBuffer ret = new StringBuffer();
			
			String nameMarkup = "<span class=\"argName\">" + m_name + "</span>";
			
			switch(m_mode) {
			default:
			case MODE_NONE:
				ret.append(nameMarkup);
				break;
			case MODE_SWITCH:
				if(m_switch != null) ret.append("<b>" + m_switch + "</b>&nbsp;");
				ret.append(nameMarkup);
				if(m_negswitch != null) ret.append("&nbsp;/&nbsp;<b>" + m_negswitch + "</b>");
				break;
			case MODE_PREFIX:
				if(m_switch != null) ret.append("<b>" + m_switch + "</b>");
				ret.append(nameMarkup);
				if(m_negswitch != null) ret.append("&nbsp;/&nbsp;<b>" + m_negswitch + "</b>");
				break;
			case MODE_ALWAYS:
				ret.append("<b>" + m_switch + "</b>");
				break;
			case MODE_BOOLEAN:
				ret.append("<span class=\"tooltip-follows\">");
				if(m_switch != null) ret.append("<b>" + m_switch + "</b>");
				ret.append("&nbsp;/&nbsp;");
				if(m_negswitch != null) ret.append("<b>" + m_negswitch + "</b>");
				ret.append("</span><div class=\"tooltip\"><div class=\"tooltip-text\">"
						+ "<span class=\"argName\">" + m_name + "</span>"
						+ "</div><div class=\"tooltip-arrow\">&#9660;</div></div>");
				break;
			}
			
			return ret.toString();
		}
	}
}
