<!-- Release Engineer Export File -->
<action name="RunAnt" summary="" isGraphical="N" category="Deploy">
<kind copy="N">3</kind>
<cmdline>
<script name="/usr/bin/ant" />
<flag name="arg1" switch="-f" pad="false" />
<argument name="BuildFile" type="entry" inpos="1" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<argument name="Target" type="entry" inpos="2" switchmode="S" switch="" negswitch="" pad="false" required="false" />
</cmdline>
<fragment name="RunAnt" summary="">
<parameter name="BuildFile" type="entry" required="Y"/>
<parameter name="Target" type="entry" required="N"/>
</fragment>
</action>
