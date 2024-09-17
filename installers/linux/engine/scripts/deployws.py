import sys
import time

def wsadminToList(inStr):
        inStr = inStr.rstrip();
        outList=[]
        if (len(inStr)>0 and inStr[0]=='[' and inStr[-1]==']'):
                tmpList = inStr[1:-1].split(" ")
        else:
                tmpList = inStr.split("\n")   #splits for Windows or Linux
        for item in tmpList:
                item = item.rstrip();         #removes any Windows "\r"
                if (len(item)>0):
                      outList.append(item)
        return outList
#endDef

def isAppExists():
    l = AdminConfig.getid("/Deployment:" + appName + "/" )
    return len(l) > 0

def getNodeNameForServer(nm):
  nodes = AdminConfig.list("Node")
  nodes = wsadminToList(nodes)
  for node in nodes:
   nodeServers = AdminConfig.list("Server", node)
   nodeServers = wsadminToList(nodeServers)
   for nodeServer in nodeServers:
    sname = AdminConfig.showAttribute(nodeServer, "name")
    if sname == nm:
     nname = AdminConfig.showAttribute(node, "name")
     return nname
  return None

def stopApp():
   global nodeName
   global serverName

   if len(appName) == 0 or appName.lower() == "none":
       return

   if len(clusterName) > 0 and clusterName.lower() != "none":
    for server in clusterMembers:
     serverName=AdminConfig.showAttribute(server,'memberName')
     nodeName = AdminConfig.showAttribute(server,'nodeName')

     print 'Stopping Application "%s" on "%s/%s"...' %(appName, nodeName, serverName)
     appMgr = AdminControl.queryNames("type=ApplicationManager,node="+nodeName+",process="+serverName+",*")
     apps = wsadminToList(AdminControl.queryNames('cell=' + cellName + ',node=' + nodeName + ',type=Application,process=' + serverName + ',*'))
     found = 0
     for app in apps:
       runningName = AdminControl.getAttribute(app, 'name')
       if appName == runningName:
        found = 1
        break
     if found == 0:
      print 'Application "%s" already stopped on "%s/%s"!' %(appName, nodeName, serverName)
     else :
      AdminControl.invoke(appMgr, 'stopApplication', appName)
      print 'Application "%s" stopped on "%s/%s"!' %(appName, nodeName, serverName)
     save()
   else:
     print 'Stopping Application "%s" on "%s/%s"...' %(appName, nodeName, serverName)
     appMgr = AdminControl.queryNames("type=ApplicationManager,node="+nodeName+",process="+serverName+",*")
     apps = wsadminToList(AdminControl.queryNames('cell=' + cellName + ',node=' + nodeName + ',type=Application,process=' + serverName + ',*'))
     found = 0
     for app in apps:
       runningName = AdminControl.getAttribute(app, 'name')
       if appName == runningName:
        found = 1
        break
     if found == 0:
      print 'Application "%s" already stopped on "%s/%s"!' %(appName, nodeName, serverName)
     else :
      AdminControl.invoke(appMgr, 'stopApplication', appName)
      print 'Application "%s" stopped on "%s/%s"!' %(appName, nodeName, serverName)
     save()

