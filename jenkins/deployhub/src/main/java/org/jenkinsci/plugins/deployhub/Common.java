package org.jenkinsci.plugins.deployhub;

import hudson.model.Hudson;
import hudson.model.TopLevelItem;
import hudson.model.TopLevelItemDescriptor;
import hudson.XmlFile;
import hudson.Extension;
import hudson.model.Action;
import jenkins.model.ModelObjectWithContextMenu;
import org.kohsuke.stapler.StaplerRequest;
import org.kohsuke.stapler.StaplerResponse;
import jenkins.model.Jenkins.DescriptorImpl;
import java.io.*;
import java.util.List;
import org.kohsuke.stapler.bind.JavaScriptMethod;

// for XML parsing
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import java.util.HashMap;

// For calls to the API
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public abstract class Common implements Action, ModelObjectWithContextMenu {

    public String getDisplayName() {
	return getClass().getSimpleName();
    }

    public String getUrlName() {
	return getClass().getSimpleName();
    }

    public List<DeployHubMenu> getAll() {
        return null;
    }

    public String getViewUrl() {
	return "deployhub";
    }

    public ContextMenu doContextMenu(StaplerRequest request, StaplerResponse response) throws Exception {
        return new ContextMenu().addAll(getAll());
    }

    public static String getServerURL()
    {
	String rootDir = Hudson.getInstance().getRootDir().getAbsolutePath();
        XmlFile t = new XmlFile(Hudson.XSTREAM, new File(rootDir, "org.jenkinsci.plugins.deployhub.DeployHub.xml"));
        if (t != null && t.exists()) {
                try {   
                        DeployHub.DescriptorImpl desc = (DeployHub.DescriptorImpl)t.read();
                        if (desc != null) return desc.getServerURL();
                        return "";
                } catch(IOException ex) {
                }
        }
        return "";
    }

    public HashMap<String,String> getUserAccounts()
    {
	HashMap<String,String> res = new HashMap<String,String>();

	String baseurl = getServerURL();

	String rootDir = Hudson.getInstance().getRootDir().getAbsolutePath();
	String jobsDir = rootDir + "/jobs";
	// Get list of job folders
	File file = new File(jobsDir);
	String[] directories = file.list(new FilenameFilter() {
		@Override
		public boolean accept(File current, String name) {
			return new File(current, name).isDirectory();
		}
	});
	for (int i=0;i<directories.length;i++) {
		try {
			DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			File configFile = new File(jobsDir+"/"+directories[i], "config.xml");
			if (!configFile.exists()) continue;
			Document doc = dBuilder.parse(configFile);
			doc.getDocumentElement().normalize();
			Element baseNode = doc.getDocumentElement();
			NodeList publishers = baseNode.getElementsByTagName("publishers");
			if (publishers != null && publishers.getLength()>0) {
				Element p = (Element)(publishers.item(0));
				NodeList dhElements = p.getElementsByTagName("org.jenkinsci.plugins.deployhub.DeployHub");
				if (dhElements != null && dhElements.getLength()>0) {
					// Found our plugin as a post-build (publisher) step.
					Element n = (Element)(dhElements.item(0));
					Element username = (Element)(n.getElementsByTagName("username").item(0));
					Element password = (Element)(n.getElementsByTagName("password").item(0));
					if (username != null && password != null) {
						System.out.println("username="+username.getTextContent());	
						System.out.println("password="+password.getTextContent());
						String url=baseurl+"/dmadminweb/API/XXXuser="+username.getTextContent()+"&pass="+password.getTextContent();
						res.put(url,directories[i]);
					}
				}
			}
		} catch(Exception ex) {
			System.out.println("Exception ex = "+ex.getMessage());
		}
	}
	return res;
    }

    public String httpGet(String url) throws Exception {
	URL obj = new URL(url);
	HttpURLConnection con = (HttpURLConnection) obj.openConnection();

	con.setRequestMethod("GET");
	con.setRequestProperty("User-Agent","Mozilla/5.0");
	int responseCode = con.getResponseCode();
	BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
	String inputLine;
	StringBuffer response = new StringBuffer();

	while ((inputLine = in.readLine()) != null) {
		response.append(inputLine);
	}
	in.close();
	return response.toString();
    }
}
