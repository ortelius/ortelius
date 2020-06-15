<!-- Release Engineer Export File -->
<action name="WebsphereDeploy" summary="" isGraphical="N" category="WebSphere">
<kind copy="Y">4</kind>
<interpreter>${PATH_TO_WSADMIN}\wsadmin.${EXT} -lang jython -user ${WSUSERNAME} -password ${WSPASSWORD} -f </interpreter>
<scripts>
</scripts>
<cmdline>
<script name="${DMHOME}\scripts\deployws.py" />
<argument name="AppName" type="entry" inpos="1" switchmode="S" switch="-appName" negswitch="" pad="false" required="true" />
<argument name="EarFile" type="entry" inpos="2" switchmode="S" switch="-earfile" negswitch="" pad="false" required="true" />
<argument name="ContextRoot" type="entry" inpos="4" switchmode="S" switch="-contextroot" negswitch="" pad="false" required="true" />
<argument name="ServerName" type="entry" inpos="3" switchmode="S" switch="-server" negswitch="" pad="false" required="true" />
<argument name="NodeName" type="entry" inpos="5" switchmode="S" switch="-node" negswitch="" pad="false" required="true" />
<argument name="CellName" type="entry" inpos="6" switchmode="S" switch="-cell" negswitch="" pad="false" required="true" />
<argument name="ProcessEmbeddedConfig" type="checkbox" inpos="16" switchmode="B" switch="-processEmbeddedConfig" negswitch="" pad="false" required="false" />
<argument name="CreateMbeansForResource" type="checkbox" inpos="13" switchmode="B" switch="-createMBeansForResources" negswitch="" pad="false" required="false" />
<argument name="DeployWs" type="checkbox" inpos="15" switchmode="B" switch="-deployws" negswitch="" pad="false" required="false" />
<argument name="DeployEjb" type="checkbox" inpos="12" switchmode="B" switch="-deployejb" negswitch="" pad="false" required="false" />
<argument name="ValidateInstall" type="entry" inpos="7" switchmode="S" switch="-validateinstall" negswitch="" pad="false" required="false" />
<argument name="ReloadEnabled" type="checkbox" inpos="14" switchmode="B" switch="-reloadEnabled" negswitch="" pad="false" required="false" />
<argument name="MapToVirtualHost" type="checkbox" inpos="9" switchmode="B" switch="-Map2VH" negswitch="" pad="false" required="false" />
<argument name="DistributeApp" type="checkbox" inpos="10" switchmode="B" switch="-distributeApp" negswitch="" pad="false" required="false" />
<argument name="PrecompileJsps" type="checkbox" inpos="8" switchmode="B" switch="-precompileJSPs" negswitch="" pad="false" required="false" />
<argument name="UseMetadataFromBinary" type="checkbox" inpos="11" switchmode="B" switch="-useMetaDataFromBinary" negswitch="" pad="false" required="false" />
<argument name="AllowDispatchRemoteInclude" type="checkbox" inpos="17" switchmode="B" switch="-allowDispatchRemoteInclude" negswitch="" pad="false" required="false" />
<argument name="UseAutoLink" type="checkbox" inpos="19" switchmode="B" switch="-useAutoLink" negswitch="" pad="false" required="false" />
<argument name="AllowServiceRemoteInclude" type="checkbox" inpos="18" switchmode="B" switch="-allowServiceRemoteInclude" negswitch="" pad="false" required="false" />
<argument name="Tasks" type="entry" inpos="20" switchmode="S" switch="-tasks" negswitch="" pad="false" required="false" />
</cmdline>
<fragment name="Deploy App" summary="Deploys an Application to Websphere">
<parameter name="AppName" type="Entry" required="Y"/>
<parameter name="EarFile" type="Entry" required="Y"/>
<parameter name="ContextRoot" type="Entry" required="Y"/>
<parameter name="ServerName" type="Entry" required="Y"/>
<parameter name="NodeName" type="Entry" required="Y"/>
<parameter name="CellName" type="Entry" required="Y"/>
<parameter name="ProcessEmbeddedConfig" type="Checkbox" required="Y"/>
<parameter name="CreateMbeansForResource" type="Checkbox" required="Y"/>
<parameter name="DeployWs" type="Checkbox" required="Y"/>
<parameter name="DeployEjb" type="Checkbox" required="Y"/>
<parameter name="ValidateInstall" type="Checkbox" required="Y"/>
<parameter name="ReloadEnabled" type="Checkbox" required="Y"/>
<parameter name="MapToVirtualHost" type="Checkbox" required="Y"/>
<parameter name="DistributeApp" type="Checkbox" required="Y"/>
<parameter name="PrecompileJsps" type="Checkbox" required="Y"/>
<parameter name="UseMetadataFromBinary" type="Checkbox" required="Y"/>
<parameter name="AllowDispatchRemoteInclude" type="Checkbox" required="Y"/>
<parameter name="UseAutoLink" type="Checkbox" required="Y"/>
<parameter name="AllowServiceRemoteInclude" type="Checkbox" required="Y"/>
<parameter name="Tasks" type="Entry" required="Y"/>
</fragment>
</action>
