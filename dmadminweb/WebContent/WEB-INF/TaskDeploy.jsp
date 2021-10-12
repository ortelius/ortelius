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
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<body>

<script type="text/javascript">
var err1="<table width=100%><tr><td valign='top' width='55px'><img src='images/error.png'></td><td align='left' valign='top'><b><i>";
var err2="</i></b></td></tr></table>";
function TgtChg()
{
	// Need to validate app and target with some "pre-flight checks"
	var envs = document.getElementById("envid"); 
	var envid = envs.options[envs.selectedIndex].value;
	console.log("ValidateTask?tt=Deploy&appid=${app.id}&envid="+envid);
	$.ajax({
			async: false,
			url: "ValidateTask?tt=Deploy&appid=${app.id}&envid="+envid,
			type: 'POST',
			dataType: "json",
			success: function(data) {
				if (data.success) {
					parent.$("#modok").attr('disabled', false ).removeClass( 'ui-state-disabled' );
					
				} else {
					$("#errarea").html(err1+data.errtext+err2);
				}
			}
	});
}

</script>
  <table border=0 width="98%"><tr>
  <td valign="top">Deploying ${app.name}<br>${app.summary}</td>
  </tr></table>
  <hr>
  Target Environment <select id='envid' style='width:150px;' onChange='TgtChg();'>
  <option value="0">--- Please Select ---</option>
  <c:forEach items="${envs}" var="env">
     <option value="${env.id}">${env.name}</option>
  </c:forEach>
  </select>
  <div id="errarea"></div>
</html>