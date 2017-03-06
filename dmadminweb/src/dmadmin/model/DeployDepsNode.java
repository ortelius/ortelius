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
package dmadmin.model;

import dmadmin.json.IJSONSerializable;
import dmadmin.json.JSONObject;


public class DeployDepsNode
{
	private String id;
	private String label;
	private int level;
	private String color;
	
	public DeployDepsNode(String id,String label) {
	 this.id=id;
  this.label=label;
	 
  if (id.startsWith("bd"))
  {
   this.level=0;
   this.color="#C1DAD6";
  }
  else if (id.startsWith("sr"))
	 {
	  this.level=1;
	  this.color="#9C9F84";
	 }
	 else if (id.startsWith("cm"))
	 {
   this.level=2;
   this.color="#DEE79C";
	 }
  else if (id.startsWith("bj"))
  {
   this.level=3;
   this.color="#E7C39C";
  }
  else if (id.startsWith("df"))
  {
   this.level=4;
   this.color="#C1DAD6";
  }
  else if (id.startsWith("cv") || id.startsWith("co"))
  {
   this.level=5;
   this.color="#F1A94E";
  }
  else if (id.startsWith("av") || id.startsWith("ap"))
  {
   this.level=6;
   this.color="#DF9496";
  }
  else if (id.startsWith("en"))
  {
   this.level=7;
   this.color="#7B8D8E";
  }
  else if (id.startsWith("se"))
  {
   this.level=8;
   this.color="#ACD1E9";
  }
	}
	
	public IJSONSerializable getJSON() {
		JSONObject dep = new JSONObject();
		dep.add("id",id);
		dep.add("label",label);
		dep.add("level",level);
		dep.add("color",color);
		return dep;
	}

}
