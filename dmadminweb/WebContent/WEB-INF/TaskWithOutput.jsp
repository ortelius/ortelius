 <%--
 *  Ortelius for Microservice Configuration Mapping
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
 --%>
<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
 <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
</head>
<body>
<style>
#tasktabs.ui-tabs .ui-tabs-panel {
	padding: 0px;
}
</style>
 <div id="tasktabs" style="height: 100%; font-size: 12px;overflow:auto">
  <ul>
   <li><a href="#tasktabs-Task">Task</a></li>
   <li><a href="#tasktabs-Output">Output</a></li>
  </ul>
  <div id="tasktabs-Task">
   <div id="taskcontent"></div>
  </div>
  <div id="tasktabs-Output">
   <div style="margin-right: 8px; border: 1px solid black; background-color: #fff; color: #000; width: 100%; height: 100%; overflow-x: wrap; overflow-y: scroll">
    <code id="tasklog"></code>
   </div>   
  </div>
 </div>
</body>
<script>
$(function()
{
	$( "#tasktabs" ).tabs();

	$("#taskcontent").html("Loading...")
		.load("RunTask?f=def2&tid=${task.id}&id=${id}");
})
</script>
</html>