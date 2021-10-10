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
