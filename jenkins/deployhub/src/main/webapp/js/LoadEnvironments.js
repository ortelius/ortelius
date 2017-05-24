window.onload=function() {
	var cdiv = document.getElementById("envs");
	var mres = {};
	cdiv.innerHTML = "Loading...";
	backend.loadEnvironments(function(t) {
		var resps = t.responseObject();
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
		var comps = mres.result;
		var td="<table class=\"sortable pane bigtable stripped-odd\">"
		+"<tr class=\"header\"><th><a href=\"#\" class=\"sortheader\">Name<span class=\"sortarrow\"></a></th><th>Owner</th><th>Last Build</th></tr>";
		for (var i=0;i<comps.length;i++) {
			console.log("component "+i+" = "+comps[i].id);
			var owner="N/A";;
			if (typeof comps[i].owneruser != "undefined") {
				owner=comps[i].owneruser;
			} 
			if (typeof comps[i].ownergroup != "undefined") {
				owner = comps[i].ownergroup;
			}
			var lb="N/A";
			if (comps[i].lastbuild>0) lb = "<A href=\"../../job/"+comps[i].project+"/"+comps[i].lastbuild+"\">#"
			 + comps[i].lastbuild + "</A> (Project <A href=\"../../job/"+comps[i].project+"\">"+comps[i].project+"</A>)";;
			var cn = comps[i].domain+"."+comps[i].name;
			td+="<tr><td>"+cn+"</td><td>"+owner+"</td><td>"+lb+"</td></tr>";
		}
		td+="</table>";
		console.log("td="+td);
		cdiv.innerHTML = td;
	});
}

