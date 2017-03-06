function harvest_messagetable()
{
    set VPMV = 'vp,mv:(.*);([0-9.]*) ?';
    set CP   = 'cp:(.*[^ ]) ?';
    set RMCP = @'rm,cp:(.*)\\\\(.*[^ ]) ?';
    set TO   = 'to:([0-9]*) ?';
    set SU   = 'su:([0-9]*) ?';
    set FA   = 'fa:([0-9]*) ?';
    set NP   = 'np:([0-9]*) ?';

    return {
      /*00020110*/ 'HSRV_I_CHECKOUT'           => { 's1' => $VPMV, 's2' => $RMCP },
      /*00020100*/ 'HSRV_I_CHECKIN'            => { 's1' => $CP,   's2' => $VPMV  },
      /*00020144*/ 'HSRV_I_CHECKIN_NO_CHANGES' => { 's1' => $CP },
      /*00060079*/ 'HAPI_I_CHECKIN_SUMMARY'    => { 'd1' => $TO, 'd2' => $SU, 'd3' => $FA, 'd4' => $NP },
      /*00060080*/ 'HAPI_I_CHECKOUT_SUMMARY'   => { 'd1' => $TO, 'd2' => $SU, 'd3' => $FA, 'd4' => $NP },
      /*000a0080*/ 'CLIN_I_CHECKOUT_SUMMARY'   => { 'd1' => $TO, 'd2' => $SU, 'd3' => $FA, 'd4' => $NP },
      /*02020120*/ 'HSRV_E_CHECKIN_OTHER_PACKAGE' => { 's1' => $CP }
    };
}
