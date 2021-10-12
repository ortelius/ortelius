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