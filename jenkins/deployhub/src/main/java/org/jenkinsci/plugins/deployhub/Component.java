package org.jenkinsci.plugins.deployhub;

import hudson.Extension;
import java.util.List;
import java.util.HashMap;
import java.util.ArrayList;
import org.kohsuke.stapler.bind.JavaScriptMethod;

import hudson.model.Action;

@Extension
public class Component extends Common {
    String compname;
    String basename;

    public String getIconFileName() {
        return "/plugin/deployhub/images/application.png";
    }

    public Component(String compname) {
	int p = compname.lastIndexOf(".");
	this.compname = compname;
	this.basename = (p>=0)?compname.substring(p+1):compname;
    }

    public Component() {
    }

    public String getDisplayName() {
	return basename;
    }

@JavaScriptMethod
    public String[] loadComponent() {
	ArrayList<String> resps = new ArrayList<String>();
	HashMap<String,String> userAccounts = getUserAccounts();
	for(String url : userAccounts.keySet()) {
		String project = userAccounts.get(url);
		url = url.replace("XXX","component/"+compname+"?").replace(" ","%20");
		System.out.println(url);
		try {
			String res = httpGet(url);
			System.out.println("res="+res);
			resps.add(project);
			resps.add(res);
		} catch(Exception ex) {
			System.out.println("ex="+ex.getMessage());
		}
	}
	return resps.toArray(new String[resps.size()]);
    }
}
