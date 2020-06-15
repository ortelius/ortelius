<!-- Release Engineer Export File -->
<action name="GitCheckout" summary="" isGraphical="N" category="Deploy">
<kind copy="N">3</kind>
<cmdline>
<script name="$DMHOME/scripts/git-checkout" />
<argument name="GitRepo" type="entry" inpos="1" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<argument name="GitCommit" type="entry" inpos="2" switchmode="S" switch="" negswitch="" pad="false" required="true" />
<argument name="ToDirectory" type="entry" inpos="3" switchmode="S" switch="" negswitch="" pad="false" required="true" />
</cmdline>
<fragment name="GitCheckout" summary="">
<parameter name="GitRepo" type="entry" required="Y"/>
<parameter name="GitCommit" type="entry" required="Y"/>
<parameter name="ToDirectory" type="entry" required="Y"/>
</fragment>
</action>
