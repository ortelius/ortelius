<!-- Release Engineer Export File -->
<action name="S3Upload" summary="" isGraphical="N" category="AWS">
<kind copy="N">3</kind>
<cmdline>
<script name="/bin/aws" />
<flag name="arg1" switch="s3" pad="false" />
<flag name="arg2" switch="cp" pad="false" />
<argument name="FromFile" type="entry" inpos="1" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<argument name="ToFile" type="entry" inpos="2" switchmode="S" switch="" negswitch="" pad="false" required="true" />
</cmdline>
<fragment name="S3Upload" summary="">
<parameter name="FromFile" type="entry" required="Y"/>
<parameter name="ToFile" type="entry" required="Y"/>
</fragment>
</action>
