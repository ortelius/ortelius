package org.jenkinsci.plugins.deployhub;

import hudson.Extension;
import java.util.List;
import java.util.HashMap;
import java.util.ArrayList;
import org.kohsuke.stapler.bind.JavaScriptMethod;

import hudson.model.Action;

@Extension
public class Components extends Common {
    public String getIconFileName() {
        return "/plugin/deployhub/images/component.png";
    }

    public Action getDynamic(String name) {
	System.out.println("getDynamic (Components) name=["+name+"]");
	return new Component(name);
    }

@JavaScriptMethod
    public String[] loadComponents() {
	ArrayList<String> resps = new ArrayList<String>();
	HashMap<String,String> userAccounts = getUserAccounts();
	for(String url : userAccounts.keySet()) {
		String project = userAccounts.get(url);
		url = url.replace("XXX","components?all=Y&");
		System.out.println(url);
		try {
			String res = httpGet(url);
			System.out.println("res="+res);
			resps.add(res);
		} catch(Exception ex) {
			System.out.println("ex="+ex.getMessage());
		}
	}
	return resps.toArray(new String[resps.size()]);
    }
}

