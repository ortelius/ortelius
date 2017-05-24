package org.jenkinsci.plugins.deployhub;

import hudson.model.Hudson;
import hudson.XmlFile;
import hudson.Extension;
import hudson.model.RootAction;
import hudson.model.Action;
import jenkins.model.ModelObjectWithContextMenu;
import org.kohsuke.stapler.StaplerRequest;
import org.kohsuke.stapler.StaplerResponse;
import jenkins.model.Jenkins.DescriptorImpl;
import java.io.*;

import java.util.List;

/**
 * Entry point to all the UI samples.
 * 
 * @author Kohsuke Kawaguchi
 */
@Extension
public class Root implements RootAction, ModelObjectWithContextMenu {
    public String getIconFileName() {
        return "/plugin/deployhub/images/deployhub.png";
    }

    public String getDisplayName() {
        return "DeployHub";
    }

    public String getUrlName() {
        return "deployhub";
    }

    public List<DeployHubMenu> getAll() {
        return null;
    }

    public String getServerURL() {
	return Common.getServerURL();
    }

    public String getViewUrl() {
	return "deployhub";
    }

    public Action getDynamic(String name) {
	if (name.equalsIgnoreCase("environments")) return new Environments();
	if (name.equalsIgnoreCase("servers")) return new Servers();
	if (name.equalsIgnoreCase("applications")) return new Applications();
	if (name.equalsIgnoreCase("components")) return new Components();
	return null;
    }

    public ContextMenu doContextMenu(StaplerRequest request, StaplerResponse response) throws Exception {
        return new ContextMenu().addAll(getAll());
    }
}
