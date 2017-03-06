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

function LoadSrv4CompData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;
 var url = "";
 var servlet = "";
 var duplist = new Array();
 
 servlet = "GetComponentLayout";
 url = "compid=" + objid + "&t=s";
 
 $.getJSON(servlet, url, function(res)
   {
  var td = "";
  var rowcnt = 0;

   
    data = res.data;
   var displayrows = {};
   
   for (a = 0; a < data.length; a++)
   {
    var row = data[a];
    
    console.log("row");

    
    console.log(row);
    
    if (typeof row[0].type == "undefined")
     continue;
    
    var val = row[3].name;
    if (typeof row[3].when != "undefined")
    {
     var d = convertDate(row[3].when);
     val = d.toLocaleDateString() + " " + d.toLocaleTimeString();
     val += " by " + row[3].name;
    } else {
    	var builddesc=(row[4]>0)?"(Build <a onClick=\"javascript:DisplayBuild("+objid+","+row[4]+",true);\">#"+row[4]+"</a>)":"";
    	val = "<a onClick=\"DisplayDeploy(" + row[3].id + ");\">"+val+"</a> "+builddesc+" on "+row[3].tooltip;
    }
    
    // else val="";
    
    linkval1="<a href='javascript:SwitchDisplay(\""+row[0].type+row[0].id+"\");'>"+row[0].name+"</a>";
    linkval2="<a href='javascript:SwitchDisplay(\""+row[1].type+row[1].id+"\");'>"+row[1].name+"</a>";
    linkval3="<a href='javascript:SwitchDisplay(\""+row[2].type+row[2].id+"\");'>"+row[2].name+"</a>";
    
    var key = row[0].name + row[1].name;
    
    if (displayrows[key] === undefined)
    {
     var a_row = new Object();
     a_row.val = row[2].name + val;
     // a_row.html = "<tr class=\"clickablerow\" \"><td style=\"border: 1px solid #ddd;\"  id=\"" + row[0].type + row[0].id  +"\">" + row[0].name + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[1].type + row[1].id  +"\">" + row[1].name + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[2].type + row[2].id  +"\">" + row[2].name  + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[3].type + row[3].id  +"\">" + val  + "</td></tr>";
     a_row.html = "<tr class=\"clickablerow\" \"><td style=\"border: 1px solid #ddd;\"  id=\"" + row[0].type + row[0].id  +"\">" + linkval1 + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[1].type + row[1].id  +"\">" + linkval2 + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[2].type + row[2].id  +"\">" + linkval3  + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[3].type + row[3].id  +"\">" + val  + "</td></tr>";
     displayrows[key] = a_row;
    }
    else
    {
     var oldval = displayrows[key].val;
     
     if (oldval == "")
     {
      var a_row = new Object();
      a_row.val = row[2].name + val;
      //a_row.html = "<tr class=\"clickablerow\" \"><td style=\"border: 1px solid #ddd;\"  id=\"" + row[0].type + row[0].id  +"\">" + row[0].name + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[1].type + row[1].id  +"\">" + row[1].name + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[2].type + row[2].id  +"\">" + row[2].name  + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[3].type + row[3].id  +"\">" + val  + "</td></tr>";
      a_row.html = "<tr class=\"clickablerow\" \"><td style=\"border: 1px solid #ddd;\"  id=\"" + row[0].type + row[0].id  +"\">" + linkval1 + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[1].type + row[1].id  +"\">" + linkval2 + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[2].type + row[2].id  +"\">" + linkval3  + "</td><td style=\"border: 1px solid #ddd;\" id=\"" + row[3].type + row[3].id  +"\">" + val  + "</td></tr>";
      displayrows[key] = a_row;
     }
    }
   }
  
  var sorted_keys = Object.keys(displayrows).sort();
   
  for (var x in sorted_keys) 
  {
   var key = sorted_keys[x];
   td += displayrows[key].html;
   rowcnt++;   
  }
  
  for (rowcnt; rowcnt < 5; rowcnt++)
  {
   td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td><td style=\"border: 1px solid #ddd;\">&nbsp;</td></tr>";
  }

  $("#" + tablename + " > tbody").html(td);

  var $table = $("#" + tablename);
  $table.delegate("tr", "click", function()
    {
   $table.find("tr.ui-state-highlight").removeClass('ui-state-highlight');
   $(this).addClass('ui-state-highlight');

   $("edit_button").button("option", "disabled", false);
   $("delete_button").button("option", "disabled", false);
    });

  $("add_button").button();
  $("delete_button").button(
    {
     disabled : true
    });

  $("edit_button").button(
    {
     disabled : true
    });
   });
}

