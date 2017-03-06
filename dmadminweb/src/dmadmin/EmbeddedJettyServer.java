/*
 *
 *  DeployHub is an Agile Application Release Automation Solution
 *  Copyright (C) 2017 Catalyst Systems Corporation DBA OpenMake Software
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package dmadmin;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.nio.file.Files;
import java.security.ProtectionDomain;
import java.util.ArrayList;
import java.util.List;

import org.apache.jasper.servlet.JspServlet;
import org.eclipse.jetty.annotations.ServletContainerInitializersStarter;
import org.eclipse.jetty.apache.jsp.JettyJasperInitializer;
import org.eclipse.jetty.plus.annotation.ContainerInitializer;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.servlet.DefaultServlet;
import org.eclipse.jetty.servlet.ServletHolder;
import org.eclipse.jetty.webapp.Configuration;
import org.eclipse.jetty.webapp.WebAppContext;

public class EmbeddedJettyServer {

	public static void main(String[] args) throws Exception{
		
		int port = Integer.parseInt(System.getProperty("port", "8080"));
        Server server = new Server(port);
        
        //System.setProperty("org.apache.jasper.compiler.disablejsr199", "false");

		//MBeanContainer mbContainer = new MBeanContainer( ManagementFactory.getPlatformMBeanServer() );
        //server.addBean( mbContainer );
		
        ProtectionDomain domain = EmbeddedJettyServer.class.getProtectionDomain();
        URL location = domain.getCodeSource().getLocation();
        
        String eclipse_running  = System.getProperty("eclipse.running", "false");
        if (eclipse_running.equals("true"))
        {
        	String app_home = domain.getCodeSource().getLocation().toString() + "../..";
        	location = new URL (app_home);
        }
        	
        WebAppContext webapp = new WebAppContext();
        webapp.setContextPath("/dmadminweb");
        webapp.setDescriptor(location.toExternalForm() + "/WEB-INF/web.xml");
        webapp.setServer(server);
        webapp.setWar(location.toExternalForm());
        
        JettyJasperInitializer sci = new JettyJasperInitializer();
        ServletContainerInitializersStarter sciStarter = new ServletContainerInitializersStarter(webapp);
        ContainerInitializer initializer = new ContainerInitializer(sci, null);
        List<ContainerInitializer> initializers = new ArrayList<ContainerInitializer>();
        initializers.add(initializer);

        webapp.setAttribute("org.eclipse.jetty.containerInitializers", initializers);
        webapp.addBean(sciStarter, true);
        /*webapp.setConfigurations(new Configuration[]{
                new AnnotationConfiguration(), new WebXmlConfiguration(),
                new WebInfConfiguration(),
                new PlusConfiguration(), new MetaInfConfiguration(),
                new FragmentConfiguration(), new EnvConfiguration()});
                */
		Configuration.ClassList classlist = Configuration.ClassList.setServerDefault( server );
        classlist.addBefore(
                "org.eclipse.jetty.webapp.JettyWebXmlConfiguration",
                "org.eclipse.jetty.annotations.AnnotationConfiguration" );
        
        ClassLoader jspClassLoader = new URLClassLoader(new URL[0], EmbeddedJettyServer.class.getClassLoader());
        webapp.setClassLoader(jspClassLoader);
            
        ServletHolder holderJsp = new ServletHolder("jsp", JspServlet.class);
        holderJsp.setInitOrder(0);
        holderJsp.setInitParameter("fork","false");
        holderJsp.setInitParameter("keepgenerated", "true");
        webapp.addServlet(holderJsp, "*.jsp");
            
        ServletHolder holderDefault = new ServletHolder("default", DefaultServlet.class);
        holderDefault.setInitParameter("dirAllowed", "true");
        webapp.addServlet(holderDefault, "/");  
        
		//classlist.addAfter("org.eclipse.jetty.webapp.FragmentConfiguration", "org.eclipse.jetty.plus.webapp.EnvConfiguration", "org.eclipse.jetty.plus.webapp.PlusConfiguration");
         
        // (Optional) Set the directory the war will extract to.
        // If not set, java.io.tmpdir will be used, which can cause problems
        // if the temp directory gets cleaned periodically.
        // Your build scripts should remove this directory between deployments

        File tmpdir = Files.createTempDirectory("re-").toFile();  
        if (!tmpdir.exists()) {
            if (!tmpdir.mkdirs()) {
                throw new IOException("Unable to create temp directory: " + tmpdir);
            }
        }
        File scdir = new File(tmpdir.toString(), "embedded-jetty-jsp");

            if (!scdir.exists()) {
                if (!scdir.mkdirs()) {
                    throw new IOException("Unable to create scratch directory: " + scdir);
                }
            }
		System.out.println("War Dir=" + tmpdir.toString());
        webapp.setTempDirectory(tmpdir);

        server.setHandler(webapp);
        server.start();
		server.dumpStdErr();
        server.join();

	}

}
