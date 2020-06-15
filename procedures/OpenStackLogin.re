<!-- Release Engineer Export File -->
<function name="OpenStackLogin" summary="" isExpr="Y" category="OpenStack">
<kind copy="N">3</kind>
<cmdline>
<script name="/usr/bin/openstack-login" />
<argument name="AuthEndPoint" type="entry" inpos="1" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<argument name="AuthPayload" type="entry" inpos="2" switchmode="S" switch="" negswitch="" pad="false" required="true" />
</cmdline>
<fragment name="OpenStackLogin" summary="">
<parameter name="AuthEndPoint" type="entry" required="Y"/>
<parameter name="AuthPayload" type="entry" required="Y"/>
<result name="result" />
</fragment>
</function>
