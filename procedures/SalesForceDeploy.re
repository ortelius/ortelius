<!-- Release Engineer Export File -->
<action name="SalesForceDeploy" summary="" isGraphical="N" category="SalesForce">
<kind copy="N">2</kind>
<dmscript>
dXNpbmcgZHJvcHpvbmUgJ3pvbmUxJyB7CgogICAgY2hlY2tvdXQocmVwb3NpdG9yeTogJFJlcG8pOwoKICAgIHVzaW5nIHN0cmVhbSAkc2ZzdHJlYW0gewogICAgICBlY2hvICc8cHJvamVjdCBuYW1lPSJEZXBsb3lIdWIgU2FsZXNmb3JjZSBEZXBsb3kiIGRlZmF1bHQ9ImRlcGxveUNvZGUiIGJhc2VkaXI9Ii4iIHhtbG5zOnNmPSJhbnRsaWI6Y29tLnNhbGVzZm9yY2UiID4nOwogICAgICBlY2hvICcnOwogICAgICBlY2hvICcgIDx0YXNrZGVmIHJlc291cmNlPSJjb20vc2FsZXNmb3JjZS9hbnRsaWIueG1sIiBjbGFzc1BhdGg9IiR7YW50LmxpYnJhcnkuZGlyfS9hbnQtc2FsZXNmb3JjZS5qYXIiIHVyaT0iYW50bGliOmNvbS5zYWxlc2ZvcmNlIiAvPic7CiAgICAgIGVjaG8gJyc7CQogICAgICBlY2hvICcgIDwhLS0gU2hvd3MgZGVwbG95aW5nIGNvZGUsIHJ1bm5pbmcgYWxsIHRlc3RzLCBhbmQgcnVubmluZyB0ZXN0cyAoMSBvZiB3aGljaCBmYWlscyksIGFuZCBsb2dnaW5nLiAtLT4nOwogICAgICBlY2hvICcgIDx0YXJnZXQgbmFtZT0iZGVwbG95Q29kZSI+JzsKICAgICAgZWNobyAnICA8IS0tIFVwbG9hZCB0aGUgY29udGVudHMgb2YgdGhlICJjb2RlcGtnIiBwYWNrYWdlLCBydW5uaW5nIGFsbCB0ZXN0cyAtLT4nOwogICAgICBlY2hvICIgIDxzZjpkZXBsb3kgdXNlcm5hbWU9XCIkVXNlck5hbWVcIiBwYXNzd29yZD1cIiRQYXNzd29yZFwiIHNlc3Npb25JZD1cIlwiIHNlcnZlcnVybD1cIiRTZXJ2ZXJVUkxcIiBtYXhQb2xsPVwiOTBcIiBwb2xsV2FpdE1pbGxpcz1cIjYwMDAwXCIgZGVwbG95Um9vdD1cImNvZGVwa2dcIiB0ZXN0TGV2ZWw9XCIkVGVzdExldmVsXCIgcm9sbGJhY2tPbkVycm9yPVwidHJ1ZVwiIGxvZ1R5cGU9XCJEZWJ1Z29ubHlcIiAvPiI7CiAgICAgIGVjaG8gJyAgPC90YXJnZXQ+JzsKICAgICAgZWNobyAnIDwvcHJvamVjdD4nOwogICAgfQoKICAgIGNyZWF0ZShmaWxlOiAiYnVpbGQueG1sIiwgc3RyZWFtOiAkc2ZzdHJlYW0pOwogICAgUnVuQW50KEJ1aWxkRmlsZToiJHtkcm9wem9uZS5wYXRofS9idWlsZC54bWwiLFRhcmdldDoiZGVwbG95Q29kZSIpOwp9Cg==
</dmscript>
<arguments>
<argument name="Password" type="entry" inpos="2" switchmode="S" switch="undefined" negswitch="undefined" pad="false" required="true" />
<argument name="Repo" type="entry" inpos="4" switchmode="S" switch="undefined" negswitch="undefined" pad="false" required="true" />
<argument name="ServerURL" type="entry" inpos="3" switchmode="S" switch="undefined" negswitch="undefined" pad="false" required="true" />
<argument name="TestLevel" type="entry" inpos="5" switchmode="S" switch="undefined" negswitch="undefined" pad="false" required="true" />
<argument name="UserName" type="entry" inpos="1" switchmode="S" switch="undefined" negswitch="undefined" pad="false" required="true" />
</arguments>
<fragment name="SalesForceDeploy" summary="">
<parameter name="UserName" type="entry" required="Y"/>
<parameter name="Password" type="entry" required="Y"/>
<parameter name="ServerURL" type="entry" required="Y"/>
<parameter name="Repo" type="entry" required="Y"/>
<parameter name="TestLevel" type="entry" required="Y"/>
</fragment>
</action>
