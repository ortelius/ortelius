package org.jenkinsci.plugins.deployhub;

import hudson.Extension;
import java.util.List;
import java.util.HashMap;
import java.util.ArrayList;
import org.kohsuke.stapler.bind.JavaScriptMethod;

import hudson.model.Action;

@Extension
public class Application extends Common {
    String appname;
    String basename;

    public String getIconFileName() {
        return "/plugin/deployhub/images/application.png";
    }

    public Application(String appname) {
	int p = appname.lastIndexOf(".");
	this.appname = appname;
	this.basename = (p>=0)?appname.substring(p+1):appname;
    }

    public Application() {
	System.out.println("Application constructor called with no id");
    }

    public String getDisplayName() {
	return basename;
    }

@JavaScriptMethod
    public String[] loadApplication() {
	ArrayList<String> resps = new ArrayList<String>();
	HashMap<String,String> userAccounts = getUserAccounts();
	for(String url : userAccounts.keySet()) {
		String project = userAccounts.get(url);
		url = url.replace("XXX","application/"+appname+"?").replace(" ","%20");
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
