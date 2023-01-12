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
