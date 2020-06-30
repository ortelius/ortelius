<!-- Release Engineer Export File -->
<action name="TomcatPostDeploy" summary="Cleans Work Directory and Starts Tomcat" isGraphical="N" category="Tomcat">
<kind copy="Y">4</kind>
<scripts>
</scripts>
<cmdline>
<script name="$DMHOME\scripts\tomcatctl.${EXT}" />
<flag name="arg1" switch="Post" pad="false" />
<flag name="arg2" switch="$TOMCAT_SERVICE_NAME" pad="false" />
<flag name="arg3" switch="$TOMCAT_ROOT" pad="false" />
<flag name="arg4" switch="$APPLICATION_NAME" pad="false" />
</cmdline>
<fragment name="TomcatPostDeploy" summary="Cleans Work Directory and Starts Tomcat">
</fragment>
</action>
