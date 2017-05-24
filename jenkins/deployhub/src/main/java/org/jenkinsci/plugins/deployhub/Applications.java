package org.jenkinsci.plugins.deployhub;

import hudson.Extension;
import java.util.List;
import java.util.HashMap;
import java.util.ArrayList;
import org.kohsuke.stapler.bind.JavaScriptMethod;

import hudson.model.Action;

@Extension
public class Applications extends Common {
    public String getIconFileName() {
        return "/plugin/deployhub/images/application.png";
    }

    public Action getDynamic(String name) {
	System.out.println("Applications - getDynamic("+name+")");
	return new Application(name);
    }

@JavaScriptMethod
    public String[] loadApplications() {
	ArrayList<String> resps = new ArrayList<String>();
	HashMap<String,String> userAccounts = getUserAccounts();
	for(String url : userAccounts.keySet()) {
		String project = userAccounts.get(url);
		url = url.replace("XXX","applications?all=Y&");
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
