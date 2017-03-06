package dmadmin;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import dmadmin.model.Application;
import dmadmin.model.Environment;

/**
 * Servlet implementation class GetAppVersInEnv
 */
public class GetAppVersInEnv
	extends HttpServletBase
{
	private static final long serialVersionUID = 1L;
       
    public GetAppVersInEnv() {
        super();
    }

	@Override
	public void handleRequest(DMSession session, boolean isPost,
			HttpServletRequest request, HttpServletResponse response)
		throws ServletException, IOException
	{
		int envid = getIntParameter(request, "envid", 0);
		int appid = getIntParameter(request, "appid", 0);
		if((envid == 0) && (appid == 0)) {
			throw new RuntimeException("Either envid or appid must be specified");
		}
		if(envid > 0) {
			Environment env = session.getEnvironment(envid, false);
			request.setAttribute("env", env);
			request.setAttribute("mode", "apps");
		}
		if(appid > 0) {
			Application app = session.getApplication(appid, false);
			request.setAttribute("app", app);
			request.setAttribute("mode", "envs");
		}
		request.setAttribute("admin", getStringParameter(request, "admin", "N").equalsIgnoreCase("Y"));
		request.getRequestDispatcher("/WEB-INF/GetAppsInEnv.jsp").forward(request, response);		
	}
}
