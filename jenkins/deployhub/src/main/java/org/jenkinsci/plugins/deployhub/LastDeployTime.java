package org.jenkinsci.plugins.deployhub;

import org.kohsuke.stapler.DataBoundConstructor;

import hudson.Extension;
import hudson.model.AbstractItem;
import hudson.XmlFile;
import hudson.model.Hudson;
import hudson.views.ListViewColumnDescriptor;
import hudson.views.ListViewColumn;
import net.sf.json.JSONObject;
import java.io.*;

public class LastDeployTime extends ListViewColumn {

    private boolean displayName;
    private boolean trim;
    private int displayLength; //numbers of lines to display
    private int columnWidth;
    private boolean forceWidth;

    @DataBoundConstructor
    public LastDeployTime(boolean displayName, boolean trim, int displayLength, int columnWidth, boolean forceWidth) {
        super();
        this.displayName = displayName;
        this.trim = trim;
        this.displayLength = displayLength;
        this.columnWidth = columnWidth;
        this.forceWidth = forceWidth;
    }

    public LastDeployTime() {
        this(false, false, 1, 80, false);
    }
    
    public boolean isDisplayName() {
        return displayName;
    }

    public boolean isTrim() {
        return trim;
    }

    public int getDisplayLength() {
        return displayLength;
    }

    public int getColumnWidth() {
        return columnWidth;
    }
    
    public boolean isForceWidth() {
        return forceWidth;
    }

    public String getToolTip(AbstractItem job) {
        return formatDescription(job, false);
    }
    
    public String getDescription(AbstractItem job){
        return formatDescription(job, isTrim());
    }
    
    private String formatDescription(AbstractItem job, boolean trimIt) {
        if (job == null) {
            return null;
        }
	
	String rootDir=job.getRootDir().getAbsolutePath();
	XmlFile t = new XmlFile(Hudson.XSTREAM, new File(rootDir, "DeployHub.xml"));
	if (t != null && t.exists()) {
		try {
			JSONObject keyvals = (JSONObject)t.read();
			if (keyvals.containsKey("DeploymentID")) {
				return (String)keyvals.get("DeploymentID").toString();
			}
		} catch(IOException ex) {
		}
	} 
	return "N/A";
    }

    @Extension
    public static class DescriptorImpl extends ListViewColumnDescriptor {

        @Override
        public boolean shownByDefault() {
            return true;
        }

        @Override
        public String getDisplayName() {
	    return "Last Deployment";
        }

        @Override
        public String getHelpFile() {
            return "/plugins/DeployHub/help-description-column.html";
        }
    }
}
