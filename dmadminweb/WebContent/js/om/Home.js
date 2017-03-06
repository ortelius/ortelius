/*
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

function ShowHome(hbc)
{
	console.log("currenttree="+currenttree);
 if (displayhome)
 {
  $("#panel_container_right").hide();
  $("#panel_container_historyandpending").show();
 
  $("#homependingtab").html("<table id=\"homependingtab\"><tbody></tbody></table>");
  $("#homehisttab").html("<table id=\"homehisttab\"><tbody></tbody></table>");
  
  createNewsFeed("homependingtab","","",true); 
  createNewsFeed("homehisttab","","",false);
  console.log("hbc="+hbc);
  if (currenttree != "#domains_tree" && lastSelectedNode.substring(0, 2) == "do" && !hbc) {
	  var colours=["#eaedf0","#eaedf0","#eaedf0","#eaedf0","#eaedf0","#eaedf0","#eaedf0","#eaedf0"];
	  $.ajax({
		  url : "SubDomains?domainid=" + objid + "&ds=Y",
		  dataType : 'json',
		  type : 'GET',
		  success : function(res)
		  {
			  $("#domsumm").text("");
			  // Calculate max height for each subdomain
			  var maxdepth=$(window).height()-140;
			  console.log("initial maxdepth="+maxdepth+" res.length="+res.length);
			  if (res.length==0) {
				  // No sub-domains
				  $("#tabs-DomainSummary").hide();
				  return;

			  }
			  for (i=0;i<res.length;i++) {
				  if (!res[i].lifecycle) {
					  // not a lifecycle domain
					  $("#tabs-DomainSummary").hide();
					  return;
				  }
				  var uac = res[i].appcount;
				  var udh = (uac<3)?220:80+(uac*50);
				  var dac = 0;
				  for (i2=0;i2<res[i].envcount;i2++) {
					  console.log("res["+i+"].envs["+i2+"].appcount="+res[i].envs[i2].appcount);
					  var ac=res[i].envs[i2].appcount;
					  var dh=(ac<3)?220:80+(ac*50);
					  dac += dh
				  }
				  console.log(i+") uac="+uac+" dac="+dac);
				  if (udh+dac > maxdepth) maxdepth = udh+dac;
			  }
			  console.log("maxdepth="+maxdepth);
			  
			  for(i=0;i<res.length;i++) {
				  var x="<div style='border-radius: 5px; border:2px solid #d4dde7;background-color: "
					+colours[i]
				  	+";width:233px;height:"+maxdepth+"px;float:left'>"
				  	+"<table border=0><tr>"
				  	+"<td><img src='css/images/lifecycle_24x.png'></td>"
				  	+"<td valign='center'>"+res[i].name+"</td>"
				  	+"</tr></table><hr>";
				  // undeployed apps
				  x=x+"<div style='border-radius: 5px; border:2px solid #d4dde7;margin: 0 auto;background-color:#ffddaa;width:210px;min-height:190px;'>"
			  		+"<table border=0><tr>"
				  	+"<td valign='center'>"
				  	+"Undeployed Applications"
				  	+"</td>"
				  	+"</tr></table><hr>";
				  for (z=0;z<res[i].appcount;z++) {
					  switch(res[i].apps[z].status) {
					  case "Y":
						  icon="approved_24x.png";
						  break;
					  case "N":
						  icon="rejected_24x.png";
						  break;
					  default:
						  icon=(res[i].apps[z].pid>0)?"appversion_24x.png":"application_24x.png";
					  	  break;
					  }
					  console.log("icon for "+res[i].apps[z].name+" is "+icon);
					  var pf=(res[i].apps[z].pid>0)?"av":"ap";
					  if (z==0) {
						  
					  }
					  x=x+"<div style='border-radius: 5px; border:2px solid #d4dde7;margin: 0 auto;background-color:#ccbbaa;width:190px;height:35px;'>"
			  			+"<table border=0><tr>"
					  	+"<td><img src='css/images/"+icon+"'></td>"
					  	+"<td valign='center'>"
					  	+"<a href='javascript:SwitchDisplay(\""+pf+res[i].apps[z].id+"\");'><b>"+res[i].apps[z].name+"</b></a>"
					  	+"</td>"
					  	+"</tr></table>"
			  			+"</div>"
			  			+"<div style='height:10px;width:210px;margin 0 auto'></div>";
				  }
				  x=x+"</div><div style='height:10px;width:160px;margin 0 auto'></div>";
				  x=x+"<div style='height:10px;width:160px;margin 0 auto'></div>";
				  for (e=0;e<res[i].envcount;e++) {
				  		console.log("env="+res[i].envs[e].name);
				  		x=x+"<div style='border-radius: 5px; border:2px solid #d4dde7;margin: 0 auto;background-color:#ffeedd;width:210px;min-height:190px;'>"
				  		+"<table border=0><tr>"
					  	+"<td><img src='css/images/environment_24x.png'></td>"
					  	+"<td valign='center'>"
					  	+"<a href='javascript:SwitchDisplay(\"en"+res[i].envs[e].id+"\");'><b>"+res[i].envs[e].name+"</b></a>"
					  	+"</td>"
					  	+"</tr></table><hr>";
				  		// Apps in env
				  		console.log("res["+i+"].envs["+e+"]");
				  		console.log(res[i].envs[e]);
				  		for (a=0;a<res[i].envs[e].appcount;a++) {
				  			console.log("app="+res[i].envs[e].apps[a].name+" pid="+res[i].envs[e].apps[a].pid+" status="+res[i].envs[e].apps[a].status);
				  			switch(res[i].envs[e].apps[a].status) {
							  case "Y":
								  icon="approved_24x.png";
								  break;
							  case "N":
								  icon="rejected_24x.png";
								  break;
							  default:
								  icon=(res[i].envs[e].apps[a].pid>0)?"appversion_24x.png":"application_24x.png";
							  	  break;
				  			}
				  			console.log("icon="+icon);
				  			var pf=(res[i].envs[e].apps[a].pid>0)?"av":"ap";
				  			x=x+"<div style='border-radius: 5px; border:2px solid #d4dde7;margin: 0 auto;background-color:#ccbbaa;width:190px;height:35px;'>"
				  			+"<table border=0><tr>"
						  	+"<td><img src='css/images/"+icon+"'></td>"
						  	+"<td valign='center'>"
						  	+"<a href='javascript:SwitchDisplay(\""+pf+res[i].envs[e].apps[a].id+"\");'><b>"+res[i].envs[e].apps[a].name+"</b></a>"
						  	+"</td>"
						  	+"</tr></table>"
				  			+"</div>"
				  			+"<div style='height:10px;width:190px;margin 0 auto'></div>";
				  		}
				  		x=x+"</div><div style='height:20px;width:210px;margin 0 auto'></div>";
				  }
				  	x=x+"</div>"
				  	+"<div style='width:7px;height:20px;float:left'></div>";
				  	
				  $("#domsumm").append(x);
			  }
			  $("#tabs-DomainSummary").show();
		  },
		  error : function(jqxhr, status, err)
		  {
		   console.log(status);
		   console.log(err);
		  }
		 });
  } else {
	  $("#tabs-DomainSummary").hide();
  }
  displayhome=false;
 }
 else
 {
  $("#panel_container_historyandpending").hide();
  
  if (currenttree != "#domains_tree" && lastSelectedNode.substring(0, 2) == "do")
   $("#panel_container_right").hide();
  else
   $("#panel_container_right").show(); 
 }
}


function ActivateHomeSubTabs(tabId)
{
 var menuList = ["#tabs-HomePending", "#tabs-HomeHistory", "#tabs-DomainSummary", "#tabs-HomeDashboard"];
 
 for (i=0;i<menuList.length;i++)
 {
   var key = menuList[i];
   var tmp = key + "-data";
   $(key).removeClass('current');
   $(key).removeClass('active');
   $(tmp).hide();
 }

 console.log(tabId);

 if ($("#" + tabId).length > 0)
 {
  $("#" + tabId).addClass('current');
  $("#" + tabId).addClass('active');
  var tmp = "#" + tabId + "-data";
  $(tmp).show();
 }
 
}