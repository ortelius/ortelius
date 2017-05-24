window.onload=function() {
	var adiv = document.getElementById("app");
	var mres = {};
	adiv.innerHTML = "Loading...";
	backend.loadApplication(function(t) {
		var resps = t.responseObject();
		console.log(resps);
		for (var t=0;t<resps.length;t++) {
			console.log("resps["+t+"]="+resps[t]);
			var project = resps[t];
			console.log("project="+project);
			t++;
			console.log("resps["+t+"]="+resps[t]);
			var d = JSON.parse(resps[t]);
			if (d.success) {
				for (var q=0;q<d.result.length;q++) d.result[q].project = project;
				mres = mergeJSON(mres,d);
			} else {
				console.log(d.error);
			}
		}
		console.log("mres = ");
		console.log(mres);
		var d = mres.result;
		var td="<table class=\"stripped-odd\"><tr><td><i>Name:</i></td><td>"+d.name+"</td></tr>";
		td+="<tr><td><i>Summary:</i></td><td>"+d.summary+"</td></tr>";
		td+="<tr><td><i>Domain:</i></td><td>"+d.domain+"</td></tr>";
		var owner="N/A";;
		if (typeof d.owneruser != "undefined") {
			owner=d.owneruser;
		} 
		if (typeof d.ownergroup != "undefined") {
			owner = d.ownergroup;
		}
		td+="<tr><td><i>Owner:</i></td><td>"+owner+"</td></tr>";
		td+="</table>";
		td+="<h2>Components</h2>";
		td+="The following DeployHub Components make up this Application. Components that are associated with Projects in this Jenkins Instance are ";
		td+="hyperlinked so you can navigate to them.";
		td+="<table class=\"sortable pane bigtable stripped-odd\">"
		+"<tr class=\"header\"><th><a href=\"#\" class=\"sortheader\">Name<span class=\"sortarrow\"></a></th><th>Owner</th><th>Last Build</th></tr>";
		var comps = d.components;
		for (var i=0;i<comps.length;i++) {
			console.log("component "+i+" = "+comps[i].id);
			console.log(comps[i]);
			var owner="N/A";;
			if (typeof comps[i].owneruser != "undefined") {
				owner=comps[i].owneruser;
			} 
			if (typeof comps[i].ownergroup != "undefined") {
				owner = comps[i].ownergroup;
			}
			var lb="N/A";
			var cn="";
			var curl="";
			if (typeof comps[i].serverurl != "undefined") {
				curl = comps[i].serverurl.endsWith("/")?comps[i].serverurl:comps[i].serverurl+"/";
			}
			console.log("curl="+curl+" jenkinsurl="+jenkinsurl);
			if (curl == jenkinsurl) {
				if (comps[i].lastbuild>0) lb = "<A href=\"../../../job/"+comps[i].project+"/"+comps[i].lastbuild+"\">#"
				 + comps[i].lastbuild + "</A> (Project <A href=\"../../../job/"+comps[i].project+"\">"+comps[i].project+"</A>)";;
				cn = "<a href=\"../../Components/"+comps[i].domain+"."+comps[i].name+"\">"+comps[i].domain+"."+comps[i].name+"</a>";
			} else {
				cn = comps[i].domain+"."+comps[i].name;
			}
			td+="<tr><td>"+cn+"</td><td>"+owner+"</td><td>"+lb+"</td></tr>";
		}
		td+="</table>";
		
		console.log("td="+td);
		adiv.innerHTML = td;
	});
}

