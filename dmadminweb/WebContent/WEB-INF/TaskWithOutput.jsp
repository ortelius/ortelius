<%--
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
 <div id="tasktabs" style="height: 100%;overflow:auto">
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