def startApp():
 global nodeName
 global serverName

 if len(appName) == 0 or appName.lower() == "none":
     restartServer()
     return

 if len(clusterName) > 0 and clusterName.lower() != "none":
    configJVM()
    for server in clusterMembers:
     serverName=AdminConfig.showAttribute(server,'memberName')
     nodeName = AdminConfig.showAttribute(server,'nodeName')
     startServer()

     print 'Starting Application "%s" on "%s/%s"...' %(appName, nodeName, serverName)
     appMgr = AdminControl.queryNames("type=ApplicationManager,node="+nodeName+",process="+serverName+",*")
     result = AdminApp.isAppReady(appName)
     while (result == "false"):
       ### Wait 5 seconds before checking again
       time.sleep(5)
       result = AdminApp.isAppReady(appName)
     time.sleep(10)

     apps = wsadminToList(AdminControl.queryNames('cell=' + cellName + ',node=' + nodeName + ',type=Application,process=' + serverName + ',*'))
     found = 0
     for app in apps:
       runningName = AdminControl.getAttribute(app, 'name')
       if appName == runningName:
        found = 1
        break
     if found == 1:
      print 'Application "%s" already started on "%s/%s"!' %(appName, nodeName, serverName)
     else :
      AdminControl.invoke(appMgr, 'startApplication', appName)
      print 'Application "%s" started on "%s/%s"!' %(appName, nodeName, serverName)
     save()
 else:
     nodeName = getNodeNameForServer(serverName)
     configJVM()
     startServer()
     print 'Starting Application "%s" on "%s/%s"...' %(appName, nodeName, serverName)
     appMgr = AdminControl.queryNames("type=ApplicationManager,node="+nodeName+",process="+serverName+",*")
     result = AdminApp.isAppReady(appName)
     while (result == "false"):
       ### Wait 5 seconds before checking again
       time.sleep(5)
       result = AdminApp.isAppReady(appName)
     time.sleep(10)

     apps = wsadminToList(AdminControl.queryNames('cell=' + cellName + ',node=' + nodeName + ',type=Application,process=' + serverName + ',*'))
     found = 0
     for app in apps:
       runningName = AdminControl.getAttribute(app, 'name')
       if appName == runningName:
        found = 1
        break
     if found == 1:
      print 'Application "%s" already started on "%s/%s"!' %(appName, nodeName, serverName)
     else :
      AdminControl.invoke(appMgr, 'startApplication', appName)
      print 'Application "%s" started on "%s/%s"!' %(appName, nodeName, serverName)
     save()

