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

function GetDomainData(currentdomain)
{
	var res=null;
	var seldom = (currentdomain!='') ? ('&seldom='+currentdomain) : '';
	$.ajax({
		  dataType: "json",
		  url: "GetDomainContent?typestr=dmtarget&domainid=0"+seldom,
		  async: false
		}).done(function( data ) {
				console.log("data="+data);
				res=data;
		});
	return res;
}


function dtCreateTree(nodename,currentdomain)
{
	var BaseNode=0;
	var Loading=1;
    var $nodename = $(nodename);
    $nodename.bind(
    	"select_node.jstree", function(evt, data){
    		console.log("Tree -> Select Node");
    		dtSelectNode(data,Loading);
    		Loading=0;
    	}).bind(
    		"loaded.jstree", function () {
    		console.log("tree loaded");
    		if(currentdomain != "") {
    			$nodename.jstree("select_node", "#do"+currentdomain); // .trigger("select_node.jstree");
    		}
    		Loading=0;
    	}).jstree({
    		"json_data" : {
    	  		 "data" : GetDomainData(currentdomain),	// Initial data, including opened nodes to the selected target domain
                 "ajax" : {
                	 "url":	function(node) {
                		 return "GetDomainContent?typestr=dmtarget&domainid="+node.attr("id").substr(2);
                	 }
                 }
    		},
      "types" : {
          // I set both options to -2, as I do not need depth and children count checking
          // Those two checks may slow jstree a lot, so use only when needed
          "max_depth" : -2,
          "max_children" : -2,
          // I want only `domain` nodes to be root nodes 
          // This will prevent moving or creating any other type as a root node
          "valid_children" : [ "Domain" ],
          "types" : {
              // The `domain` node 
              "Domain" : {
                  "icon" : {
                      "image" : "css/images/domain_16x.png"
                  }
              },
              "Lifecycle" : {
                  "icon" : {
                      "image" : "css/images/lifecycle_16x.png"
                  }
              }
          }
      },
      "core" : {"html_titles" : true,  "load_open" : true, "animation": 50 },
      "plugins" : [ "themes", "json_data", "ui", "crrm", "types" ],
      "ui": {"select_limit" : 1}
      });
 
    var $jstree = $.jstree;
}
	