package org.jenkinsci.plugins.deployhub;
import hudson.Launcher;
import hudson.Extension;
import hudson.util.FormValidation;
import hudson.model.AbstractBuild;
import hudson.model.BuildListener;
import hudson.model.AbstractProject;
import hudson.tasks.Publisher;
import hudson.tasks.Recorder;
import hudson.tasks.BuildStepDescriptor;
import hudson.tasks.BuildStepMonitor;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;
import net.sf.json.JSONException;
import org.kohsuke.stapler.DataBoundConstructor;
import org.kohsuke.stapler.StaplerRequest;
import org.kohsuke.stapler.QueryParameter;

import javax.servlet.ServletException;
// import java.io.IOException;

import java.io.*;
import java.net.*;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import hudson.EnvVars;

/**
 * Sample {@link Builder}.
 *
 * <p>
 * When the user configures the project and enables this builder,
 * {@link DescriptorImpl#newInstance(StaplerRequest)} is invoked
 * and a new {@link DeployHubPlugin} is created. The created
 * instance is persisted to the project configuration XML by using
 * XStream, so this allows you to use instance fields (like {@link #name})
 * to remember the configuration.
 *
 * <p>
 * When a build is performed, the {@link #perform(AbstractBuild, Launcher, BuildListener)}
 * method will be invoked. 
 *
 * @author Kohsuke Kawaguchi
 */



public class DeployHubPlugin extends Recorder {

    private final String username;
    private final String password;
    private List<JSONObject> deployApplication;
    private List<JSONObject> useComponent;

    private boolean debug=false;
/*
private void WriteDebug(String dbline)
{
  PrintWriter pw = null;

  try {
     File file = new File("/tmp/debug.txt");
     FileWriter fw = new FileWriter(file, true);
     pw = new PrintWriter(fw);
     pw.println(dbline);
  } catch (IOException e) {
     e.printStackTrace();
  } finally {
     if (pw != null) {
        pw.close();
     }
  }
}
*/

@Override
public BuildStepMonitor getRequiredMonitorService()
{
    return BuildStepMonitor.NONE;
}
    

