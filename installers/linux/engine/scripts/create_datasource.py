import sys,java
from java.util import Properties
from java.io import FileInputStream
from org.python.modules import time
lineSep = java.lang.System.getProperty('line.separator')

def datasource(cluster,user,password,url,env,jdbc_driver,timeOut,maxConn,minConn,reapTime,unusdTimeout,agedTimeout):

# Declare global variables 

global AdminConfig 
global AdminControl



## JDBCProvider ##


name = "jdbcOracle"+ env

print " Name of JDBC Provider which will be created ---> " + name

print " ----------------------------------------------------------------------------------------- "

# Gets the name of cell

cell = AdminControl.getCell()

cellid = AdminConfig.getid('/Cell:'+ cell +'/')

print " ----------------------------------------------------------------------------------------- "



# checking for the existence of Cluster


Serverid = AdminConfig.getid('/Cell:'+ cell +'/ServerCluster:'+ cluster +'/')

print " Checking for existence of Server :" + cluster

if len(Serverid) == 0:

print "Cluster doesnot exists "

else:

print "Cluster exist:"+ cluster

print " ----------------------------------------------------------------------------------------- "


## removing old jdbc provider and creating a new jdbc provider 

print " Checking for the existence of JDBC Provider :"+ name 

s2 = AdminConfig.getid('/Cell:'+ cell +'/ServerCluster:'+ cluster +'/JDBCProvider:'+ name) 

if len(s2) > 0:

print " JDBC Provider exists with name :"+ name

print " Removing JDBC Provider with name :"+ name

AdminConfig.remove(s2)

print " JDBC Provider removed "

AdminConfig.save()

print " Saving Configuraion "

print " ----------------------------------------------------------------------------------------- " 

## Creating New JDBC Provider ##

print " Creating New JDBC Provider :"+ name 

n1 = ["name" , name ]

desc = ["description" , "Oracle JDBC Driver"]

impn = ["implementationClassName" , "oracle.jdbc.pool.OracleConnectionPoolDataSource"]

classpath = ["classpath" , jdbc_driver ]

attrs1 = [n1 , impn , desc , classpath]

jdbc = AdminConfig.create('JDBCProvider' , Serverid , attrs1)

print " New JDBC Provider created :"+ name 

AdminConfig.save()

print " Saving Configuraion " 

print " ----------------------------------------------------------------------------------------- "

## checking for the existence of JAASAuthData and deleting ## 

node = AdminControl.getNode()

alias1 = node +"/"+ env

print " Checking for the existence of JAASAUTHDATA :"+ alias1 

jaasAuthDataList = AdminConfig.list("JAASAuthData")

if len(jaasAuthDataList) == 0: 

print " Creating New JAASAuthData with Alias name :"+ alias1

sec = AdminConfig.getid('/Cell:'+ cell +'/Security:/')

alias_attr = ["alias" , alias1]

desc_attr = ["description" , "alias"]

userid_attr = ["userId" , user ]

password_attr = ["password" , password]

attrs = [alias_attr , desc_attr , userid_attr , password_attr ]

authdata = AdminConfig.create('JAASAuthData' , sec , attrs)

print " Created new JASSAuthData with Alias name :"+ alias1

AdminConfig.save()

print " Saving Configuraion "

print " ----------------------------------------------------------------------------------------- "

else :

matchFound = 0 

jaasAuthDataList = AdminConfig.list("JAASAuthData") 

jaasAuthDataList=jaasAuthDataList.split(lineSeparator)

for jaasAuthId in jaasAuthDataList:

getAlias = AdminConfig.showAttribute(jaasAuthId, "alias")

if (cmp(getAlias,alias1) == 0):

print " JAASAuthData exists with name :"+ alias1

print " Removing JAASAuthData with name :"+ alias1

AdminConfig.remove(jaasAuthId)

print " JAASAuthData removed " 

AdminConfig.save()

print " Saving Configuraion " 

matchFound = 1

break

if (matchFound == 0):

print " No match was found for the given JASSAuthData : "+ alias1

#endIf

print " ----------------------------------------------------------------------------------------- "


## J2C Authentication Entries ##

print " Creating New JAASAuthData with Alias name :"+ alias1

sec = AdminConfig.getid('/Cell:'+ cell +'/Security:/')

alias_attr = ["alias" , alias1]

desc_attr = ["description" , "alias"]

userid_attr = ["userId" , user ]

password_attr = ["password" , password]

attrs = [alias_attr , desc_attr , userid_attr , password_attr ]

authdata = AdminConfig.create('JAASAuthData' , sec , attrs)

print " Created new JASSAuthData with Alias name :"+ alias1 

AdminConfig.save()

print " Saving Configuraion "

print " ----------------------------------------------------------------------------------------- " 

## DataSource ##

datasource = "DataSource"+ env

print " Name of datasource which will be created on JDBC Provider :"+ name +" is :"+ datasource

ds = AdminConfig.getid('/Cell:'+ cell +'/ServerCluster:'+ cluster +'/JDBCProvider:'+ name) 

name1 = ["name" , datasource]

jndi = ["jndiName" , "jdbc/tiers3DS"]

authentication = ["authDataAlias" , alias1]

st_cachesize = ["statementCacheSize" , "150"]

ds_hlpclass = ["datasourceHelperClassname" , "com.ibm.websphere.rsadapter.Oracle10gDataStoreHelper"]

