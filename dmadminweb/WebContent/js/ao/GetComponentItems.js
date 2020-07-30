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
function getComponentItemsBody(id, name, summ, suffix)
{
 var img = "css/images/components_sm.png";
 img = img.replaceAll('-', '_');
 return "<span style=\"position:relative; top:5px;\"><img src=\"" + img + "\" style=\"padding-right:5px\">" + name + "</span>";
}

function getComponentItemsMarkup(id, name, summ, xpos, ypos, suffix)
{
/* if (xpos > 480)
 {
  xpos = 480 - xpos;

  if (xpos < 0)
   xpos = xpos * -1;
 }

 if (ypos < 50)
  ypos += 50;
*/
 return '<div class="drawcomponent componentitembox" id="window' + id + '" ' + 'style="position: absolute; top: ' + ypos + 'px; left: ' + xpos + 'px ">'
   + getComponentItemsBody(id, name, summ, suffix) + '</div>';
}

function SaveComponentItemsDetails(id)
{
 if (DontSave)
  return;

 var url = "f=csv&c=" + id;
 parent.$("#homeatts :input").each(function()
 {
  url = url + "&" + $(this).attr("id") + "=" + $(this).val();
 });
 $.getJSON('UpdateAttrs', url, function(data)
 {
  // Update the fragment window
  console.log("Updating the window for #window" + id);
  $(".componentitembox").each(function(index)
  {
   console.log(index + ": " + $(this).attr("id"));
  });
  $("#window" + id).html(getComponentItemsBody(id, data.title, data.summary, ''));
 });
}

