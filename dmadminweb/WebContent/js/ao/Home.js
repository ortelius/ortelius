/*
 *
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
 */

function ShowHome(hbc,nocallback)
{
	nocallback = nocallback || false;
	console.log("currenttree="+currenttree);

 if (currenttree == "#releases_tree")
  $("#panel_container_historyandpending").css('left', '288px');
 else
  $("#panel_container_historyandpending").css('left', '396px');
 
  $("#panel_container_right").hide();
  $("#panel_container_historyandpending").show();
 
  $("#homependingtab").html("<table id=\"homependingtab\"><tbody></tbody></table>");
  $("#homehisttab").html("<table id=\"homehisttab\"><tbody></tbody></table>");
  
  createNewsFeed("homependingtab","","",true); 
  createNewsFeed("homehisttab","","",false);
  console.log("hbc="+hbc);

  if (hbc)
  {
   $("#panel_container_historyandpending").show();
   $("#panel_container_right_list").hide();
   ActivateHomeSubTabs('tabs-HomeDashboard');
  }
  else
  {
   $("#panel_container_historyandpending").hide();
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