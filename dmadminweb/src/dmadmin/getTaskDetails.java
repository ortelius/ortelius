/*
 * Copyright (c) 2021 Linux Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package dmadmin;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
// import javax.servlet.http.HttpSession;

import dmadmin.model.Domain;
import dmadmin.model.Task;
import dmadmin.model.Task.TaskType;
import dmadmin.model.TaskApprove;
import dmadmin.model.TaskCreateVersion;
import dmadmin.model.TaskDeploy;
import dmadmin.model.TaskList;
import dmadmin.model.TaskMove;
import dmadmin.model.TaskRemove;
import dmadmin.model.TaskRequest;

/**
 * Servlet implementation class getTaskDetails
 */
public class getTaskDetails extends HttpServletBase {
	private static final long serialVersionUID = 1L;

    /**
     * @see HttpServlet#HttpServlet()
     */
    public getTaskDetails() {
        super();
        // TODO Auto-generated constructor stub
    }

    @Override
    public void handleRequest(DMSession session, boolean isPost,
   			HttpServletRequest request, HttpServletResponse response)
   		throws ServletException, IOException
   	{
    	int tid = ServletUtils.getIntParameter(request, "tid");	// task id
		String tts = request.getParameter("tt");
		Task.TaskType tt = Task.TaskType.fromString(tts);
		String cbfunc = request.getParameter("cbfunc");
		request.setAttribute("tid", tid);
		request.setAttribute("cbfunc",cbfunc);

		switch(tt) {
		case APPROVE: {
			System.out.println("Getting task approve (in getTaskData)");
			TaskApprove task = session.getTaskApprove(tid);
			System.out.println("getTaskData: task.getSuccessTemplate="+(task.getSuccessTemplate() == null?"null":"not null"));
			Domain domain = task.getApprovalDomain();
			request.setAttribute("task",task);
			request.setAttribute("domain",domain);
			request.getRequestDispatcher("/WEB-INF/TaskApprove.jsp").forward(request, response);
			}
			break;
		case MOVE: {
			TaskMove task = session.getTaskMove(tid);
			Domain domain = task.getTargetDomain();
			request.setAttribute("task",task);
			request.setAttribute("domain",domain);
			request.setAttribute("corm","Move");
			request.setAttribute("uaoption","utmp");
			request.getRequestDispatcher("/WEB-INF/TaskMoveCopy.jsp").forward(request, response);
			}
			break;
		case REMOVE: {
			TaskRemove task = session.getTaskRemove(tid);
			request.setAttribute("task",task);
			request.getRequestDispatcher("/WEB-INF/TaskMoveCopy.jsp").forward(request, response);
			}
			break;
		case DEPLOY: {
			TaskDeploy task = session.getTaskDeploy(tid);
			request.setAttribute("task",task);
			request.getRequestDispatcher("/WEB-INF/TaskDeploy.jsp").forward(request, response);
			}
			break;
		case REQUEST: {
			TaskRequest task = session.getTaskRequest(tid);
			Task linkedTask = task.getLinkedTask();
			request.setAttribute("task",task);
			request.setAttribute("linktask",linkedTask);
			TaskList tasks = session.getTasksInDomain(task.getDomainId());
			TaskList tasksExcludingRequests = new TaskList();
			for (Task t: tasks) {
				if (t.getTaskType() != TaskType.REQUEST) {
					tasksExcludingRequests.add(t);
				}
			}
			request.setAttribute("tasklist",tasksExcludingRequests);
			request.getRequestDispatcher("/WEB-INF/TaskRequest.jsp").forward(request, response);
			}
			break;
		case CREATE_VERSION:
			TaskCreateVersion taskCrVer = session.getTaskCreateVersion(tid);
			Domain domain = taskCrVer.getTargetDomain();
			request.setAttribute("task",taskCrVer);
			request.setAttribute("domain",domain);
			request.getRequestDispatcher("/WEB-INF/TaskCreateVersion.jsp").forward(request, response);
			break;
		case UNKNOWN:
			break;
		default:
			break;
		}
   	}
}