def configJVM():
 global nodeName
 global serverName

 if len(clusterName) > 0 and clusterName.lower() != "none":
  for server in clusterMembers:
    serverName=AdminConfig.showAttribute(server,'memberName')
    nodeName = AdminConfig.showAttribute(server,'nodeName')

    if (changedValues(serverName,genericJvmArguments,initialHeapSize,maximumHeapSize,customProperty,websphereVariables)) :
       print "JVM Configuration Changed."
       stopServer()
    else:
       print "No JVM Configuration Changes needed."
       return

    if len(genericJvmArguments) > 0 and genericJvmArguments.lower() != "none":
       jvm = AdminConfig.getid('/Server:' + serverName + '/JavaProcessDef:/JavaVirtualMachine:/')
       AdminConfig.modify(jvm, [['genericJvmArguments',genericJvmArguments]])

    if len(initialHeapSize) > 0 and initialHeapSize.lower() != "none":
       AdminTask.setJVMProperties('[-nodeName ' + nodeName + ' -serverName ' + serverName + ' -initialHeapSize '+initialHeapSize+']')

    if len(maximumHeapSize) > 0 and maximumHeapSize.lower() != "none":
       AdminTask.setJVMProperties('[-nodeName ' + nodeName + ' -serverName ' + serverName + ' -maximumHeapSize ' + maximumHeapSize +']')

    if len(customProperty) > 0 and customProperty.lower() != "none":
       jvm = AdminConfig.getid('/Server:' + serverName + '/JavaProcessDef:/JavaVirtualMachine:/')
       props = customProperty.split(',')
       for prop in props:
         kv = prop.split('=')
         varName = kv[0]
         newVarValue = kv[1]

         # Look for existing property so we can replace it (by removing it first)
         currentProps = wsadminToList(AdminConfig.list("Property", jvm))
         for prop2 in currentProps:
           if kv[0] == AdminConfig.showAttribute(prop2, "name"):
             AdminConfig.remove(prop2);
             break

           # Store new property in 'systemProperties' object
         attrs = [["name", varName], ["value", newVarValue]]
         print "Adding property %s=%s to Server %s" %(varName,newVarValue,serverName)
         AdminConfig.create("Property", jvm, attrs, "systemProperties")

    if len(websphereVariables) > 0 and websphereVariables.lower() != "none":
       cell = AdminConfig.getid("/Cell:" + cellName + "/")
       props = websphereVariables.split(',')
       for prop in props:
         kv = prop.split('=')
         varName = kv[0]
         newVarValue = kv[1]
         varSubstitutions = wsadminToList(AdminConfig.list("VariableSubstitutionEntry",cell))

         found = 0
         for varSubst in varSubstitutions:
          getVarName = AdminConfig.showAttribute(varSubst, "symbolicName")
          if getVarName == varName:
            AdminConfig.modify(varSubst,[["value", newVarValue]])
            print getVarName+" changed to "+newVarValue
            found = 1
            break

         if found == 0:
           print varName+" added to "+newVarValue
           AdminTask.setVariable(['-variableName', varName, '-variableValue', newVarValue, '-scope', 'Cell=' + cellName])

    save()
    startServer()

 else:
    nodeName = getNodeNameForServer(serverName)

    if (changedValues(serverName,genericJvmArguments,initialHeapSize,maximumHeapSize,customProperty,websphereVariables)) :
       print "JVM Configuration Changed."
       stopServer()
    else:
       print "No JVM Configuration Changes needed."
       return

    if len(genericJvmArguments) > 0 and genericJvmArguments.lower() != "none":
       jvm = AdminConfig.getid('/Server:' + serverName + '/JavaProcessDef:/JavaVirtualMachine:/')
       AdminConfig.modify(jvm, [['genericJvmArguments',genericJvmArguments]])

    if len(initialHeapSize) > 0 and initialHeapSize.lower() != "none":
       AdminTask.setJVMProperties('[-nodeName ' + nodeName + ' -serverName ' + serverName + ' -initialHeapSize '+initialHeapSize+']')

    if len(maximumHeapSize) > 0 and maximumHeapSize.lower() != "none":
       AdminTask.setJVMProperties('[-nodeName ' + nodeName + ' -serverName ' + serverName + ' -maximumHeapSize ' + maximumHeapSize +']')

    if len(customProperty) > 0 and customProperty.lower() != "none":
       jvm = AdminConfig.getid('/Server:' + serverName + '/JavaProcessDef:/JavaVirtualMachine:/')
       props = customProperty.split(',')
       for prop in props:
         kv = prop.split('=')
         varName = kv[0]
         newVarValue = kv[1]

         # Look for existing property so we can replace it (by removing it first)
         currentProps = wsadminToList(AdminConfig.list("Property", jvm))
         for prop2 in currentProps:
           if kv[0] == AdminConfig.showAttribute(prop2, "name"):
             AdminConfig.remove(prop2);
             break

           # Store new property in 'systemProperties' object
         attrs = [["name", varName], ["value", newVarValue]]
         AdminConfig.create("Property", jvm, attrs, "systemProperties")

    if len(websphereVariables) > 0 and websphereVariables.lower() != "none":
       cell = AdminConfig.getid("/Cell:" + cellName + "/")
       props = websphereVariables.split(',')
       for prop in props:
         kv = prop.split('=')
         varName = kv[0]
         newVarValue = kv[1]
         varSubstitutions = wsadminToList(AdminConfig.list("VariableSubstitutionEntry",cell))

         found = 0
         for varSubst in varSubstitutions:
          getVarName = AdminConfig.showAttribute(varSubst, "symbolicName")
          if getVarName == varName:
            AdminConfig.modify(varSubst,[["value", newVarValue]])
            found = 1
            break

         if found == 0:
           AdminTask.setVariable(['-variableName', varName, '-variableValue', newVarValue, '-scope', 'Cell=' + cellName])

    save()
    startServer()

