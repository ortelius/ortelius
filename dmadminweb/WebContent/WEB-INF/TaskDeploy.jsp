 <%--
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