map_configalias_attr=["mappingConfigAlias", "DefaultPrincipalMapping"]

map_attrs=[authentication , map_configalias_attr]

mapping_attr=["mapping", map_attrs]

ds_attr = [name1 , jndi , authentication , st_cachesize , ds_hlpclass ,mapping_attr ]

newds = AdminConfig.create('DataSource' , ds , ds_attr)

print " New DataSource created with name :"+ datasource 

AdminConfig.save()

print " Saving Configuraion "

print " ----------------------------------------------------------------------------------------- "

## set the properties for the datasource ##

print " Setting the properties for DataSource :"+ datasource

newds1 = AdminConfig.getid('/Cell:'+ cell +'/ServerCluster:'+ cluster +'/JDBCProvider:'+ name +'/DataSource:'+ datasource)

propSet = AdminConfig.create('J2EEResourcePropertySet' , newds1 , "")

name3 = ["name" , "URL"]

type = ["type" , "java.lang.String"]

required = ["required" , "true"]

value = ["value" , url]

rpAttrs = [name3 , type , required , value]

jrp = AdminConfig.create('J2EEResourceProperty' , propSet , rpAttrs)


print " Properties created for DataSource :"+ datasource 

AdminConfig.save()

print " Saving Configuraion "

print " ----------------------------------------------------------------------------------------- "

# Create an associated connection pool for the new DataSource#

print " Creating Connection Pool Setting for DataSource :"+ datasource

timeout = ["connectionTimeout" , timeOut]

maxconn = ["maxConnections" , maxConn]

minconn = ["minConnections" , minConn]

reaptime = ["reapTime" , reapTime]

unusedtimeout = ["unusedTimeout" , unusdTimeout]

agedtimeout = ["agedTimeout" , agedTimeout]

purgepolicy = ["purgePolicy" , "EntirePool"]

connPoolAttrs = [timeout , maxconn , minconn , reaptime , unusedtimeout , agedtimeout , purgepolicy] 

AdminConfig.create("ConnectionPool", newds , connPoolAttrs)

print " Connection Pool Setting created for DataSource :"+ datasource 

AdminConfig.save()

print " Saving Configuraion "

print " ----------------------------------------------------------------------------------------- "


## Full Syncronization ##

print " Syncronizing configuration with Master Repository "

nodelist = AdminTask.listManagedNodes().split(lineSep)

for nodename in nodelist :

print " Doing Full Resyncronization of node.......... "

####################Identifying the ConfigRepository MBean and assign it to variable######################

repo = AdminControl.completeObjectName('type=ConfigRepository,process=nodeagent,node='+ nodename +',*')

print AdminControl.invoke(repo, 'refreshRepositoryEpoch')

sync = AdminControl.completeObjectName('cell='+ cell +',node='+ nodename +',type=NodeSync,*')

print AdminControl.invoke(sync , 'sync')

#time.sleep(20)

print " ----------------------------------------------------------------------------------------- "

print " Full Resyncronization completed "

print " ----------------------------------------------------------------------------------------- "

#######Restarting Node Agent#########

nodelist = AdminTask.listManagedNodes().split(lineSep)

for nodename in nodelist :

print " Restarting Nodeagent of "+nodename+" node " 

na = AdminControl.queryNames('type=NodeAgent,node='+nodename+',*') 

AdminControl.invoke(na,'restart','true true') 

print " ----------------------------------------------------------------------------------------- "

time.sleep(30)

##########Testing Database Connection################

dsid = AdminConfig.getid('/ServerCluster:'+ cluster +'/JDBCProvider:'+ name +'/DataSource:'+ datasource +'/')

print " Testing Database Connection"

print AdminControl.testConnection(dsid)

print " ----------------------------------------------------------------------------------------- "
####################################################################################################################
####################################################################################################################

#main program starts here

arglen=len(sys.argv)

num_exp_args=2

if (arglen != num_exp_args):

print "Two arguments are required.one of them is property file"

print " ----------------------------------------------------------------------------------------- "

sys.exit(-1)

propFile=sys.argv[0]

properties=Properties();

try:

properties.load(FileInputStream(propFile))

print " ----------------------------------------------------------------------------------------- "

print "Succesfully read property file "+propFile

print " ----------------------------------------------------------------------------------------- "

except:

print "Cannot read property file "+propFile
sys.exit(-1)

print " ----------------------------------------------------------------------------------------- "

cluster = str(properties.getProperty("CLUSTER_NAME"))
env = sys.argv[1]
user = str(properties.getProperty("dbms.userId"))
password = str(properties.getProperty("dbms.password"))
url = str(properties.getProperty("dbms.url"))
jdbc_driver = str(properties.getProperty("JDBC_DRIVER_PATH"))
timeOut = int(properties.getProperty("TIMEOUT"))
maxConn = int(properties.getProperty("MAXCONN"))
minConn = int(properties.getProperty("MINCONN"))
reapTime = int(properties.getProperty("REAPTIME"))
unusdTimeout = int(properties.getProperty("UNUSEDTIMEOUT"))
agedTimeout = int(properties.getProperty("AGEDTIMEOUT"))

datasource(cluster,user,password,url,env,jdbc_driver,timeOut,maxConn,minConn,reapTime,unusdTimeout,agedTimeout)