def changedValues(serverName, genericJvmArguments,initialHeapSize,maximumHeapSize,customProperty,websphereVariables):
    print "Checking configuration..."
    chg = 0
    jvm = AdminConfig.getid('/Server:' + serverName + '/JavaProcessDef:/JavaVirtualMachine:/')

    if (len(genericJvmArguments) > 0 and genericJvmArguments.lower() != "none"):
       current = AdminConfig.showAttribute(jvm,'genericJvmArguments')
       print "    genericJvmArguments %s=%s" %(current,genericJvmArguments)
       if (current != genericJvmArguments):
        chg = 1

    if (len(initialHeapSize) > 0 and initialHeapSize.lower() != "none"):
       current = AdminConfig.showAttribute(jvm,'initialHeapSize')
       print "    initialHeapSize %s=%s" %(current,initialHeapSize)
       if (current != initialHeapSize):
        cnt = 1

    if (len(maximumHeapSize) > 0 and maximumHeapSize.lower() != "none"):
       current = AdminConfig.showAttribute(jvm,'maximumHeapSize')
       print "    maximumHeapSize %s=%s" %(current,maximumHeapSize)
       if (current != maximumHeapSize):
        chg = 1

    if (len(customProperty) > 0 and customProperty.lower() != "none"):
       props = customProperty.split(',')
       cnt=0
       for prop in props:
         kv = prop.split('=')
         varName = kv[0]
         newVarValue = kv[1]

         # Look for existing property so we can replace it (by removing it first)
         currentProps = wsadminToList(AdminConfig.list("Property", jvm))
         for prop2 in currentProps:
           if varName == AdminConfig.showAttribute(prop2, "name"):
             current =  AdminConfig.showAttribute(prop2, "value")
             print "    customProperty %s=%s" %(current,newVarValue)
             if newVarValue != current:
              cnt = cnt +1
              break
       if (cnt > 0):
        chg = 1

    if len(websphereVariables) > 0 and websphereVariables.lower() != "none":
       cell = AdminConfig.getid("/Cell:" + cellName + "/")
       props = websphereVariables.split(',')
       cnt=0
       for prop in props:
         kv = prop.split('=')
         varName = kv[0]
         newVarValue = kv[1]
         varSubstitutions = wsadminToList(AdminConfig.list("VariableSubstitutionEntry",cell))

         for varSubst in varSubstitutions:
          getVarName = AdminConfig.showAttribute(varSubst, "symbolicName")
          if getVarName == varName:
            current = AdminConfig.showAttribute(varSubst, "value")
            print "    websphereVariables %s=%s" %(current,newVarValue)
            if newVarValue != current:
             cnt = cnt + 1
             break
       if (cnt > 0):
        chg = 1

    if (chg > 0):
      return 1
    else :
      return 0

def changedDataSource():

    if len(dbUser) > 0 and dbUser.lower() != "none" and len(dbName) > 0 and dbName.lower() != "none" and len(dbPassword) > 0 and dbPassword.lower() != "none" and len(dbPort) > 0 and dbPort.lower() != "none" and len(dbHostName) > 0 and dbHostName.lower() != "none":
       print "Updating DataSource..."
       mgmtnodeName = AdminControl.getNode()
       dbFullUser = mgmtnodeName + "/" + dbUser

       sec = AdminConfig.getid('/Cell:'+ cellName +'/Security:/')
       alias_attr = ["alias" , dbFullUser]
       desc_attr = ["description" , "alias"]
       userid_attr = ["userId" , dbUser ]
       password_attr = ["password" , dbPassword]
       attrs = [alias_attr , desc_attr , userid_attr , password_attr ]
       authdata = AdminConfig.create('JAASAuthData' , sec , attrs)
       print " Created new JASSAuthData with Alias name :"+ dbFullUser
       AdminConfig.save()

       datasource = dbName
       print " Name of datasource which will be created on JDBC Provider :"+ dbName +" is :"+ datasource
       ds = AdminConfig.getid('/Cell:'+ cellName +'/ServerCluster:'+ clusterName +'/JDBCProvider:'+ datasource)
       name1 = ["name" , datasource]
       jndi = ["jndiName" , "jdbc/" + dbName]
       authentication = ["authDataAlias" , dbFullUser]
       ds_hlpclass = ["datasourceHelperClassname" , "com.ibm.websphere.rsadapter.DB2UniversalDataStoreHelper"]
       map_configalias_attr=["mappingConfigAlias", "DefaultPrincipalMapping"]
       map_attrs=[authentication , map_configalias_attr]
       mapping_attr=["mapping", map_attrs]
       ds_attr = [name1 , jndi , authentication , ds_hlpclass ,mapping_attr ]
       newds = AdminConfig.create('DataSource' , ds , ds_attr)
       print " New DataSource created with name :"+ datasource
       AdminConfig.save()
    return 0

