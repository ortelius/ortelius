 <%--
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
 --%>
<!DOCTYPE html>
<html lang="en">
<head>
    <title id="Description">File Audit Trace</title>
    <link rel="stylesheet" href="https://cdn.datatables.net/1.10.19/css/jquery.dataTables.min.css" type="text/css" />
    <link rel="stylesheet" href="https://cdn.datatables.net/buttons/1.5.6/css/buttons.dataTables.min.css" />
    <link rel="stylesheet" type="text/css" href="css/devops_frydesign.css" />

	<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.7.0/jquery.min.js" integrity="sha512-3gJwYpMe3QewGELv8k/BX9vcqhryRdzRMxVfq6ngyWXwo03GFEzjsUm8Q7RZcHPHksttq7/GFoxjCVUjkjvPdw==" crossorigin="anonymous" referrerpolicy="no-referrer"></script>
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
