/*
 *
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
var ioc=0;
 

$(document).ready(function()
{
 if (licType == "OSS")
  return;
});

function hookSummary(tablename, objtypeAsInt, objtype, objid, addParams)
{
}

function hookList(listName)
{
}

function hookTabs(currenttree)
{
}

function hookActivateSubTabs(classname)
{
 return classname;
}

function hookTabId(tabId, classname)
{
}

function hookHide()
{
 
}

function hookAddRow(type)
{
}

function hookEventOpenRow(frameid)
{
 var tree = "#" + frameid + "_tree";
 
 return tree;
}

function hookTaskMenu(currenttree)
{
}

function hookDelRow(type)
{
 var data = null;
 
 return data;
}
