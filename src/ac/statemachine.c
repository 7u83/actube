#include <stdint.h>

#include <mavl.h>

#include "cw/capwap.h"
#include "cw/dbg.h"

#include "statemachine.h"

static cw_StateMachineState_t statemachine_states[];

int cw_statemachine_run(struct cw_StateMachine * mach)
{
	struct mavl * st = cw_statemachine_load_states(NULL,statemachine_states);
	mavl_destroy(st);
	return 0;
}

static cw_StateMachineState_t statemachine_states[]={
	{	CW_STATE_NONE, CAPWAP_STATE_DISCOVERY,		/* transition */
		NULL,0						/* timer */

	}
	,


	{
		CAPWAP_STATE_DTLS_SETUP, CAPWAP_STATE_JOIN,	/* transition */
		"wait-join",CAPWAP_WAIT_JOIN,			/* timer */
		1						/* retval */
	}
	,
	{
		CAPWAP_STATE_JOIN,CAPWAP_STATE_TIMEOUT,
		NULL,0,
		0, "WTP Join timeout"
	}
	,
	{
		CAPWAP_STATE_JOIN, CAPWAP_STATE_JOIN,
		NULL,0,
		1, "WTP has joined"
		
	}
	,
	{
		CAPWAP_STATE_JOIN, CAPWAP_STATE_CONFIGURE,
		"capwap-timers/change-state-pending-timer",CAPWAP_TIMER_CHANGE_STATE_PENDING_TIMER,
		1, NULL
	}
	,
	{
		CAPWAP_STATE_CONFIGURE,CAPWAP_STATE_TIMEOUT,
		NULL,0,
		0, "WTP change state timeout"
	}
	,
	{
		CAPWAP_STATE_CONFIGURE,CAPWAP_STATE_DATA_CHECK,
		"capwap-timers/data-check-timer",CAPWAP_TIMER_DATA_CHECK_TIMER,
		1, NULL
	}
	,
	{
		CAPWAP_STATE_DATA_CHECK,CAPWAP_STATE_TIMEOUT,
		NULL,0,
		0, "WTP data check timeout"
	}
	,
	
	{
		CAPWAP_STATE_DATA_CHECK,CAPWAP_STATE_RUN,
		"capwap-timers/echo-interval",CAPWAP_ECHO_INTERVAL,
		1, NULL
	}
	,
	{
		CAPWAP_STATE_RUN,CAPWAP_STATE_RUN,
		"capwap-timers/echo-interval",CAPWAP_ECHO_INTERVAL,
		1, NULL
	}
	,
	{
		CAPWAP_STATE_RUN,CAPWAP_STATE_TIMEOUT,
		NULL,0,
		0, "WTP echo timout"
	}
	,
	{0,0,0}
};



static int cmp_statemachine_state(const void *state1, const void *state2)
{
        const struct cw_StateMachineState *s1 = state1;
        const struct cw_StateMachineState *s2 = state2;
        int rc;
        rc = s1->state - s2->state;
        if (rc!=0)
                return rc;
        return s1->prevstate-s2->prevstate;
}


struct mavl * cw_statemachine_load_states (struct mavl * statemachine_states, cw_StateMachineState_t * states)
{
        cw_StateMachineState_t * s;
        int replaced;
	cw_dbg(DBG_STATE,"Loading State Machine States");

	if (statemachine_states == NULL){
		statemachine_states = mavl_create(cmp_statemachine_state,NULL,sizeof(cw_StateMachineState_t));
		if (statemachine_states == NULL)
			return NULL;
	}

        s=states;
        while (s->state != 0){
                const char * repstr;
                mavl_replace(statemachine_states,s,&replaced);

                if (replaced){
                        repstr = "Replacing";
                }
                else{
                        repstr = "Adding";
                }
                cw_dbg(DBG_STATE,"     %s: [%s->%s]",repstr,
                        cw_strstate(s->prevstate),
                        cw_strstate(s->state));
                s++;
        }
        return NULL;
}