def removeApp():

    if len(appName) == 0 or appName.lower() == "none":
        return

    if isAppExists():
     print 'Removing Application "%s"...' %(appName)
     AdminApp.uninstall(appName)
     AdminConfig.save();
     print 'Application "%s" removed successfully!' %(appName)

def synchronizeNode():
    print 'Synchronizing node "%s"...' %(nodeName)
    AdminControl.invoke(AdminControl.completeObjectName('type=NodeSync,node='+nodeName+',*'), 'sync')
    print 'Node "%s" synchronized successfully!' %(nodeName)

def startServer():
    serverObj = AdminControl.completeObjectName('WebSphere:type=Server,name=' + serverName + ',*')
    if len(serverObj) > 0:
      serverStatus = AdminControl.getAttribute(serverObj, 'state')
    else:
      serverStatus = 'STOPPED'

    if serverStatus != "STARTED":
      print 'Starting server "%s" on node "%s"...' %(serverName, nodeName)
      AdminControl.startServer(serverName, nodeName)
    serverStatus = 'STOPPED'

    while (serverStatus != 'STARTED'):
     serverObj = AdminControl.completeObjectName('WebSphere:type=Server,name=' + serverName + ',*')
     if len(serverObj) > 0:
      serverStatus = AdminControl.getAttribute(serverObj, 'state')
     else:
      serverStatus = 'STOPPED'
     time.sleep(5)
    print "Server %s is now %s" %(serverName, serverStatus)

def stopServer():
    serverObj = AdminControl.completeObjectName('WebSphere:type=Server,name=' + serverName + ',*')
    if len(serverObj) > 0:
      serverStatus = AdminControl.getAttribute(serverObj, 'state')
    else:
      serverStatus = 'STOPPED'

    if serverStatus != "STOPPED":
      print 'Stopping server "%s" on node "%s"...' %(serverName, nodeName)
      AdminControl.stopServer(serverName, nodeName)
      while (serverStatus != 'STOPPED'):
       serverObj = AdminControl.completeObjectName('WebSphere:type=Server,name=' + serverName + ',*')
       if len(serverObj) > 0:
        serverStatus = AdminControl.getAttribute(serverObj, 'state')
       else:
        serverStatus = 'STOPPED'
      time.sleep(5)
    print "Server %s is now %s" %(serverName, serverStatus)

def restartServer():
    serverObj = AdminControl.completeObjectName('WebSphere:type=Server,name=' + serverName + ',*')

    if len(serverObj) > 0:
      serverStatus = AdminControl.getAttribute(serverObj, 'state')
    else:
      serverStatus = 'STOPPED'

    if serverStatus != "STOPPED":
      print 'Stopping server "%s" on node "%s"...' %(serverName, nodeName)
      AdminControl.stopServer(serverName)
      while (serverStatus != 'STOPPED'):
       serverObj = AdminControl.completeObjectName('WebSphere:type=Server,name=' + serverName + ',*')
       if len(serverObj) > 0:
        serverStatus = AdminControl.getAttribute(serverObj, 'state')
       else:
        serverStatus = 'STOPPED'
       time.sleep(5)

    print "Server %s is now %s" %(serverName, serverStatus)

    serverObj = AdminControl.completeObjectName('WebSphere:type=Server,name=' + serverName + ',*')
    if serverStatus != "STARTED":
      print 'Starting server "%s" on node "%s"...' %(serverName, nodeName)
      AdminControl.startServer(serverName)
    serverStatus = 'STOPPED'

    while (serverStatus != 'STARTED'):
     serverObj = AdminControl.completeObjectName('WebSphere:type=Server,name=' + serverName + ',*')
     if len(serverObj) > 0:
      serverStatus = AdminControl.getAttribute(serverObj, 'state')
     else:
      serverStatus = 'STOPPED'
     time.sleep(5)
    print "Server %s is now %s" %(serverName, serverStatus)

