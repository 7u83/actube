#ifndef STATEMACHINE_H
#define STATEMACHINE_H

struct cw_StateMachine{
	uint8_t prevstate,currentstate;

};


struct mavl * cw_statemachine_load_states (struct mavl * statemachine_states, cw_StateMachineState_t * states);
int cw_statemachine_run(struct cw_StateMachine * mach);

#endif
