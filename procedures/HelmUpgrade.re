<!-- DeployHub Export File -->
<action name="HelmUpgrade" summary="" isGraphical="N" category="General">
<kind copy="N">3</kind>
<cmdline>
<script name="$DMHOME/scripts/dh2helm.py" />
<argument name="ReleaseName" type="entry" inpos="3" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<argument name="Chart" type="entry" inpos="2" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<argument name="RspFile" type="entry" inpos="1" switchmode="S" switch="" negswitch="" pad="false" required="true" />
</cmdline>
<fragment name="HelmUpgrade" summary="">
<parameter name="RspFile" type="entry" required="Y"/>
<parameter name="Chart" type="entry" required="Y"/>
<parameter name="ReleaseName" type="entry" required="Y"/>
</fragment>
</action>
