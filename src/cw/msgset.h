#ifndef __MESSAGE_SET_H
#define __MESSAGE_SET_H

#include "mlist.h"
#include "sock.h"
#include "mavl.h"

#include "val.h"
#include "cfg.h"

struct cw_ElemDef{
	int proto;
	int vendor;
	int id;
	int mand;
	int op;
};

struct cw_ElemData{
	int proto;
	int vendor;
	int id;
	int mand;
};

struct cw_ElemHandlerParams {
	struct cw_Conn * conn;  /**< a connection the message belongs to*/
	struct cw_MsgSet * msgset;
	struct cw_MsgData * msgdata; 
	struct cw_ElemData * elemdata;
	struct sockaddr *from;
	mavl_t mand_found;
	mlist_t unrecognized;
//	cw_Val_t * elem;
	char * debug_details;
	uint32_t dbg_level;
	cw_Cfg_t * cfg;
	cw_Cfg_t * cfg_list[10];
};



struct cw_ElemHandler {
	const char * name;
	int id;
	int vendor;
	int proto;
	int min_len;
	int max_len;
	/*const struct cw_Type * type;*/
	const void * type;
	const char * key;
        int (*get)(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params, 
		uint8_t*data, int len);
		
	int (*put)(struct cw_ElemHandler * handler, struct cw_ElemHandlerParams * params, uint8_t * dst);

	int (*mkkey)(const char *pkey, uint8_t*data, int len, char *dst);
	int (*patch)(uint8_t *dst, void *data );
	void * param;
	uint8_t flags;
};


struct cw_MsgSet {
	mavl_t msgdata;
	mavl_t handlers_by_id;
	mavl_t handlers_by_key;
//	mavl_t types_tree;
	mavl_t statemachine_states;
	int (*write_header)(struct cw_ElemHandler * handler, uint8_t * dst, int len);
	int (*header_len)(struct cw_ElemHandler *handler);
};



struct cw_State{
	uint8_t state;
	uint8_t next;
};
typedef struct cw_State cw_State_t;

struct cw_MsgDef{
	const char * name;
	int type;	/**< Message type */
	int receiver;	/**< Who can receive this message */
	cw_State_t * states;	/**< states in wich the message is allowed */ 

	
	struct cw_ElemDef * elements;
	int (*preprocess)(struct cw_Conn * conn);
	int (*postprocess)(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len);

/*	uint8_t next_state;*/
};




struct cw_MsgData{
	int type;
	const char * name;
	cw_State_t * states;
	int receiver;
	mavl_t elements_tree;
	mlist_t elements_list;
	mlist_t mand_keys;  	/**< Keys of mandatory elements */

	int (*preprocess)(struct cw_Conn * conn);
	int (*postprocess)(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len);
/*	uint8_t next_state;*/
};


struct cw_StateMachineState{
	uint8_t prevstate;
	uint8_t state;
	const char * timer_key;
	int timer_default;
	int retval;
	const char *dbgmsg;
	uint8_t jump_prevstate;
	uint8_t jump_state;


};
typedef struct cw_StateMachineState cw_StateMachineState_t;

extern struct cw_MsgSet * cw_msgset_create();

void cw_msgset_destroy(struct cw_MsgSet * set);
int cw_msgset_add(struct cw_MsgSet * set,
			struct cw_MsgDef messages[], struct cw_ElemHandler handlers[]);
			
int cw_msgset_add_states(struct cw_MsgSet * set, cw_StateMachineState_t * states);

/*mlist_t cw_msgset_get_msg(struct cw_MsgSet * set, int type);*/

struct cw_MsgData * cw_msgset_get_msgdata(struct cw_MsgSet *set,int type);
struct cw_ElemHandler * cw_msgset_get_elemhandler(struct cw_MsgSet * set,
		int proto, int vendor, int id);


#define CW_MSGSET_POSTPROCESS	1
#define CW_MSGSET_PREPROCESS	2

typedef int (*cw_MsgCallbackFun)(struct cw_ElemHandlerParams * params, uint8_t * elems_ptr, int elems_len);
cw_MsgCallbackFun cw_msgset_set_postprocess(struct cw_MsgSet * set,int msg_id,
	cw_MsgCallbackFun fun);


#endif