def stopCluster():
    print 'Stopping cluster "%s"...' %(clusterName)
    print adminControlClusterId
    AdminControl.invoke(adminControlClusterId,'stop')
    stopApp()
    print 'Cluster "%s" stopped successfully' %(clusterName)

def startCluster():
    print 'Starting cluster "%s"...' %(clusterName)
    AdminControl.invoke(adminControlClusterId,'start')
    startApp()
    print 'Cluster "%s" started successfully' %(clusterName)

def installApp():
    if len(appName) == 0 or appName.lower() == "none":
        return

    if isAppExists():
     removeApp()
    print 'Installing application from "%s" ...' %(fileName)
    print options
    AdminApp.install(fileName, options)
    save()

    if warClassLoaderPolicy == 'YES':
     dep = AdminConfig.getid("/Deployment:" + appName + "/")
     deployedObject = AdminConfig.showAttribute(dep, "deployedObject")
     AdminConfig.modify(deployedObject, [['warClassLoaderPolicy', 'SINGLE']])

    if classLoader == 'YES':
     dep = AdminConfig.getid("/Deployment:" + appName + "/")
     deployedObject = AdminConfig.showAttribute(dep, "deployedObject")
     classldr = AdminConfig.showAttribute(deployedObject,"classloader")
     AdminConfig.modify(classldr, [['mode', 'PARENT_LAST']])

    if len(startingWeight) > 0 and startingWeight.lower() != "none":
      dep = AdminConfig.getid("/Deployment:" + appName + "/")
      deployedObject = AdminConfig.showAttribute(dep, "deployedObject")
      AdminConfig.modify(deployedObject, [['startingWeight', startingWeight]])

    save()
    print 'Successfully installed application "%s"' %(fileName)

def stopAndRemoveApp():
    if isAppExists():
        stopApp()
        removeApp()
        save()

def save():
    print 'Saving the changes...'
    AdminConfig.save()
    print 'Changes saved successfully.'


