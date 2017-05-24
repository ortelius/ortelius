package org.jenkinsci.plugins.deployhub;

import static org.apache.commons.io.IOUtils.copy;
import hudson.ExtensionList;
import hudson.ExtensionPoint;
import hudson.model.Action;
import hudson.model.Describable;

import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import jenkins.model.Jenkins;

import org.kohsuke.stapler.StaplerRequest;
import org.kohsuke.stapler.StaplerResponse;

public abstract class DeployHubMenu implements ExtensionPoint, Action, Describable<DeployHubMenu> {
    public String getIconFileName() {
        return "gear.png";
    }

    public String getUrlName() {
        return getClass().getSimpleName();
    }

    public String getDisplayName() {
        return getClass().getSimpleName();
    }

    public abstract String getDescription();

    public DeployHubMenuDescriptor getDescriptor() {
        return (DeployHubMenuDescriptor) Jenkins.getInstance().getDescriptorOrDie(getClass());
    }

    public static ExtensionList<DeployHubMenu> all() {
        return Jenkins.getInstance().getExtensionList(DeployHubMenu.class);
    }
}
