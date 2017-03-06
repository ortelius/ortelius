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
    <title id="Description">Success/Failed Deployments per Environment Report</title>
    <link rel="stylesheet" href="js/jqwidgets/styles/jqx.base.css" type="text/css" />
    <link rel="stylesheet" href="js/jqwidgets/styles/jqx.energyblue.css" type="text/css" />
    <link rel="stylesheet" type="text/css" href="css/devops_frydesign.css?v=${initParam.BUILD_VERSION}" />    
    <script type="text/javascript" src="js/jquery.min.js"></script>
    <script type="text/javascript" src="js/jqwidgets/jqxcore.js"></script>
    <script type="text/javascript" src="js/jqwidgets/jqxbuttons.js"></script>
    <script type="text/javascript" src="js/jqwidgets/jqxscrollbar.js"></script>
    <script type="text/javascript" src="js/jqwidgets/jqxdata.js"></script> 
    <script type="text/javascript" src="js/jqwidgets/jqxdatatable.js"></script> 
    <script type="text/javascript" src="js/jqwidgets/jqxdata.export.js"></script>
    <script type="text/javascript">

     $(document).ready(function () {
           var url = "/dmadminweb/ReportsData?type=FileAudit&md5=${param.md5}";

            // prepare the data
            var source =
            {
                dataType: "json",
                dataFields: [
                             {name: "deployment", type: "int"},
                             {name: "deployedat", type: "string"},
                             {name: "username", type: 'string' },
                             {name: "application", type: 'string' },
                             {name: "environment", type: 'string' },     
                             {name: "component", type: 'string' },
                             {name: "server", type: 'string' },      
                             {name: "file", type: 'string' }
                ],
                url: url
            };


            var dataAdapter = new $.jqx.dataAdapter(source,
              {
                  loadComplete: function () {
                      var length = dataAdapter.records.length;
                      console.log(dataAdapter.records);
                  }
              }
          );

            $("#dataTable").jqxDataTable(
            {
                width: '100%',
                theme: 'energyblue',
                pageSize: 25,
                pageable: true,
                altRows: true,
                pagerButtonsCount: 10,
                source: dataAdapter,
                columnsResize: true,
                exportSettings: {fileName: "EnvSuccessFail"},
                columns: [
                          {text: "Deployment", dataField:"deployment"},
                          {text: "Deployed On", dataField:"deployedat"},
                          {text: "User", dataField:"username"},
                          {text: "Application", dataField: "application"},
                          {text: "Environment", dataField: "environment"},     
                          {text: "Component", dataField: "component"},
                          {text: "Server", dataField: "server"},      
                          {text: "File", dataField: "file"}
                 ] 
            });

            $("#excelExport").jqxButton();
            $("#xmlExport").jqxButton();
            $("#csvExport").jqxButton();
            $("#pdfExport").jqxButton();
            $("#print").jqxButton();
            
            $("#excelExport").click(function () {
                $("#dataTable").jqxDataTable('exportData', 'xls');
            });
            $("#xmlExport").click(function () {
                $("#dataTable").jqxDataTable('exportData', 'xml');
            });
            $("#csvExport").click(function () {
                $("#dataTable").jqxDataTable('exportData', 'csv');
            });
            $("#pdfExport").click(function () {
                $("#dataTable").jqxDataTable('exportData', 'pdf');
            });   

            $("#print").click(function () {
             $("#dataTable").jqxDataTable({exportSettings:  { fileName: null}});
             var gridContent = $("#dataTable").jqxDataTable('exportData', 'html');
             var newWindow = window.open('', '', 'width=800, height=500'),
             document = newWindow.document.open(),
             pageContent =
                 '<!DOCTYPE html>' +
                 '<html>' +
                 '<head>' +
                 '<meta charset="utf-8" />' +
                 '<title>File Audit Trace</title>' +
                 '</head>' +
                 '<body><H2>File Audit Trace</H2>' + gridContent + '</body></html>';
             document.write(pageContent);
             document.close();
             $("#dataTable").jqxDataTable({exportSettings:  { fileName: "EnvSuccessFail"}});
             newWindow.print();
         });  
        });

     function GetURLParameter(sParam)
     {
         var sPageURL = window.location.search.substring(1);
         var sURLVariables = sPageURL.split('&');
         for (var i = 0; i < sURLVariables.length; i++) 
         {
             var sParameterName = sURLVariables[i].split('=');
             if (sParameterName[0] == sParam) 
             {
                 return sParameterName[1];
             }
         }
     } 
     
    </script>
</head>
<body class='default'>
     <div style="text-align:center;"><H2 class="reports">File Audit Trace</H2><p></div>
     <div id="dataTable"></div>
       <div style='margin-top: 20px;'>
        <div style='float: left;'>
            <input type="button" value="Export to Excel" id='excelExport' />
            <input type="button" value="Export to XML" id='xmlExport' />
            <input type="button" value="Export to CSV" id='csvExport' />
            <input type="button" value="Export to PDF" id='pdfExport' />
            <input type="button" value="Print" id='print' />            
        </div>
    </div>
</body>
</html>