function ciClickElement(id, comptype)
{
 summSaveItemobjid = id;
 
 if (id < 0)
  summSavedomid = 1;

 var addParams = "";
 if (id < 0)
   addParams += "&comptype=" + comptype;
 
 console.log("GetSummaryData?objtype=14&id=" + id + addParams);
 $.ajax(
 {
  url : "GetSummaryData?objtype=14&id=" + id + addParams,
  dataType : 'json',
  async: false,
  type : 'GET',
  success : function(res)
  {
   console.log(res);
   saveRes = res;
   var title = "";
   var tdedit3 = "<form id=\"compitemform\"><div style=\"display:none\">" + id + "</div><table id=\"compitemtab\" class=\"dev_table\"><tbody>";
   var td = "";
   var repolist = [];
   var prefix = "";
   var findprefix = "";
   
   if (comptype == "")
   {
    for (var r = 0; r < res.data.length; r++)
    {
     var row = res.data[r];
     var label = row[3];
     var val = row[4];
     
     if (label.indexOf("Roll Forward") >= 0)
      findprefix = "rf_";
     else  if (label.indexOf("Roll Back") >= 0)
      findprefix = "rb_";
    
     if (label == "Kind")
     {
      if (val == "DATABASE")
        comptype = "database";
      else if (val == "DOCKER")
        comptype = "docker";
      else
        comptype = "file";
     } 
    }
    comptype = findprefix + comptype;
   }
   
   if (comptype == "rf_database")
   {
     tdedit3 = "<form id=\"compitemform\"><div style=\"display:none\">rf_" + id + "</div><table id=\"compitemtab\" class=\"dev_table\"><tbody>";
     prefix = "rf_";
   }  
   if (comptype == "rb_database")
   {
    tdedit3 = "";
    prefix = "rb_";
   } 
   
   for (var r = 0; r < res.data.length; r++)
   {
    var row = res.data[r];
    var field = row[0];
    var callback = row[1];
    var rtype = row[2];
    var label = row[3];
    var val = row[4];
    var isuser = true;
    var oldval = "";

    if (label != "Predecessor" && label != "XPos" && label != "YPos" && label != "Summary" && label != "Name"  && label != "Kind" && label != "Roll Forward" && label != "Rollback")
    {
     var myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";
     
     myid = myid.replace("rollforward","rf_");
     myid = myid.replace("rollback","rb_");
     
     td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
     td += label;
     td += ":</td><td>";
     td += val;
     td += "</tr>";
    }
    
    if (label == "Summary")
    {
    }
    else if (label == "Target Directory")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"target_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"target_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"target_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"target_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Roll Forward Target Directory")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"rf_target_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"rf_target_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"rf_target_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"rf_target_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Roll Back Target Directory")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"rb_target_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"rb_target_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"rb_target_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"rb_target_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Name")
    {
    }
    else if (label == "Build Id")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"buildid_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"buildid_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"buildid_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"buildid_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
     
     var sel = $("#lastbuildnumber_sumrow > td:nth-child(2)").text();
     if (sel == "")
     {
      buildid = val;
      $("#lastbuildnumber_sumrow > td:nth-child(2)").html(val);
     }
    }
    else if (label == "Build URL")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"buildurl_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"buildurl_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"buildurl_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"buildurl_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Helm Chart")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"chart_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"chart_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"chart_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"chart_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Helm Chart Version")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"chartversion_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"chartversion_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"chartversion_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"chartversion_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Helm Chart Namespace")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"chartnamespace_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"chartnamespace_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"chartnamespace_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"chartnamespace_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }    
    else if (label == "Operator")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"operator_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"operator_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"operator_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"operator_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Build Date")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"builddate_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"builddate_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"builddate_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"builddate_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Container Registry")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"dockerrepo_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"dockerrepo_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"dockerrepo_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"dockerrepo_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Container Digest")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"dockersha_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"dockersha_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"dockersha_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"dockersha_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Container Tag")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"dockertag_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"dockertag_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"dockertag_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"dockertag_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Git Commit")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"gitcommit_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"gitcommit_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"gitcommit_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"gitcommit_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Git Repo")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"gitrepo_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"gitrepo_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"gitrepo_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"gitrepo_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Git Tag")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"gittag_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"gittag_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"gittag_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"gittag_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Git URL")
    {
     tdedit3 += "<tr>";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><input name=\"giturl_val\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"giturl_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"giturl_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"giturl_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Roll Forward")
    {
     tdedit3 += "<tr style=\"display:none\">";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"" + prefix + "rollup_val\">";
     if (val == "OFF")
     {
      val = "Off";
      tdedit3 += "<option value=\"OFF\" selected>Off</option>";
      tdedit3 += "<option value=\"ON\">On</option>";
      tdedit3 += "<option value=\"ALL\">All</option>";
     }
     else if (val == "ON")
     {
      val = "On";
      tdedit3 += "<option value=\"OFF\">Off</option>";
      tdedit3 += "<option value=\"ON\" selected>On</option>";
      tdedit3 += "<option value=\"ALL\">All</option>";
     }
     else
     {
      val = "All";
      tdedit3 += "<option value=\"OFF\">Off</option>";
      tdedit3 += "<option value=\"ON\">On</option>";
      tdedit3 += "<option value=\"ALL\" selected>All</option>";
     }
     tdedit3 += "</td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"" + prefix + "rollup_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"" + prefix + "rollup_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"" + prefix + "rollup_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Rollback")
    {
     tdedit3 += "<tr style=\"display:none\">";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td style=\"padding-top:2px;padding-bottom:2px;\"><select name=\"" + prefix + "rollback_val\">";
     if (val == "OFF")
     {
      val = "Off";
      tdedit3 += "<option value=\"OFF\" selected>Off</option>";
      tdedit3 += "<option value=\"ON\">On</option>";
      tdedit3 += "<option value=\"ALL\">All</option>";
     }
     else if (val == "ON")
     {
      val = "On";
      tdedit3 += "<option value=\"OFF\">Off</option>";
      tdedit3 += "<option value=\"ON\" selected>On</option>";
      tdedit3 += "<option value=\"ALL\">All</option>";
     }
     else
     {
      val = "All";
      tdedit3 += "<option value=\"OFF\">Off</option>";
      tdedit3 += "<option value=\"ON\">On</option>";
      tdedit3 += "<option value=\"ALL\" selected>All</option>";
     }
     tdedit3 += "</td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"" + prefix + "rollback_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"" + prefix + "rollback_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"" + prefix + "rollback_oldval\" value=\"" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Repository")
    {
     repolist.push("repository_row");
     tdedit3 += "<tr id=\"repository_row\">";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><select name=\"repository_val\" id=\"repository_val\" onChange=\"ToggleRepoProps(" + id + ",'repository_row',false,true)\" /></td>";
     save_repository_val = val;
     current_repository_val = save_repository_val;
     tdedit3 += "<td><input type=\"hidden\" name=\"repository_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"repository_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"repository_oldval\" value=\"re" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Roll Forward Repository")
    {
     repolist.push("rf_repository_row");
     tdedit3 += "<tr id=\"rf_repository_row\">";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><select name=\"rf_repository_val\" id=\"rf_repository_val\" onChange=\"ToggleRepoProps(" + id + ",'rf_repository_row',false,true)\" /></td>";
     save_rf_repository_val = val;
     current_rf_repository_val = save_rf_repository_val;
     tdedit3 += "<td><input type=\"hidden\" name=\"rf_repository_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"rf_repository_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"rf_repository_oldval\" value=\"re" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
    else if (label == "Roll Back Repository")
    {
     repolist.push("rb_repository_row");
     tdedit3 += "<tr id=\"rb_repository_row\">";
     tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
     tdedit3 += "<td><select name=\"rb_repository_val\" id=\"rb_repository_val\" onChange=\"ToggleRepoProps(" + id + ",'rb_repository_row',false,true)\" /></td>";
     save_rb_repository_val = val;
     current_rb_repository_val = save_rb_repository_val;
     tdedit3 += "<td><input type=\"hidden\" name=\"rb_repository_field\" value=\"" + field + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"rb_repository_callback\" value=\"" + callback + "\"/></td>";
     tdedit3 += "<td><input type=\"hidden\" name=\"rb_repository_oldval\" value=\"re" + val + "\"/></td>";
     tdedit3 += "</tr>";
    }
   }

   if (comptype == "rb_database")
   { 
    var pwd = parent.$("#compitem > tbody:last-child");
    pwd.append(td);
    
    $('#compitemtab > tbody:last-child').append(tdedit3);
    var ids = $("#compitemform > div").text() + ",rb_" + id;
    $("#compitemform > div").html(ids);
   }
   else
   { 
    tdedit3 += "</tbody></table></form>";
 
    var pwd = parent.$("#compitem");
    pwd.empty().append(td);
   
    pwd = parent.$("#compitem_data_edit");
    pwd.empty().append(tdedit3);
   } 
   var myform = pwd.find("#compitemform");
   
   var prefix = "";
   var workid = save_repository_id;
   
   if (comptype == "rf_database")
   {
    prefix = "rf_";
    workid = save_rf_repository_id;
   }
   else if (comptype == "rb_database")
   {
    prefix = "rb_";
    workid = save_rb_repository_id;
   }
    
   if ($("#" + prefix + "repository_val").length > 0)
   {
    $.ajax(
    {
     url : "GetRepositories?objid="+summSaveobjtype+summSaveobjid+"&domid="+summSavedomid,
     dataType : 'json',
     type : 'GET',
     success : function(res)
     {

      // res =
      // $.parseJSON("[{\"type\":\"ac\",\"id\":310,\"name\":\"Action310\",\"showlink\":true},{\"type\":\"ac\",\"id\":311,\"name\":\"Action311\",\"showlink\":true}]");
      var myform = pwd.find("#compitemform");
      var owner = $("#" + prefix + "repository_val");
      
      console.log("save_repository_val=["+save_repository_val+"]");
      var select_val = "";
      
      for (n = 0; n < res.length; n++)
      {
       if (workid == res[n].id)
       {
        owner.append('<option id="' + prefix + 'repository' + n + '" selected value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
        select_val = res[n].type + res[n].id;
       } 
       else
        owner.append('<option id="' + prefix + 'repository' + n + '" value=\"' + res[n].type + res[n].id + "\">" + res[n].name + '</option>');
      }
      
      if (select_val == "" && res.length > 0)
        $("#" +  prefix + "repository_val").val(res[0].type + res[0].id);

      ToggleRepoProps(id,prefix + "repository_row",true);
     },
     error : function(jqxhr, status, err)
     {
      console.log(status);
      console.log(err);
     }
    });
   }
   
   }
  });
}

function SaveSummaryItemData(instance, tablename, objtypeAsInt, objtype, objid, addParams)
{
 var pwd = parent.$("#compitem_data_edit");
 
 if (!pwd.is(":visible"))
  return;
 
 var summ_pwd = parent.$("#summ_data_edit");
 var myform = summ_pwd.find("#summform");
 var kind = myform.find(":input[name=\"kind_oldval\"]").val();
 var id = $("#compitemform > div").text();
 var repolist = id.split(",");
 
 for (k=0;k< repolist.length; k++)
 {
  var compitemid = -1;
  var prefix = "";
  repo = repolist[k];
  
  if (repo.startsWith("rf_") || repo.startsWith("rb_"))
  {
   prefix = repo.substring(0,3);
   id = repo.substring(3);
  } 
  else
  {
   prefix = "";
   id = repo;
  }
  
 if (id < 0)
 {
  if (addcomptype == "docker")
   kind = "ik2";
  else if (addcomptype == "database")
   kind = "ik0";
  else
   kind = "ik1";
 } 
 
 var savedata =
 {
  objtype : 14,
  id : id,
  compid: objid,
  comptype: kind
 };

 var sumret = GetSaveSummaryItemData(instance, savedata, prefix);

 namecbfn = "ChangeSummaryName";

 console.log(savedata);

 if (sumret)
 {
  $.ajax(
  {
   url : "UpdateSummaryData",
   dataType : "json",
   type : "POST",
   async: false,
   data : savedata,
   success : function(data)
   {
    console.log(data);
    if (data && data.saved)
    {
     id = data.id;
     var propret = GetSavePropertyData(instance, savedata, prefix);
     SavePropertyData(id, propret, prefix);

     parent.$("#compitem_data_edit").hide();
    }
    else
    {
     if (data && data.error)
     {
      alert(data.error);
      return;
     }
     else
     {
      alert("Save failed!");
     }
    }
   },
   error : function(jqxhr, status, err)
   {
    console.log(status);
    console.log(err);
    alert(err);
    return;
   }
  });
 }
 else
 {
  var propret = GetSavePropertyData(instance, savedata, prefix);
  SavePropertyData(objid, propret,prefix);

  parent.$("#compitem_data_edit").hide();
 }
 }
 return;
}

function GetSavePropertyData(instance, data, prefix)
{
 ret = false;
 data.nameCallbackReqd = false;
 data.typeCallbackReqd = false;
 data.iconCallbackReqd = false;
 data.newname = null;
 data.newsumm = null;

 var form = $("#compitemform");
 var viewArr = form.serializeArray();
 var view = {};

 for ( var i in viewArr)
 {
  if (viewArr[i].name.indexOf("prop_") >= 0) {
	  console.log("viewArr["+i+"].name="+viewArr[i].value);
	  view[viewArr[i].name] = viewArr[i].value;
  }
 }
 // Add any unchecked checkboxes
 console.log("prop_val_recursive="+view["prop_val_recursive"]);
 if (typeof view["prop_val_recursive"] == "undefined") {
	 console.log("adding prop_val_recursive");
	 view["prop_val_recursive"]="0";
 }

 console.log(view);

 return view;
}

function GetSaveSummaryItemData(instance, data, prefix)
{

 ret = false;
 data.nameCallbackReqd = false;
 data.typeCallbackReqd = false;
 data.iconCallbackReqd = false;
 data.newname = null;
 data.newsumm = null;

 var form = $("#compitemform");
 var viewArr = form.serializeArray();
 var view = {};

 for ( var i in viewArr)
 {
  if (prefix.length > 0 && viewArr[i].name.startsWith(prefix))
   view[viewArr[i].name.substring(3)] = viewArr[i].value; 
  else if (prefix.length == 0 && !viewArr[i].name.startsWith("rf_") && !viewArr[i].name.startsWith("rb_"))
    view[viewArr[i].name] = viewArr[i].value;
 }

 console.log(view);

 var summ_pwd = parent.$("#summ_data_edit");
 var myform = summ_pwd.find("#summform");
 var name = myform.find(":input[name=\"name_val\"]").val();
 view.name_val = name + " " + prefix + "item"; 
 
//if (view.name_val != view.name_oldval)
// {
  data.newname = view.name_val;
  data.nameCallbackReqd = true;
  console.log('change_' + view.name_field + ' = ' + view.name_val);
  data['change_' + view.name_field] = view.name_val;
  ret = true;
// }

 if (view.summary_val != view.summary_oldval)
 {
  console.log('change_' + view.summary_field + ' = ' + view.summary_val);
  data['change_' + view.summary_field] = view.summary_val;
  ret = true;
 }

 if (typeof view.rollup_val !== 'undefined')
 {
   console.log('change_' + view.rollup_field + ' = ' + view.rollup_val);
   data['change_' + view.rollup_field] = view.rollup_val;
   ret = true;
 } 

 if (typeof view.rollback_val !== 'undefined')
 {
   console.log('change_' + view.rollback_field + ' = ' + view.rollback_val);
   data['change_' + view.rollback_field] = view.rollback_val;
   ret = true;
 }
 
 if (typeof view.target_val !== 'undefined' && typeof view.target_oldval !== 'undefined')
 {
  if (view.target_val != view.target_oldval)
  {
   console.log('change_' + view.target_field + ' = ' + view.target_val);
   data['change_' + view.target_field] = view.target_val;
   ret = true;
  }
 } 

 if (typeof view.repository_val !== 'undefined' && typeof view.repository_oldval !== 'undefined')
 {
  if (view.repository_val != view.repository_oldval)
  {
   console.log('change_' + view.repository_field + ' = ' + view.repository_val);
   data['change_' + view.repository_field] = view.repository_val;
   ret = true;
  }
 } 
 
 if (typeof view.buildid_val !== 'undefined' && typeof view.buildid_oldval !== 'undefined')
 {
  if (view.buildid_val != view.buildid_oldval)
  {
   console.log('change_' + view.buildid_field + ' = ' + view.buildid_val);
   data['change_' + view.buildid_field] = view.buildid_val;
   ret = true;
  }
 } 

 if (typeof view.buildurl_val !== 'undefined' && typeof view.buildurl_oldval !== 'undefined')
 {
  if (view.buildurl_val != view.buildurl_oldval)
  {
   console.log('change_' + view.buildurl_field + ' = ' + view.buildurl_val);
   data['change_' + view.buildurl_field] = view.buildurl_val;
   ret = true;
  }
 } 

 if (typeof view.chart_val !== 'undefined' && typeof view.chart_oldval !== 'undefined')
 {
  if (view.chart_val != view.chart_oldval)
  {
   console.log('change_' + view.chart_field + ' = ' + view.chart_val);
   data['change_' + view.chart_field] = view.chart_val;
   ret = true;
  }
 } 

 if (typeof view.chartversion_val !== 'undefined' && typeof view.chartversion_oldval !== 'undefined')
 {
  if (view.chartversion_val != view.chartversion_oldval)
  {
   console.log('change_' + view.chartversion_field + ' = ' + view.chartversion_val);
   data['change_' + view.chartversion_field] = view.chartversion_val;
   ret = true;
  }
 } 

 if (typeof view.chartnamespace_val !== 'undefined' && typeof view.chartnamespace_oldval !== 'undefined')
 {
  if (view.chartnamespace_val != view.chartnamespace_oldval)
  {
   console.log('change_' + view.chartnamespace_field + ' = ' + view.chartnamespace_val);
   data['change_' + view.chartnamespace_field] = view.chartnamespace_val;
   ret = true;
  }
 } 
 
 
 if (typeof view.operator_val !== 'undefined' && typeof view.operator_oldval !== 'undefined')
 {
  if (view.operator_val != view.operator_oldval)
  {
   console.log('change_' + view.operator_field + ' = ' + view.operator_val);
   data['change_' + view.operator_field] = view.operator_val;
   ret = true;
  }
 } 

 if (typeof view.builddate_val !== 'undefined' && typeof view.builddate_oldval !== 'undefined')
 {
  if (view.builddate_val != view.builddate_oldval)
  {
   console.log('change_' + view.builddate_field + ' = ' + view.builddate_val);
   data['change_' + view.builddate_field] = view.builddate_val;
   ret = true;
  }
 } 

 if (typeof view.dockersha_val !== 'undefined' && typeof view.dockersha_oldval !== 'undefined')
 {
  if (view.dockersha_val != view.dockersha_oldval)
  {
   console.log('change_' + view.dockersha_field + ' = ' + view.dockersha_val);
   data['change_' + view.dockersha_field] = view.dockersha_val;
   ret = true;
  }
 } 

 if (typeof view.dockertag_val !== 'undefined' && typeof view.dockertag_oldval !== 'undefined')
 {
  if (view.dockertag_val != view.dockertag_oldval)
  {
   console.log('change_' + view.dockertag_field + ' = ' + view.dockertag_val);
   data['change_' + view.dockertag_field] = view.dockertag_val;
   ret = true;
  }
 } 
 
 if (typeof view.gitcommit_val !== 'undefined' && typeof view.gitcommit_oldval !== 'undefined')
 {
  if (view.gitcommit_val != view.gitcommit_oldval)
  {
   console.log('change_' + view.gitcommit_field + ' = ' + view.gitcommit_val);
   data['change_' + view.gitcommit_field] = view.gitcommit_val;
   ret = true;
  }
 } 

 if (typeof view.gitrepo_val !== 'undefined' && typeof view.gitrepo_oldval !== 'undefined')
 {
  if (view.gitrepo_val != view.gitrepo_oldval)
  {
   console.log('change_' + view.gitrepo_field + ' = ' + view.gitrepo_val);
   data['change_' + view.gitrepo_field] = view.gitrepo_val;
   ret = true;
  }
 } 

 if (typeof view.gittag_val !== 'undefined' && typeof view.gittag_oldval !== 'undefined')
 {
  if (view.gittag_val != view.gittag_oldval)
  {
   console.log('change_' + view.gittag_field + ' = ' + view.gittag_val);
   data['change_' + view.gittag_field] = view.gittag_val;
   ret = true;
  }
 } 

 if (typeof view.giturl_val !== 'undefined' && typeof view.giturl_oldval !== 'undefined')
 {
  if (view.giturl_val != view.giturl_oldval)
  {
   console.log('change_' + view.giturl_field + ' = ' + view.giturl_val);
   data['change_' + view.giturl_field] = view.giturl_val;
   ret = true;
  }
 } 

 return ret;
}

function SavePropertyData(id, view, prefix)
{
	var dor = deleteoldrows;
	console.log("in SavePropertyData, deleteoldrows="+deleteoldrows);
	
	for ( var prop in view)
 {
  var name = "";
  
  if (prefix.length > 0 && !prop.startsWith(prefix))
   continue;
    
  if (prop.indexOf("prop_oldval") >= 0) continue;
  console.log("x) prop="+prop);
  
  if (prop.indexOf("rf_prop_val") >= 0 || prop.indexOf("rb_prop_val") >= 0)
   name = prop.substring(12); 
  else if (prop.indexOf("prop_val") >= 0)
   name = prop.substring(9);

  var val = view[prop];
  if (prop.indexOf("prop_val_recursive") >=0)
  {
   if (val == "No")
    val = 'N';
   else
    val = 'Y';
  }
  
  console.log("x) view["+prop+"] ="+view[prop]);

  prop = prop.replace("_val", "_oldval");

  var oldval = view[prop];

  if (prop.indexOf("prop_oldval_recursive") >= 0)
  {
   if (oldval == "No")
    oldval = 'N';
   else
    oldval = 'Y';
  }
  console.log("x) oldval="+oldval);

  var mask = "NNN";

  if (name.length > 0)
  {
   var key = "";
   var data =
   {
    objtype : 14,
    id : id
   };

   console.log("val="+val+" oldval="+oldval+" dor="+dor);
   key = "";
   if (oldval == "" && val != "")
    key = "prop_add_" + name;
   else if (val == "")
    key = "prop_delete_" + name;
   else if (val != oldval) 
	key = "prop_change_" + name;
   else if (val == oldval && dor)
	key = "prop_add_" + name;
   
   console.log("key="+key);
   
   if (key != "")
   {
    console.log(name + "=" + val + "," + oldval + "," + mask);
    data[key] = val;
    console.log(data);
    console.log("deleteoldrows="+deleteoldrows);
    $.ajax(
    {
     url : "GetComponentItem?ciid=" + id +"&oldrows=" + (deleteoldrows?"Y":"N"),
     type : 'POST',
     dataType : "json",
     data : data,
     async : false,
     success : function(data)
     {
     }
    });
    deleteoldrows = false;	// reset to false after first applied change
   }
  }
 }
}

function ToggleRepoProps(objid,t, init,selchanged)
{
	var mrp=""; // <tr class=\"repoprops\"><td colspan=\"2\"><I>Modifiable Repository Properties:</I></td></tr>";
	var mrp2="";
	console.log("objid="+objid+" t="+t+" init="+init+" selchanged="+selchanged);
 var field = "#" + t;
 var trrow = "#" + t;
 var prefix = "";
 var td = "";
 var tdedit3 = "";
 
 if ($(field).length == 0)
  return;
 
 field = field.replace("_row","_val");
 if (t.startsWith("rf_"))
  prefix = "rf_";
 else  if (t.startsWith("rb_"))
  prefix = "rb_"; 
  
 current_repository_val = $(field).val();
 console.log("REPO=" + current_repository_val);
 
 if (selchanged)
  deleteoldrows = (current_repository_val != save_repository_val);
 else
  deleteoldrows = false;
 
 console.log("deleteoldrows="+deleteoldrows);

 if (objid < 0)
  olditemvalues = [];
 
 var repoid = (current_repository_val!=null)?$(field).val().substring(2):0;

 
 if (deleteoldrows)
  $("#compitemtab > tbody > tr.repoprops").remove();
// if (init)
// { 
//  $(field).find("tr.repoprops").remove();
//  $(field).find("tr.rf_repoprops").remove();
//  $(field).find("tr.rb_repoprops").remove();
//  init = false;
// }  
  console.log("GetComponentItemSourceData?ciid=" + objid + "&reason=defs&repid=" + repoid);
  
//  $.ajax(
//    {
//     url : "GetComponentItemSourceData?ciid=" + objid + "&reason=defs&repid=" + repoid,
//     dataType : 'json',
//     type : 'GET',
//     async : false,
//     success : function(res)
//     {
//      console.log(res);
//      olditemvalues = [];
//      
//      saveRes = res.defs;
//      
//      tdedit3 += mrp;
//      for (var p = 0; p < res.defs.length; p++)
//      {
//       var label = res.defs[p].name;
//       var val = olditemvalues[label];
//       var field = "";
//       var callback = "";
//
//       if (typeof val == "undefined") val = "";
//       
//       console.log("a) val="+val+" label="+label);
//       
//       tdedit3 += "<tr class=\"" + prefix + "repoprops\">";
//       tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + label + ":</td>";
//       if (label == "recursive" || label == "UseSSL") {
//    	   var checked = val==1?"checked":"";
//    	   tdedit3 += "<td><input name=\"" + prefix + "prop_val_" + label + "\" id=\"prop_val_" + label + "\" style='width:100%' type=\"checkbox\" value=\"1\" " + checked + "/></td>";   
//       } else {
//    	   tdedit3 += "<td><input name=\"" + prefix + "prop_val_" + label + "\" id=\"prop_val_" + label + "\" style='width:100%' type=\"text\" value=\"" + val + "\"/></td>";
//       }
//       tdedit3 += "<td><input type=\"hidden\" name=\"" + prefix + "prop_oldval_" + label + "\" id=\"prop_oldval_" + label + "\" value=\"" + val + "\"/></td>";
//       tdedit3 += "</tr>";
//      }
//      tdedit3+=mrp2;
//      $(trrow).after(tdedit3);
//      $("#repository_sumrow").after(td);
//     }
//    });
// }
// else
// { 
//  $(field).find("tr." + prefix + "repoprops").remove();
 
  if (!deleteoldrows && objid >= 0)
  { 
   $.ajax(
   {
    url : "GetComponentItemSourceData?ciid=" + objid + "&repid=" + repoid,
    dataType : 'json',
    type : 'GET',
    async : false,
    success : function(res)
    {
     console.log(res);
     olditemvalues = [];
     

     for (var p = 0; p < res.data.length; p++)
     {
      var row = res.data[p];
      if (row[3] != "Repository") {
    	  console.log("Setting values["+row[3]+"] to "+row[4]);
    	  olditemvalues[row[3]] = row[4];
      }
     }

     console.log(olditemvalues);
     tdedit3 += mrp;
     for (var p = 0; p < res.defs.length; p++)
     {
      var label = res.defs[p].name;
      var display_label = label.charAt(0).toUpperCase() + label.slice(1) + " Override";
      display_label = display_label.replace("URI","URL");
    
      var val = olditemvalues[label];
      var field = "";
      var callback = "";
      
      if (typeof val == "undefined") val = "";
      
      console.log("b) val="+val);
      
      tdedit3 += "<tr class=\"" + prefix + "repoprops\">";
      tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + display_label + ":</td>";
      if (label == "recursive" || label == "UseSSL") {
    	 var checked = (val==1 || val == "Y")?"checked":"";
    	 tdedit3 += "<td><input name=\"" + prefix + "prop_val_" + label + "\" id=\"prop_val_" + label + "\" type=\"checkbox\" value=\"1\"" + checked + "/></td>";
    	 val = (checked == "checked")?"Yes":"No";
      } else {
     	 tdedit3 += "<td><input name=\"" + prefix + "prop_val_" + label + "\" id=\"prop_val_" + label + "\" type=\"text\" value=\"" + val + "\"/></td>";
      }
      
      tdedit3 += "<td><input type=\"hidden\" name=\"" + prefix + "prop_oldval_" + label + "\" id=\"prop_oldval_" + label + "\" value=\"" + val + "\" /></td>";
      tdedit3 += "</tr>";
      
      if (label != "Predecessor" && label != "XPos" && label != "YPos" && label != "Summary" && label != "Name")
      {
       var myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";
       td += "<tr id=\"" + myid + "\" ><td class=\"summlabel repoindent\">";
       td += display_label;
       td += ":</td><td>";
       td += val;
       td += "</tr>";
      }
     }
    }
   });
  }
  
    if (tdedit3 == "")
    {
    	console.log("GetComponentItemSourceData?ciid=" + objid + "&reason=defs&repid=" + repoid);
    $.ajax(
      {
       url : "GetComponentItemSourceData?ciid=" + objid + "&reason=defs&repid=" + repoid,
       dataType : 'json',
       type : 'GET',
       async : false,
       success : function(res)
       {
        console.log(res);
        
        saveRes = res.defs;
        
        tdedit3 += mrp;
        
        for (var p = 0; p < res.defs.length; p++)
        {
         var label = res.defs[p].name;
         var val = olditemvalues[label];
         var field = "";
         var callback = "";
         var display_label = label.charAt(0).toUpperCase() + label.slice(1) + " Override";
         display_label = display_label.replace("URI","URL");

         if (typeof val == "undefined") val = "";
         console.log("3) label="+label+" val="+val);
         
         tdedit3 += "<tr class=\"" + prefix + "repoprops\">";
         tdedit3 += "<td style=\"text-align:left; white-space: nowrap;\">" + display_label + ":</td>";
         if (label == "recursive" || label == "UseSSL") {
          var checked = (val==1 || val == "Y")?"checked":"";
        	 tdedit3 += "<td><input name=\"" + prefix + "prop_val_" + label + "\" id=\"prop_val_" + label + "\" type=\"checkbox\" value=\"1\" " + checked + "/></td>";
        	 val = (checked == "checked")?"Yes":"No";
         } else {
        	 tdedit3 += "<td><input name=\"" + prefix + "prop_val_" + label + "\" id=\"prop_val_" + label + "\" type=\"text\" value=\"" + val + "\"/></td>";
         }
         tdedit3 += "<td><input type=\"hidden\" name=\"" + prefix + "prop_oldval_" + label + "\" id=\"prop_oldval_" + label + "\" value=\"" + val + "\"/></td>";
         tdedit3 += "</tr>";
         
         if (label != "Predecessor" && label != "XPos" && label != "YPos" && label != "Summary" && label != "Name")
         {
          var myid = label.toLocaleLowerCase().replace(/ /g, "") + "_sumrow";
          td += "<tr id=\"" + myid + "\" ><td class=\"summlabel\">";
          td += display_label;
          td += ":</td><td>";
          td += val;
          td += "</tr>";
         }
        }
       }
      }); 
    }
    tdedit3+=mrp2;
    $("#" + prefix + "repository_row").after(tdedit3);
    $("#" + prefix + "repository_sumrow").after(td);
// } 
}
