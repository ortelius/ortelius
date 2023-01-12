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


public class DeployDepsEdge
{
	private String from;
	private String to;
	private boolean arrow;


	public DeployDepsEdge(String from, String to) {
	 this.from=from;
	 this.to=to;
	 this.arrow = false;
	}

 public DeployDepsEdge(String from, String to,boolean arrow) {
  this.from=from;
  this.to=to;
  this.arrow = arrow;
 }

	public IJSONSerializable getJSON() {
		JSONObject dep = new JSONObject();
		dep.add("from",from);
		dep.add("to",to);
		if (arrow)
		 dep.add("dashes", true);
		return dep;
	}

}
