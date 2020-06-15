<!-- Release Engineer Export File -->
<action name="CreateDeployment" summary="" isGraphical="N" category="AWS">
<kind copy="N">3</kind>
<cmdline>
<script name="/bin/aws" />
<flag name="arg2" switch="deploy" pad="false" />
<flag name="arg1" switch="create-deployment" pad="false" />
<argument name="ApplicationName" type="entry" inpos="1" switchmode="S" switch="--application-name" negswitch="" pad="false" required="true" />
<argument name="DeploymentGroupName" type="entry" inpos="2" switchmode="S" switch="--deployment-group-name" negswitch="" pad="false" required="true" />
<argument name="S3Location" type="entry" inpos="3" switchmode="S" switch="--s3-location" negswitch="" pad="false" required="true" />
<argument name="DeploymentConfigName" type="entry" inpos="4" switchmode="S" switch="--deployment-config-name" negswitch="" pad="false" required="true" />
</cmdline>
<fragment name="CreateDeployment" summary="">
<parameter name="ApplicationName" type="entry" required="Y"/>
<parameter name="DeploymentGroupName" type="entry" required="Y"/>
<parameter name="S3Location" type="entry" required="Y"/>
<parameter name="DeploymentConfigName" type="entry" required="Y"/>
</fragment>
</action>