def main(Args):

   global options
   global appName
   global serverName
   global dbUser
   global dbName
   global dbPassword
   global dbPort
   global dbHostName
   global clusterName
   global appMgr
   global cellName
   global fileName
   global nodeName
   global contextroot
   global baseFileName
   global modList
   global module
   global uri
   global virtualhost
   global clusterId
   global adminControlClusterId
   global clusterMembers
   global warClassLoaderPolicy
   global classLoader
   global genericJvmArguments
   global initialHeapSize
   global maximumHeapSize
   global startingWeight
   global customProperty
   global websphereVariables
   global additionalServers

   appName = ""
   serverName = ""
   clusterName = ""
   dbUser = ""
   dbName = ""
   dbPassword = ""
   dbPort = ""
   dbHostName = ""
   cellName = ""
   fileName = ""
   nodeName = ""
   contextroot = ""
   Map2VH = ""
   MapModule2Server = "YES"
   validateinstall = ""
   module = ""
   uri = ""
   virtualhost = "none"
   warClassLoaderPolicy = ""
   classLoader = ""
   genericJvmArguments = ""
   initialHeapSize = ""
   maximumHeapSize = ""
   startingWeight = ""
   customProperty = ""
   websphereVariables = ""
   additionalServers = ""

   print "*******"
   print Args
   print "*******"
   precompileJSPs="-nopreCompileJSPs"
   distributeApp="-nodistributeApp"
   useMetaDataFromBinary="-nouseMetaDataFromBinary"
   deployejb="-nodeployejb"
   createMBeansForResources="-nocreateMBeansForResources"
   reloadEnabled="-noreloadEnabled"
   deployws="-nodeployws"
   processEmbeddedConfig="-noprocessEmbeddedConfig"
   allowDispatchRemoteInclude="-noallowDispatchRemoteInclude"
   allowServiceRemoteInclude="-noallowServiceRemoteInclude"
   useAutoLink="-nouseAutoLink"
   baseFileName=""

   tasks       = ['changedDataSource', 'stopAndRemoveApp','installApp','startApp']
   funclist = {'stopAndRemoveApp': stopAndRemoveApp,
               'installApp': installApp,
               'stopSever': stopServer,
               'startServer': startServer,
               'restartServer': restartServer,
               'synchronizeNode': synchronizeNode,
               'changedDataSource': changedDataSource,
               'stopApp': stopApp,
               'startApp': startApp,
               'stopCluster': stopCluster,
               'startCluster': startCluster}

   for i in range(len(Args)):
    larg = Args[i].lower()

    if larg == "-cell".lower():
     cellName=Args[i+1]
     i +=1
    elif larg == "-node".lower():
     nodeName=Args[i+1]
     i +=1
    elif larg == "-server".lower():
     serverName=Args[i+1]
     i +=1
    elif larg == "-dbUser".lower():
     dbUser=Args[i+1]
     i +=1
    elif larg == "-dbName".lower():
     dbName=Args[i+1]
     i +=1
    elif larg == "-dbPassword".lower():
     dbPassword=Args[i+1]
     i +=1
    elif larg == "-dbPort".lower():
     dbPort=Args[i+1]
     i +=1
    elif larg == "-dbHostName".lower():
     dbHostName=Args[i+1]
     i +=1
    elif larg == "-cluster".lower():
     clusterName=Args[i+1]
     i +=1
    elif larg == "-virtualhost".lower():
     virtualhost=Args[i+1]
     i +=1
    elif larg == "-preCompileJSPs".lower():
     precompileJSPs ="-preCompileJSPs"
    elif larg == "-Map2VH".lower():
     Map2VH="YES"
    elif larg == "-MapModuleToServer".lower():
     MapModule2Server="YES"
    elif larg == "-distributeApp".lower():
     distributeApp = "-distributeApp"
    elif larg == "-useMetaDataFromBinary".lower():
     useMetaDataFromBinary = "-useMetaDataFromBinary"
    elif larg == "-deployejb".lower():
     deployejb ="-deployejb"
    elif larg == "-createMBeansForResources".lower():
     createMBeansForResources = "-createMBeansForResources"
    elif larg == "-reloadEnabled".lower():
     reloadEnabled = "-reloadEnabled"
    elif larg == "-warClassLoaderPolicy".lower():
     warClassLoaderPolicy=Args[i+1]
     i +=1
    elif larg == "-classLoader".lower():
     classLoader=Args[i+1]
     i +=1
    elif larg == "-genericJvmArguments".lower():
     genericJvmArguments=Args[i+1]
     i +=1
    elif larg == "-customProperty".lower():
     customProperty=Args[i+1]
     i +=1
    elif larg == "-websphereVariables".lower():
     websphereVariables=Args[i+1]
     i +=1
    elif larg == "-initialHeapSize".lower():
     initialHeapSize=Args[i+1]
     i +=1
    elif larg == "-additionalServers".lower():
     additionalServers=Args[i+1]
     i +=1
    elif larg == "-maximumHeapSize".lower():
     maximumHeapSize=Args[i+1]
     i +=1
    elif larg == "-startingWeight".lower():
     startingWeight=Args[i+1]
     i +=1
    elif larg == "-deployws".lower():
     deployws = "-deployws"
    elif larg == "-processEmbeddedConfig".lower():
     processEmbeddedConfig = "-processEmbeddedConfig"
    elif larg == "-allowDispatchRemoteInclude".lower():
     allowDispatchRemoteInclude = "-allowDispatchRemoteInclude"
    elif larg == "-allowServiceRemoteInclude".lower():
     allowServiceRemoteInclude = "-allowServiceRemoteInclude"
    elif larg == "-useAutoLink".lower():
     useAutoLink = "-useAutoLink"
    elif larg == "-appName".lower():
     appName=Args[i+1]
     i +=1
    elif larg == "-tasks".lower():
     tasks=Args[i+1].split(',')
     i +=1
    elif larg == "-earfile".lower():
     fileName=Args[i+1]
     i +=1
    elif larg == "-contextroot".lower():
     contextroot=Args[i+1]
     i +=1
    elif larg == "-validateinstall".lower():
     validateinstall=Args[i+1]
     i +=1

   options = [ precompileJSPs,
              distributeApp,
              useMetaDataFromBinary,
              deployejb,
              createMBeansForResources,
              reloadEnabled,
              deployws,
              processEmbeddedConfig,
              allowDispatchRemoteInclude,
              allowServiceRemoteInclude,
              useAutoLink,
              '-filepermission .*\.dll=755#.*\.so=755#.*\.a=755#.*\.sl=755'
            ]


   if len(clusterName) > 0 and clusterName.lower() != "none":
    clusterId=AdminConfig.getid("/ServerCluster:"+clusterName)
    adminControlClusterId=AdminControl.completeObjectName("type=Cluster,name="+clusterName+",*")
    clusterMembers=AdminConfig.list('ClusterMember', clusterId)
    clusterMembers = wsadminToList(clusterMembers)
   else:
    appMgr = AdminControl.queryNames("type=ApplicationManager,node="+nodeName+",process="+serverName+",*")

   if len(warClassLoaderPolicy) > 0 and warClassLoaderPolicy.lower() != "none":
     warClassLoaderPolicy = "YES"

   if len(classLoader) > 0 and classLoader.lower() != "none":
     classLoader = "YES"

   if len(fileName) > 0 and fileName.lower() != "none":
    parts=fileName.split("/");
    baseFileName=parts[-1];

   if len(contextroot) > 0  and contextroot.lower() != "none":
        options.append('-contextroot')
        options.append(contextroot)

   if len(additionalServers) > 0  and additionalServers.lower() != "none":
        addsrv = additionalServers.split(',')
        additionalServers = ""
        for srv in addsrv:
          nspair = srv.split('/')
          additionalServers = additionalServers + "+WebSphere:cell="+cellName+ ",node=" + nspair[0] + ",server=" + nspair[1]
   else:
       additionalServers = ""

   if len(cellName) == 0 or cellName.lower() == "none":
        cells = AdminConfig.list("Cell" )
        cells = wsadminToList(cells)
        cellName = cells[0].split('(')[0]

   if len(validateinstall) > 0:
        options.append('-validateinstall')
        options.append(validateinstall)

   if virtualhost.lower() != "none":
     modList = AdminApp.taskInfo(fileName,"MapModulesToServers").split("\n")
     modules = []


     for line in modList:
      if line.startswith("Module: "):
       module=line[len("Module: "):].replace('\r','')
      elif line.startswith("URI: "):
       uri=line[len("URI: "):].replace('\r','')
       modules.append("'" + module + "' " + uri)
       location = " WebSphere:cell="+cellName+",server="+serverName
       if len(clusterName) > 0 and clusterName.lower() != "none":
        location = " WebSphere:cell="+cellName+",cluster="+clusterName

     opt = "-MapWebModToVH ["
     for line in modules:
       if line.find('.war') >= 0:
        opt = opt + "[" + line + " " + virtualhost + "]"
     opt = opt + "]"
     options.append(opt)

   if MapModule2Server == "YES" and len(fileName) > 0 and fileName.lower() != "none":
     modList = AdminApp.taskInfo(fileName,"MapModulesToServers").split("\n")
     modules = []

     for line in modList:
      if line.startswith("Module: "):
       module=line[len("Module: "):].replace('\r','')
      elif line.startswith("URI: "):
       uri=line[len("URI: "):].replace('\r','')
       modules.append("'" + module + "' " + uri)
       location = " WebSphere:cell="+cellName+",server="+serverName + additionalServers
       if len(clusterName) > 0 and clusterName.lower() != "none":
        location = " WebSphere:cell="+cellName+",cluster="+clusterName + additionalServers

     opt = "-MapModulesToServers ["
     for line in modules:
       opt = opt + "[" + line + location + "]"
     opt = opt + "]"
     options.append(opt)

   if len(appName) > 0 and appName.lower() != "none":
        options.append('-appname')
        options.append(appName)

   print 'Completed the initialization successfully.'

   print options
   print tasks

   for currenttask in tasks:
        funclist[currenttask]()

if __name__ == '__main__' or __name__ == 'main':
   main(sys.argv[0:])
