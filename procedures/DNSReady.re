<!-- Release Engineer Export File -->
<function name="DNSReady" summary="" isExpr="N" category="General">
<kind copy="N">3</kind>
<cmdline>
<script name="/opt/deployhub/engine/scripts/dnsready.pl" />
<argument name="Name" type="entry" inpos="1" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<argument name="IpAddr" type="entry" inpos="2" switchmode="S" switch="" negswitch="" pad="false" required="true" />
</cmdline>
<fragment name="DNSReady" summary="">
<parameter name="Name" type="entry" required="Y"/>
<parameter name="IpAddr" type="entry" required="Y"/>
<result name="result" />
</fragment>
</function>
