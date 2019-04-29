 <%--
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
 --%>
<!DOCTYPE html>
<html lang="en">
<head>
    <title id="Description">File Audit Trace</title> 
    <link rel="stylesheet" href="https://cdn.datatables.net/1.10.19/css/jquery.dataTables.min.css" type="text/css" />
    <link rel="stylesheet" href="https://cdn.datatables.net/buttons/1.5.6/css/buttons.dataTables.min.css" />
    <link rel="stylesheet" type="text/css" href="css/devops_frydesign.css" />  

    <script type="text/javascript" src="js/jquery.min.js"></script>
    <script type="text/javascript" src="https://cdn.datatables.net/1.10.19/js/jquery.dataTables.min.js"></script>
    <script type="text/javascript" src="https://cdn.datatables.net/buttons/1.5.6/js/dataTables.buttons.min.js"></script>
    <script type="text/javascript" src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.flash.min.js"></script>
    <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jszip/3.1.3/jszip.min.js"></script>
    <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/pdfmake/0.1.53/pdfmake.min.js"></script>
    <script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/pdfmake/0.1.53/vfs_fonts.js"></script>
    <script type="text/javascript" src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.html5.min.js"></script>
    <script type="text/javascript" src="https://cdn.datatables.net/buttons/1.5.6/js/buttons.print.min.js"></script>
    <script type="text/javascript">
    
        $(document).ready(function () {
              $('#reports').DataTable( {
               dom: 'lftiprB',
               "ajax": {
                "url": "/dmadminweb/ReportsData?type=FileAudit&md5=${param.md5}",
                "type": "GET"
              },
             
             "columns": [
                         {"data": "deployment"},
                         {"data": "deployedat"},
                         {"data": "username"},
                         {"data": "application"},
                         {"data": "environment"},
                         {"data": "component"},
                         {"data": "server"},
                         {"data": "file"},
                        ],
              buttons: [
                        'copy', 'csv', 'excel', 'pdf', 'print'
                       ]           
         } );
        });

    </script>
</head>
<body class='default'>
     <div style="text-align:center;"><H2 class="reports">File Audit Trace</H2><p></div>
     <div id="dataTable"></div>
       <div style='margin-top: 20px;'>
        <div style='float: left;'>
		<table id="reports" class="display" style="width:100%">
        <thead>
            <tr>
				<th>Deployment</th>     
				<th>Deployed On</th>                          
				<th>User</th>
				<th>Application</th> 
				<th>Environment</th> 	
				<th>Component</th> 
				<th>Server</th> 	
				<th>File</th> 																					
            </tr>
        </thead>
        <tfoot>
            <tr>
				<th>Deployment</th>     
				<th>Deployed On</th>                          
				<th>User</th>
				<th>Application</th> 
				<th>Environment</th> 	
				<th>Component</th> 
				<th>Server</th> 	
				<th>File</th> 												
            </tr>
        </tfoot>
        </table>         
        </div>
    </div>
</body>
</html>
