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