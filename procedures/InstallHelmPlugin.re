<!-- Release Engineer Export File -->
<action name="InstallHelmPlugin" summary="Installs a single helm plugin" isGraphical="N" category="General">
<kind copy="N">2</kind>
<cmdline>
<script name="$DMHOME/scripts/installhelmplugin.sh" />
<argument name="PluginUrl" type="entry" inpos="1" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<argument name="PluginVersion" type="entry" inpos="2" switchmode="S" switch="" negswitch="" pad="false" required="true" />
</cmdline>
<fragment name="InstallHelmPlugin" summary="">
<parameter name="PluginUrl" type="entry" required="Y"/>
<parameter name="PluginVersion" type="entry" required="Y"/>
</fragment>
</action>
