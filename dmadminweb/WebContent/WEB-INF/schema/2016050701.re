<!-- DeployHub Export File -->
<action name="ansible_set_credentials" summary="" isGraphical="N" category="General">
<kind copy="N">2</kind>
<dmscript>
aWYgKCRTc2hDcmVkZW50aWFsICE9ICIiKSB7CgkvLyBVc2UgU3VwcGxpZWQgQ3JlZGVudGlhbCBOYW1lCgllY2hvICJJTkZPOiBVc2luZyBTdXBwbGllZCBDcmVkZW50aWFsICRTc2hDcmVkZW50aWFsIjsKCXNldCBjcmVkID0gZ2V0Y3JlZGVudGlhbCgkU3NoQ3JlZGVudGlhbCk7Cn0gZWxzZSB7CgkvLyBVc2UgU2VydmVyIENyZWRlbnRpYWwKCXNldCBjcmVkID0gJHtzZXJ2ZXIuY3JlZGVudGlhbH07CglpZiAoJHtjcmVkLmlkfSkgewoJCWVjaG8gIklORk86IFVzaW5nIENyZWRlbnRpYWwgZm9yIFNlcnZlciAke3NlcnZlci5uYW1lfSI7Cgl9Cn0KaWYgKCR7Y3JlZC5pZH0pIHsKCXNldCAtZyBBbnNpYmxlU3NoVXNlciA9ICR7Y3JlZC51c2VybmFtZX07CglzZXQgLWcgQW5zaWJsZVNzaFBhc3N3b3JkID0gJHtjcmVkLnBhc3N3b3JkfTsKfSBlbHNlIHsKCWVjaG8gIklORk86IE5vIFNTSCBjcmVkZW50aWFscyBhdmFpbGFibGUiOwp9CmlmICgkU3Vkb0NyZWRlbnRpYWwgIT0gIiIpIHsKCXNldCBzdWRvY3JlZCA9IGdldGNyZWRlbnRpYWwoJFN1ZG9DcmVkZW50aWFsKTsKCXNldCAtZyBBbnNpYmxlU3Vkb1Bhc3N3b3JkID0gJHtzdWRvY3JlZC5wYXNzd29yZH07Cn0=
</dmscript>
<arguments>
<argument name="SshCredential" type="entry" inpos="1" switchmode="S" switch="undefined" negswitch="undefined" pad="false" required="false" />
<argument name="SudoCredential" type="entry" inpos="2" switchmode="S" switch="undefined" negswitch="undefined" pad="false" required="false" />
</arguments>
<fragment name="ansible_set_credentials" summary="">
<parameter name="SshCredential" type="dropdown" required="N">
<tablename inherit="Y">dm_credentials</tablename>
</parameter>
<parameter name="SudoCredential" type="dropdown" required="N">
<tablename inherit="Y">dm_credentials</tablename>
</parameter>
</fragment>
</action>
