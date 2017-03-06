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

function LoadUserMembershipData(tablename, objtypeAsInt, objtype, objid, addParams)
{
 summSavetablename = tablename;
 summSaveobjtypeAsInt = objtypeAsInt;
 summSaveobjtype = objtype;
 summSaveobjid = objid;
 summSaveaddParams = addParams;

 $.getJSON('GroupDetails', "gid=" + objid + "&type=Users4Group", function(data)
 {
	 console.log("data");
	 console.log(data);
	 if (data.readonly) {
		 $("#title_icon").hide();
	 } else {
		 $("#title_icon").show();	 
	 }
  var td = "";
  var rowcnt = 0;
  
  
  for (a = 0; a < data.users.length; a++)
  {
   var linkval="<a href=\"javascript:SwitchDisplay('us"+data.users[a].id+"');\">"+data.users[a].name+"</a>";
   td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\"  id=\"usr" + data.users[a].id  +"\">" + linkval + "</td></tr>";
   rowcnt++;
  }

  for (rowcnt; rowcnt < 5; rowcnt++)
  {
   td += "<tr class=\"clickablerow\"><td style=\"border: 1px solid #ddd;\">&nbsp;</td></tr>";
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

function AddUser2GroupButton()
{
 var pwd = parent.$("#modal");
 var buttons = [
                {
                 text : "Ok",
                 click : function()
                 {
                  SaveUserMembersData(parent.$("#modal"),false);
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

 var tdedit = "<form id=\"UserMembersform\"><table id=\"UserMemberstab\" class=\"summtable\">";
 tdedit += "<tr><td style=\"text-align:left;\">Available Users:</td></tr>";
 tdedit += "<tr><td><select multiple id=\"user_list\" style=\"width:250px\" name=\"users_val\" size=10 /></td></tr>";
 tdedit += "</table></form>";

 pwd.empty();
 pwd.html(tdedit);

 pwd.dialog(
   {
    open : function(event, ui)
    {
     $.ajax(
       {
        url : "GroupDetails?gid=" + summSaveobjid + "&type=AvailUsers4Group",
        dataType : 'json',
        type : 'GET',
        success : function(res)
        {
         var myform = pwd.find("#UserMembersform");
         var alloc = myform.find(":input[name=\"users_val\"]");

         for (n = 0; n < res.length; n++)
          alloc.append('<option id="usr' + n + '" value="usr' + res[n].id + "\">" + res[n].name + '</option>');
        },
        error : function(jqxhr, status, err)
        {
         console.log(status);
         console.log(err);
        }
       });          
      }
   });

     pwd.dialog("option", "title", "Add User");
     pwd.dialog("option", "height", "auto");
     pwd.dialog("option", "width", "280px");
     pwd.dialog("option", "buttons", buttons);
     pwd.dialog('open');
    
   }


   function DeleteUserFromGroupButton()
   {
    $("tr.ui-state-highlight").each(function(i, tr) {       
     var key = $(this).find("td").eq(0).attr("id");
     key = key.substring(3);

     saveRes = $.ajax(
         {
          url: "GroupDetails?gid=" + summSaveobjid + "&id="+key + "&f=r",
          type: 'POST',
          dataType: "json"
         });
    });  
    
    saveRes.success(function(data){
     LoadUserMembershipData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveaddParams);
    });
    
    saveRes.fail(function( jqXHR, textStatus ) {
     alert( "Request failed: " + textStatus );
   });
   }

   function SaveUserMembersData(instance, tablename, objtypeAsInt, objtype, objid, addParams)
   {
    var form = instance.find("#UserMembersform");
    var viewArr = form.serializeArray();
    var view = {};

    console.log(viewArr);
    
    for ( var i in viewArr)
    {
     if (view[viewArr[i].name] === undefined)
      view[viewArr[i].name] = viewArr[i].value + ";";
     else
      view[viewArr[i].name] = view[viewArr[i].name] + viewArr[i].value  + ";";      
    }
    
    var srvid = "";
    
    srvid = view['users_val']; 
    
    var srvlist = srvid.split(";");
    var len = srvlist.length;
    for (var i = 0; i < len; i++) 
    {
     if (srvlist[i].length == 0)
      continue;
     
     srvid = srvlist[i].substring(3);
     console.log("GroupDetails?id="+ srvid + "&gid=" + summSaveobjid  + "&f=a");
     
     $.ajax(
      {
       url: "GroupDetails?id="+ srvid + "&gid=" + summSaveobjid + "&f=a",
       type: 'POST',
       dataType: "json",
       async: false,
       success: function(data)
       {
       }
      });
    }
    LoadUserMembershipData(summSavetablename,summSaveobjtypeAsInt,summSaveobjtype,summSaveobjid,summSaveaddParams);

   }