    // Fields in config.jelly must match the parameter names in the "DataBoundConstructor"
    @DataBoundConstructor
    public DeployHubPlugin(	String username,
				String password,
				List<JSONObject> deployApplication,
				List<JSONObject> useComponent) {
	this.username = username;
	this.password = password;
	this.deployApplication = deployApplication;
	this.useComponent = useComponent;
    }


    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }

    public List<JSONObject> getDA() {
	return deployApplication;
    }

    public boolean getDeployApplication() {
	return (getDA() != null);
    }

    private JSONArray getJA(JSONObject x,String y) {
	JSONArray ja = null;
	try {
		ja = x.getJSONArray(y);
	} catch(JSONException ex) {
		ja = null;
	}
	return ja;
    }

    private JSONObject getJO(JSONObject x,String y) {
	JSONObject jo = null;
	try {
		jo = x.getJSONObject(y);
		if (jo.isNullObject()) jo = null;
	} catch(JSONException ex) {
		jo = null;
	}
	return jo;
    }


    public String getApplication() {
	List<JSONObject> da = getDA();
	String appname="";
	if (da != null) {
		for (JSONObject a: da) {
			appname = a.getString("application");
		}
	}
	return appname;
    }

    public String getEnvironment() {
	List<JSONObject> da = getDA();
	String envname="";
	if (da != null) {
		for (JSONObject a: da) {
			envname = a.getString("environment");
		}
	}
	return envname;
    }

    public boolean getWait() {
	List<JSONObject> da = getDA();
	boolean res=false;
	if (da != null) {
		for (JSONObject a: da) {
			res = a.getBoolean("wait");
		}
	}
	return res;
    }

    public boolean isWait() {
	return getWait();
    }

    private List<JSONObject> getUA() {
	System.out.println("in getUA");
	List<JSONObject> da = getDA();
	List<JSONObject> res = new ArrayList<JSONObject>();
	boolean found=false;
	if (da != null) {
		for (JSONObject a: da) {
			try {
				JSONObject f = getJO(a,"useAdvanced");
				if (f != null) {
					res.add(f);
					found=true;
				}
			} catch(JSONException ex) {
				System.out.println("Caught JSONException");
			}
		}
	}
	System.out.println("exiting getUA, found="+found);
	return found?res:null;
    }

    private List<JSONObject> getFLV() {
	System.out.println("in getFLV");
	List<JSONObject> ua = getUA();
	List<JSONObject> res = new ArrayList<JSONObject>();
	boolean found=false;
	if (ua != null) {
		for (JSONObject a: ua) {
			try {
				JSONObject f = getJO(a,"findLatestVersion");
				if (f != null) {
					res.add(f);
					found=true;
				}
			} catch(JSONException ex) {
				System.out.println("Caught JSONException");
			}
		}
	} 
	System.out.println("exiting getFLV, found="+found);
        return found?res:null;
    }

    public List<JSONObject> getAppatts() {
	List<JSONObject> ua = getDA();
	List<JSONObject> ret = new ArrayList<JSONObject>();
	try {
		// Horrible Kludge time. If there's only one component attribute, it's only a single object.
		// Not - as you might hope or expect - an array with one component attribute in it. So try
		// grabbing the array and if that fails, default grabbing an object
		if (ua==null) return ret; 
		for (JSONObject a: ua) {
			try {
				JSONArray appatts = a.getJSONArray("appatts"); // if not an array, throws JSONException
				for (int i = 0; i < appatts.size(); i++) {
					ret.add(appatts.getJSONObject(i));
				}
			} catch(JSONException ex) {
				// Not an array - just grab the object
				JSONObject appatt = a.getJSONObject("appatts");
				ret.add(appatt);
			}
		}
	} catch(JSONException ex) {
		ret = null;
	}
	return ret;
    }

    public boolean getFindLatestVersion() {
	System.out.println("getFindLatestVersion()");
	return (getFLV() != null);
    }

    public String getBranchName() {
	List<JSONObject> flv = getFLV();
	String bn = "";
	if (flv != null) {
		for (JSONObject f: flv) {
			bn = f.getString("branchName");
		}
	}
	return bn;
    }

    private List<JSONObject> getUC() {
	return useComponent;
    }

    public boolean getUseComponent() {
	return (getUC() != null);
    }

    public List<JSONObject> getCompatts() {
	List<JSONObject> uc = getUC();
	List<JSONObject> ret = new ArrayList<JSONObject>();
	try {
		// Horrible Kludge time. If there's only one component attribute, it's only a single object.
		// Not - as you might hope or expect - an array with one component attribute in it. So try
		// grabbing the array and if that fails, default grabbing an object
		if (uc==null) return ret;
		for (JSONObject a: uc) {
			try {
				JSONArray compatts = a.getJSONArray("compatts"); // if not an array, throws JSONException
				for (int i = 0; i < compatts.size(); i++) {
					ret.add(compatts.getJSONObject(i));
				}
			} catch(JSONException ex) {
				// Not an array - just grab the object
				JSONObject compatt = a.getJSONObject("compatts");
				ret.add(compatt);
			}
		}
	} catch(JSONException ex) {
		ret = null;
	}
	return ret;
    }

    public String getComponentName() {
	List<JSONObject> uc = getUC();
	String cn = "";
	if (uc != null) {
		for(JSONObject c: uc) {
			cn = c.getString("componentName");
		}
	}
	return cn;
    }

    private List<JSONObject> getCNC() {
	System.out.println("getCNC()");
	List<JSONObject> uc = getUC();
	List<JSONObject> ret = new ArrayList<JSONObject>();
	if (uc != null) {
		for (JSONObject c: uc) {
			JSONObject cnc = getJO(c,"createNewComponent");
			System.out.println("got an object: "+cnc.toString());
			ret.add(cnc);
		}
	}
	return ret;
    }

    public String getComponentBranchName() {
	List<JSONObject> cnc = getCNC();
	String ret = "";
	if (cnc != null) {
		for(JSONObject c: cnc) {
			ret = c.getString("componentBranchName");
		}
	}
	return ret;
    }

    public boolean getCreateNewComponent() {
	return (getCNC() != null);
    }



    public boolean getUseAdvanced() {
	System.out.println("in getUseAdvanced");
	if (getUA() != null) {
		for (JSONObject debug: getUA()) {
			System.out.println("debug="+debug.toString());
		}
	} else {
	System.out.println("getUA returns NULL");
	}
	return (getUA() != null);
    }



    private JSONObject getCNV() {
	List<JSONObject> flv = getFLV();
	JSONObject res = null;
	if (flv != null) {
		for (JSONObject f: flv) {
			res = getJO(f,"createNewVersion");
		}
	}
	return res;
    }

    public boolean getCreateNewVersion() {
        return (getCNV() != null);
    }

    public String getTaskname() {
	JSONObject cnv = getCNV();
	String taskname = "";
	try {
		taskname = cnv.getString("taskname");
	} catch(JSONException ex) {
		taskname = "";
	}
        return taskname;
    }

    private JSONObject SendMessage(BuildListener listener,CookieManager cm,String urlstr)
    {
	if (debug) listener.getLogger().println("DEBUG: "+urlstr);
	JSONObject err = new JSONObject();
	err.element("success",false);
	try {
		URL url = new URL(urlstr);
		HttpURLConnection conn = (HttpURLConnection)url.openConnection();
		conn.setRequestMethod("GET");
		conn.setRequestProperty("Accept","application/json");
		if (conn.getResponseCode() != 200) {
			return err.element("error",conn.getResponseCode());
		}
		CookieStore cookieJar =  cm.getCookieStore();
		List <HttpCookie> cookies = cookieJar.getCookies();
		String reply="";
		BufferedReader br = new BufferedReader(new InputStreamReader(conn.getInputStream()));
		String l = null;
		while ((l=br.readLine())!=null) {
			if (debug) listener.getLogger().println("DEBUG: "+l);
			reply=reply+l;
		}
		br.close();
		JSONObject res=JSONObject.fromObject(reply);
		conn.disconnect();
		return res;
	}
	catch(MalformedURLException ex) {
		return err.element("error","Malformed URL: "+urlstr);
	}
	catch(IOException ex) {
		return err.element("error","IO Exception: "+ex.getMessage());
	}
    }

    @Override
    public boolean perform(AbstractBuild build, Launcher launcher, BuildListener listener) {

	String server = getDescriptor().getServerURL();

	debug=false;

	try {
		EnvVars e = build.getEnvironment(listener);

		String expapp = e.expand(getApplication());
		String expenv = e.expand(getEnvironment());
		String branchname = getBranchName();
		String expbranch = (branchname != null && branchname.length() > 0)?e.expand(branchname):null;

		if (debug) {
			listener.getLogger().println("Server is " + server);
			listener.getLogger().println("Application is " + expapp);
			listener.getLogger().println("Environment is " + expenv);
		}

		String buildno = e.expand("$BUILD_NUMBER");
		System.out.println("Build Number is "+buildno);
		String jobname = e.expand("$JOB_NAME");
		System.out.println("Job Name is "+jobname);
		//
		// BUILD_URL is in form http://hostname:8081/job/<jobname>/<buildno>
		// Release Engineer can use this to determine the Build Engine
		// (hostname:8081) and Build Job (off <jobname>) - it can then
		// add the build number to dm.dm_buildhistory.
		//
		String buildurl = e.expand("$BUILD_URL");
		System.out.println("Build URL is "+buildurl);

		CookieManager cm = new CookieManager();
		cm.setCookiePolicy(CookiePolicy.ACCEPT_ALL);
		CookieHandler.setDefault(cm);

		//
		// Login
		// -----
		//
		String urlstr = server + "/dmadminweb/API/login"
		+"?user="+URLEncoder.encode(username, "UTF-8")
		+"&pass="+URLEncoder.encode(password, "UTF-8");
		//listener.getLogger().println("DEBUG: urlstr="+urlstr);
		JSONObject res = SendMessage(listener,cm,urlstr);
		//listener.getLogger().println("JSON Result = "+res.toString());
		if (res.getBoolean("success") == false) {
			listener.getLogger().print("Failed to login to Release Engineer as user "+username+":");
			listener.getLogger().println(res.getString("error"));
			return false;
		}
		listener.getLogger().println("Logged in to Release Engineer as user "+username);

		JSONObject app = null;
		int appid = 0;
		String appname = null;

		boolean newVersionCreated = false;
		
		//
		// Are we deploying anything?
		// --------------------------
		//
		if (getDeployApplication()) {
			// Yes we're going to deploy the app
			System.out.println("Deploy Application is set");
			//
			// Find version to deploy
			// ----------------------
			//
			if (getFindLatestVersion()) {
				//
				// Find latest version
				//
				urlstr = server + "/dmadminweb/API/application/"
				+URLEncoder.encode(expapp,"UTF-8")
				+"?latest=Y";
				if (expbranch != null) urlstr += "&branch="+URLEncoder.encode(expbranch,"UTF-8");
			} else {
				//
				// Use supplied version
				//
				urlstr = server + "/dmadminweb/API/application/"
				+URLEncoder.encode(expapp,"UTF-8");
			}
			JSONObject a = SendMessage(listener,cm,urlstr);
			if (a.getBoolean("success") == false) {
				listener.getLogger().println(a.getString("error"));
				return false;
			}
			app = getJO(a,"result");
			appid = app.getInt("id");
			appname = app.getString("name");
			if (debug) listener.getLogger().println("appid="+appid);
			//
			// Do we need to create a new app version?
			// ---------------------------------------
			//
			if (getCreateNewVersion()) {
				// Check if the latest version is approved
				JSONArray applist = getJA(app,"approvals");
				if (applist.size() > 0) {
					// It's got at least one approval - create new version
					listener.getLogger().println("Application "+appname+" is approved - creating new version");
					
					urlstr = server + "/dmadminweb/API/newappver/"
					+appid
					+"?taskname="+URLEncoder.encode(getTaskname(),"UTF-8");
					JSONObject na = SendMessage(listener,cm,urlstr);
					if (na.getBoolean("success") == false) {
						listener.getLogger().println(na.getString("error"));
						return false;
					}
					app = getJO(na,"result");
					System.out.println("app is "+app);
					appid = app.getInt("id");
					appname = app.getString("name");
					System.out.println("appid="+appid+" appname="+appname);
					listener.getLogger().println("New Application Version "+appname+" created");
					newVersionCreated = true;
				}
			}
			//
			// Are we making changes to the Component?
			// ---------------------------------------
			//
			if (getUseComponent()) {
				String cn = getComponentName();
				if (cn.length() == 0) {
					// Component Name not specified
					listener.getLogger().println("Component Name not specified");
					return false;
				}
				// Find the version of this component associated with this application
				urlstr = server + "/dmadminweb/API/component/"+URLEncoder.encode(cn,"UTF-8");
				JSONObject c = SendMessage(listener,cm,urlstr);
				if (c.getBoolean("success") == false) {
					listener.getLogger().println(c.getString("error"));
					return false;
				}
				JSONObject comp = getJO(c,"result");
				String compname = comp.getString("name");
				int compid = comp.getInt("id");
				JSONArray cvs = getJA(comp,"versions");
				ArrayList<Integer> compids = new ArrayList<Integer>();
				compids.add(compid);
				if (cvs != null) {
					// Base version specified - add the versions
					for (int i = 0; i < cvs.size(); i++) {
						JSONObject c2 = cvs.getJSONObject(i);
						compids.add(c2.getInt("id"));
					}
				}
				if (debug) listener.getLogger().println("compids="+compids);
				//
				// Okay, at this point compids is a list of the component id and its version ids.
				// See if any of these IDs are associated with this application
				//
				if (debug) listener.getLogger().println("app="+app);
				JSONArray appcomps = getJA(app,"components");
				compid=0;
				if (debug) listener.getLogger().println("appcomps="+appcomps);
				if (appcomps != null) {
					for (int i = 0; i < appcomps.size(); i++) {
						JSONObject ac = appcomps.getJSONObject(i);
						if (debug) listener.getLogger().println("ac="+ac);
						int aci = ac.getInt("id");
						if (debug) listener.getLogger().println("aci="+aci);
						int mc = compids.indexOf(aci);
						if (mc>=0) {
							// Match
							compid = compids.get(mc);
							compname = ac.getString("name");
							listener.getLogger().println("Component "+compname+" is associated with application "+appname);
							break;
						} 
					}
				}
				if (compid == 0) {
					// Component not found - bail out
					listener.getLogger().println("No version of Component "+cn+" is present in Application "+appname);
					return false;
				}
				//
				// Get the details for this matched component
				//
				urlstr = server + "/dmadminweb/API/component/"+compid;
				JSONObject cx = SendMessage(listener,cm,urlstr);
				if (cx.getBoolean("success") == false) {
					listener.getLogger().println(cx.getString("error"));
					return false;
				}
				comp = getJO(cx,"result");
				compname = comp.getString("name");
				compid = comp.getInt("id");
				//
				// If we've created a new application version then we need to create a new component version and
				// associate it with this new application version. If we've NOT created a new application version
				// then we only create a new version of the component if it's associated with ANY application
				// version that's currenty approved.
				//
				boolean CreateNewComponentVersion=false;
				if (!newVersionCreated) {
					// Check to see if any of the applications associated with this component are approved
					JSONArray applist = getJA(comp,"applications");
					if (applist != null) {
						for (int i = 0; i < applist.size(); i++) {
							JSONObject ta = applist.getJSONObject(i);
							if (debug) listener.getLogger().println("ta="+ta);
							JSONArray tapp = getJA(ta,"approvals");
							if (tapp != null && tapp.size() > 0) {
								// This app has been approved
								listener.getLogger().println("Component "+comp.getString("name")+" is associated with an approved application "+ta.getString("name"));
								CreateNewComponentVersion=true;
								break;
							}
						}
					}
				}
				if (newVersionCreated || CreateNewComponentVersion) {
					// Need to create a new component version
					urlstr = server + "/dmadminweb/API/newcompver/"
					+compid;
					JSONObject nc = SendMessage(listener,cm,urlstr);
					if (nc.getBoolean("success") == false) {
						listener.getLogger().println(nc.getString("error"));
						return false;
					}
					JSONObject newcomp = getJO(nc,"result");
					int newcompid = newcomp.getInt("id");
					String newcompname = newcomp.getString("name");
					listener.getLogger().println("New Component Version "+newcomp.getString("name")+" created");
					if (newVersionCreated) {
						// Need to replace the old component in the new application with this newly
						// created component
						urlstr = server + "/dmadminweb/API/replace/"+appid+"/"+compid+"/"+newcompid;
						nc = SendMessage(listener,cm,urlstr);
						if (nc.getBoolean("success") == false) {
							listener.getLogger().println(nc.getString("error"));
							return false;
						}
						listener.getLogger().println("Component "+compname+" replaced with new version "+newcompname+" in Application "+appname);
					}
					comp = newcomp;
					compid = newcompid;
					compname = newcompname;
				}
				//
				// Now set the component attributes
				//
				boolean titlePrinted=false;
				List<JSONObject> compatts = getCompatts();
				if (compatts != null) {
					for (JSONObject xa: compatts) {
						String name = null;
						String value = null;
						try {
							name = xa.getString("cname");
							value = xa.getString("cvalue");
						} catch(JSONException ex) {
							name = null;
							value = null;
						}
						String expname = (name != null)?e.expand(name):null;
						String expvalue = (value != null)?e.expand(value):null;
						if (expname != null && expvalue != null && expname.length() > 0 && expvalue.length() > 0) {
							if (!titlePrinted) {
								listener.getLogger().println("Setting Attributes on Component \""+compname+"\"");
								titlePrinted=true;
							}
							listener.getLogger().println("Setting \""
							+expname
							+"\" to \""
							+expvalue+"\""
							);
							// Set the name and value for the specified application
							urlstr = server + "/dmadminweb/API/setvar/component/"
							+compid
							+"?name="+URLEncoder.encode(expname,"UTF-8")
							+"&value="+URLEncoder.encode(expvalue,"UTF-8");;
							JSONObject res2 = SendMessage(listener,cm,urlstr);
							if (res2.getBoolean("success") == false) {
								listener.getLogger().println(res2.getString("error"));
								return false;
							}
						}
					}
				}
			}
			//
			// Step 4 - Set the application attributes
			// ---------------------------------------
			//
			boolean titlePrinted=false;
			List<JSONObject> appatts = getAppatts();
			if (appatts != null) {
				for (JSONObject aa: appatts) {
					if (!titlePrinted) {
						listener.getLogger().println("Setting Attributes on Application \""+appname+"\"");
						titlePrinted=true;
					}
					try {
						String name = aa.getString("name");
						String value = aa.getString("value");
						String expname = (name != null)?e.expand(name):null;
						String expvalue = (value != null)?e.expand(value):null;
						listener.getLogger().println("Setting \""
						+expname
						+"\" to \""
						+expvalue+"\""
						);
						// Set the name and value for the specified application
						urlstr = server + "/dmadminweb/API/setvar/application/"
						+appid
						+"?name="+URLEncoder.encode(expname,"UTF-8")
						+"&value="+URLEncoder.encode(expvalue,"UTF-8");;
						JSONObject res2 = SendMessage(listener,cm,urlstr);
						if (res2.getBoolean("success") == false) {
							listener.getLogger().println(res2.getString("error"));
							return false;
						}
					} catch(JSONException ex) {
						// Exception will be thrown if "name" or "value" aren't set to anything
					}
				}
			}
			//
			// Send the buildurl to Release Engineer now. This is to ensure lastbuild is set on the component
			//
			urlstr = server + "/dmadminweb/API/buildnotify?buildurl="+URLEncoder.encode(buildurl,"UTF-8");
			res = SendMessage(listener,cm,urlstr);
			System.out.println("res="+res.toString());
			//
			// Step 5 - Application ready to deploy
			// ------------------------------------
			//
			listener.getLogger().print("Deploying application "+appname);
			listener.getLogger().println(" to environment "+expenv);
			String waitstr="N";
			if (getWait()) waitstr="Y";
			urlstr = server + "/dmadminweb/API/deploy/"
			+ appid
			+"?env="+URLEncoder.encode(expenv,"UTF-8")
			+"&wait="+waitstr;
			JSONObject depres = SendMessage(listener,cm,urlstr);
			if (depres.getBoolean("success") == false) {
				listener.getLogger().println(depres.getString("error"));
				return false;
			}
			int deploymentid = depres.getInt("deploymentid");
			if (getWait()) {
				//
				// We are set to wait - we will only return to here when the deployment is complete
				// --------------------------------------------------------------------------------
				//
				urlstr = server + "/dmadminweb/API/log?id=" + deploymentid;
				JSONObject logres = SendMessage(listener,cm,urlstr);
				if (logres.getBoolean("success") == false) {
					listener.getLogger().println(logres.getString("error"));
					return false;
				}
				JSONArray logoutput = getJA(logres,"logoutput");
				if (logoutput != null) {
					for (int i = 0; i < logoutput.size(); i++) {
						String logline = logoutput.getString(i);
						listener.getLogger().println(logline);
					}
				} else {
					listener.getLogger().println("could not find logoutput in returned JSON");
					return false;
				}
			} else {
				//
				// Report the deployment ID
				// ------------------------
				//
				listener.getLogger().println("Deployment #"+deploymentid+" started");
			}
		} else {
			//
			// Not deploying anything - just notify Release Engineer that we've done a build
			// and let it find the affected components.
			//
			urlstr = server + "/dmadminweb/API/buildnotify?buildurl="+URLEncoder.encode(buildurl,"UTF-8");
			res = SendMessage(listener,cm,urlstr);
			System.out.println("res="+res.toString());
		}
		return true;
	}
	catch(IOException ex) {
		listener.getLogger().println("IO Exception: "+ex.getMessage());
	}
	catch(InterruptedException ex) {
		listener.getLogger().println("Interrupted Exception: "+ex.getMessage());
	}
	/*
	catch(URISyntaxException ex) {
		listener.getLogger().println("URISyntaxException: "+ex.getMessage());
	}
	*/
	return false;
    }

    // Overridden for better type safety.
    // If your plugin doesn't really define any property on Descriptor,
    // you don't have to do this.
    @Override
    public DescriptorImpl getDescriptor() {
        return (DescriptorImpl)super.getDescriptor();
    }

    /**
     * Descriptor for {@link DeployHubPlugin}. Used as a singleton.
     * The class is marked as public so that it can be accessed from views.
     *
     * <p>
     * See <tt>src/main/resources/hudson/plugins/release_engineer/DeployHubPlugin/*.jelly</tt>
     * for the actual HTML fragment for the configuration screen.
     */
    @Extension // This indicates to Jenkins that this is an implementation of an extension point.
    public static final class DescriptorImpl extends BuildStepDescriptor<Publisher> {
        /**
         * To persist global configuration information,
         * simply store it in a field and call save().
         *
         * <p>
         * If you don't want fields to be persisted, use <tt>transient</tt>.
         */
        private String serverURL;

        /**
         * In order to load the persisted global configuration, you have to 
         * call load() in the constructor.
         */
        public DescriptorImpl() {
            load();
        }

        /**
         *
         * @param value
         *      This parameter receives the value that the user has typed.
         * @return
         *      Indicates the outcome of the validation. This is sent to the browser.
         *      <p>
         *      Note that returning {@link FormValidation#error(String)} does not
         *      prevent the form from being saved. It just means that a message
         *      will be displayed to the user. 
         */
	//
	// Note, the function should be named doCheck<FieldName>
	//
        public FormValidation doCheckApplication(@QueryParameter String value)
                throws IOException, ServletException {
            if (value.length() == 0)
                return FormValidation.error("Please set a value for the application");
            return FormValidation.ok();
        }

        public FormValidation doCheckEnvironment(@QueryParameter String value)
                throws IOException, ServletException {
            if (value.length() == 0)
                return FormValidation.error("Please set a value for the environment");
            return FormValidation.ok();
        }

        public FormValidation doCheckUsername(@QueryParameter String value)
                throws IOException, ServletException {
            if (value.length() == 0)
                return FormValidation.error("Please set a value for the username");
            return FormValidation.ok();
        }

        public FormValidation doCheckPassword(@QueryParameter String value)
                throws IOException, ServletException {
            if (value.length() == 0)
                return FormValidation.error("Please set a value for the password");
            return FormValidation.ok();
        }
/*
        public boolean isApplicable(Class<? extends AbstractProject> aClass) {
            // Indicates that this builder can be used with all kinds of project types 
            return true;
        }
*/
        public boolean isApplicable(Class aClass) {
            // Indicates that this builder can be used with all kinds of project types 
            return true;
        }

        /**
         * This human readable name is used in the configuration screen.
         */
        public String getDisplayName() {
            return "Release Engineer Interface";
        }

        @Override
        public boolean configure(StaplerRequest req, JSONObject formData) throws FormException {
            // To persist global configuration information,
            // set that to properties and call save().
            this.serverURL = formData.getString("serverURL");
            save();
            return super.configure(req,formData);
        }

        public String getServerURL() {
            return serverURL;
        }
    }
}

