package org.jenkinsci.plugins.deployhub;

import hudson.model.Descriptor;

public abstract class DeployHubMenuDescriptor extends Descriptor<DeployHubMenu> {
    @Override
    public String getDisplayName() {
        return clazz.getSimpleName();
    }
}
