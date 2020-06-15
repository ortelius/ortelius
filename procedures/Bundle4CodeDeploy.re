<!-- Release Engineer Export File -->
<action name="Bundle4CodeDeploy" summary="" isGraphical="N" category="AWS">
<kind copy="N">3</kind>
<cmdline>
<script name="$DMHOME/scripts/zipcd.sh" />
<argument name="WorkDir" type="entry" inpos="2" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<argument name="ZipName" type="entry" inpos="1" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<flag name="arg2" switch="*" pad="false" />
</cmdline>
<fragment name="Bundle4CodeDeploy" summary="">
<parameter name="ZipName" type="entry" required="Y"/>
<parameter name="WorkDir" type="entry" required="Y"/>
</fragment>
</action>
