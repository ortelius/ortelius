<!-- DeployHub Export File -->
<action name="RunWinShare" summary="" isGraphical="N" category="General">
<kind copy="N">3</kind>
<cmdline>
<script name="$DMHOME/scripts/dh2winshare.py" />
<argument name="WinshareSharename" type="entry" inpos="3" switchmode="S" switch="--winshare_sharename" negswitch="" pad="false" required="true" />
<argument name="WinshareFolder" type="entry" inpos="4" switchmode="S" switch="--winshare_folder" negswitch="" pad="false" required="false" />
<argument name="WinshareUser" type="entry" inpos="1" switchmode="S" switch="--winshare_user" negswitch="" pad="false" required="true" />
<argument name="WinsharePass" type="entry" inpos="2" switchmode="S" switch="--winshare_pass" negswitch="" pad="false" required="true" />
</cmdline>
<fragment name="RunWinShare" summary="">
<parameter name="WinshareUser" type="entry" required="Y"/>
<parameter name="WinsharePass" type="entry" required="Y"/>
<parameter name="WinshareSharename" type="entry" required="Y"/>
<parameter name="WinshareFolder" type="entry" required="N"/>
</fragment>
</action>