function AddSrv4CompButton(isVersion)
{
 var pwd = parent.$("#modal");
 console.log("AddSrv4CompButton isVersion="+isVersion);
 var buttons = [
                {
                 text : "Ok",
                 click : function()
                 {
                  SaveSrv4CompData(parent.$("#modal"),isVersion);
                  parent.$("#modal").dialog("close");
                 }
                },
                {
                 text : "Cancel",
                 click : function()
                 {
                  parent.$("#modal").dialog("close");
                 }
                } ];

 var tdedit = "<form id=\"srv4compform\"><table id=\"srv4comptab\" class=\"summtable\">";
 if (isVersion) {
	 tdedit += "<tr><td style=\"text-align:left;\"><I>Manually add this Component Version to selected end point(s). Use this if the end point has had the component version installed outside of Release Engineer.</td></tr>";
 } else {
	 tdedit += "<tr><td style=\"text-align:left;\"><I>Associate this Component with selected end point(s). This tells Release Engineer which end point(s) will receive versions of this component during a deployment.</td></tr>";
 }
 tdedit += "<tr><td style=\"text-align:left;\"><B>End Point:</B></td></tr>";
 tdedit += "<tr><td><div id=\"allocated\" name=\"allocated_val\" class=\"om_tree_selection\"/></div>";
 tdedit += "</td></tr>";
 tdedit += "</table></form>";

 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
   {
    open : function(event, ui)
    {
     CreateTree_Selection("#allocated","servers",false,"mainframe3");
    }
   });

 	if (isVersion) {
 		pwd.dialog("option", "title", "Manually Add Component Version");
 	} else {
 		pwd.dialog("option", "title", "Add End Point Association");
 	}
     pwd.dialog("option", "height", "auto");
     pwd.dialog("option", "width", "500");
     pwd.dialog("option", "buttons", buttons);
     pwd.dialog('open');
    
   }

   function DeleteSrv4CompButton()
   {
    $("tr.ui-state-highlight").each(function(i, tr) {       
     var key = $(this).find("td").eq(1).attr("id");
     key = key.substring(2);

     saveRes = $.ajax(
         {
          url: "UpdateAttrs?f=scd&c=" + summSaveobjid + "&s="+key,
          type: 'GET',
          dataType: "text"
         });
    });  
    
    saveRes.success(function(data){
     LoadSrv4CompData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveaddParams);
    });
    
    saveRes.fail(function( jqXHR, textStatus ) {
     alert( "Request failed: " + textStatus );
   });
   }

   function SaveSrv4CompData(instance, isVersion)
   {
	   console.log("SaveSrv4CompData isVersion="+isVersion);
    var form = instance.find("#srv4compform");

    var srvid = "";
    
    var srvlist = [];
    $('#allocated').jstree("get_checked",null,true).each(function(){
     if (this.id.indexOf("se") == 0)
        srvlist.push(this.id);
    });

    var len = srvlist.length;
    for (var i = 0; i < len; i++) 
    {
     if (srvlist[i].length == 0)
      continue;
     
     srvid = srvlist[i].substring(2);
     
     if (isVersion)
      url = "UpdateAttrs?f=scam&c="+ summSaveobjid + "&s=" + srvid + "&xpos=20&ypos=20";
     else
      url = "UpdateAttrs?f=scvm&c="+ summSaveobjid + "&s=" + srvid + "&xpos=20&ypos=20";
     
     console.log(url);
     
     $.ajax(
      {
       url: url,
       type: 'GET',
       dataType: "text",
       async: false,
       success: function(data)
       {
       }
      });
    }
    LoadSrv4CompData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveaddParams);

   }
     
   function LoadBuildDataForComponent(divname, objtypeAsInt, objtype, objid, addParams)
   {
	var url = "";
	var servlet = "";
	var duplist = new Array();
	
	servlet = "GetComponentLayout";
	url = "compid=" + objid + "&t=b";
	
	$.getJSON(servlet, url, function(res) {
		console.log("Build Info:");
		console.log(res);
		data = res.data;
		console.log("data.length is "+data.length);
		var headerPrinted=false;
		var td="";
		for (a = 0; a < data.length; a++) {
		    var row = data[a];
		    console.log(row);
		    if (typeof row[0].id == "undefined") continue;
		    console.log("row[0].id="+row[0].id);
		    console.log("row[0].name="+row[0].name);
		    console.log("row[1]="+row[1]);
		    console.log("row[2]="+row[2]);
		    linkval1="<a href='javascript:SwitchDisplay(\""+row[1].type+row[1].id+"\");'>"+row[1].name+"</a>";
		    linkval2="<a href='javascript:SwitchDisplay(\""+row[0].type+row[0].id+"\");'>"+row[0].name+"</a>";
		    if (!headerPrinted) {
		    	td+="<h3>Builds:</h3>";
		    	td+="<table cellspacing=\"0\" width=\"100%\">";
		    	td+="<thead class=\"ui-widget-header\">";
		    	td+="<tr><th align=\"left\" style=\"width:100px;\">Build Number</th><th align=\"left\" style=\"width:150px\">Date/Time</th><th align=\"left\">Component</th><th>Build Job</th></tr>";
		    	td+="</thead>";	   
		    	td+="<tbody>";
		    	headerPrinted=true;
		    }
		    console.log("row[4]="+row[4]);
		    var icon="";
		    var success;
		    if (row[4]=="Y") {
		    	icon="<img src=\"images/blue_small.png\" style=\"vertical-align:middle;\">&nbsp;";
		    	success=true;
		    } else {
		    	icon="<img src=\"images/red_small.png\" style=\"vertical-align:middle;\">&nbsp;";
		    	success=false;
		    }
		    
		    td+="<tr><td style=\"border: 1px solid #ddd;\">"+icon+"<a onClick=\"javascript:DisplayBuild("+objid+","+row[2]+","+success+");\">#"+row[2]+"</a></td>";
		    td+="<td style=\"border: 1px solid #ddd;\">"+row[3]+"</td>";
		    td+="<td style=\"border: 1px solid #ddd;\">"+linkval1+"</td>";
		    td+="<td style=\"border: 1px solid #ddd;\">"+linkval2+"</td></tr>";
		}
		if (headerPrinted) {
			td+"</tbody></table>";
		}
		$("#"+divname).html(td);
	});
  }
