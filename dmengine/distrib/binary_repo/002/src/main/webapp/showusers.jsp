<html>
<head>
 <title>Deployment Manager Demo</title>
 <link rel="stylesheet" href="css/style.css" type="text/css">
</head>
<body>
<%
	java.util.Collection<com.trinem.dmdemo.User> users = com.trinem.dmdemo.UserList.getUsers(request.getServerName());
%>
<div id="wrapper">
 <div id="content">
  <img src="images/logo.gif" align="right">
  <h2>Deployment Manager Demo</h2>
  
  <div id="padding"></div>
  
  <table cellspacing="0" cellpadding="5" class="bordered">
   <tr>
    <th class="thead">Firstname</th>
    <th class="thead">Surname</th>
    <th class="thead">Date of Birth</th>
   </tr>
<%
  for(com.trinem.dmdemo.User user: users)
  {
%>
   <tr>
    <td class="tleft" ><%= user.getFirstname() %></td>
    <td class="tleft"><%= user.getSurname() %></td>
    <td class="tright"><%= user.getDOB() %></td>
   </tr>
<%
  }
%>
  </table>

  <div id="padding"></div>
 </div>
</div>
</body>
</html>