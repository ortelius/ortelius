<!-- Release Engineer Export File -->
<action name="TomcatPreDeploy" summary="Shuts down Tomcat before deployment" isGraphical="N" category="Tomcat">
<kind copy="Y">4</kind>
<scripts>
</scripts>
<cmdline>
<script name="$DMHOME\scripts\tomcatctl.${EXT}" />
<flag name="arg1" switch="Pre" pad="false" />
<flag name="arg2" switch="$TOMCAT_SERVICE_NAME" pad="false" />
<flag name="arg3" switch="$TOMCAT_ROOT" pad="false" />
<flag name="arg4" switch="$APPLICATION_NAME" pad="false" />
</cmdline>
<fragment name="TomcatPreDeploy" summary="Shuts down Tomcat before deployment">
</fragment>
</action>
