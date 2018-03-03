#ifndef __MESSAGE_SET_H
#define __MESSAGE_SET_H


struct cw_MsgSet {
	mavl_t messages;
	mavl_t all_elems_by_id;
	mavl_t all_elems_by_key;
	
};

struct cw_ElemDef{
	int id;
	int mand;
	int op;
};

struct cw_ElemData{
	struct cw_ElemHandler *handler;
	int mand;
};

struct cw_ElemHandler {
	const char * name;
	int id;
	int vendor;
	int proto;
	int min_len;
	int max_len;
	const struct mdata_Type * type;
	const char * key;
/*
        int (*start_in)(struct conn *conn,struct cw_action_in *a,uint8_t*data,int len,struct sockaddr *from);
        int (*end_in)(struct conn *conn,struct cw_action_in *a,uint8_t*elem,int len,struct sockaddr *from);
*/
};

struct cw_MsgDef{
	const char * name;
	int type;	/**< Message type */
	int receiver;	/**< Who can receive this message */
	int * states;	/**< states in wich the message is allowed */

	
	struct cw_ElemDef * elements;

};


struct cw_MsgData{
	int type;
	const char * name;
	int * states;
	int receiver;
	mavl_t elements_tree;
	mlist_t elements_list;
};


extern struct cw_MsgSet * cw_msgset_create();

extern void cw_msgset_destroy(struct cw_MsgSet * set);
extern int cw_msgset_add(struct cw_MsgSet * set,
			struct cw_MsgDef messages[], struct cw_ElemHandler elements[]);
mlist_t cw_msgset_get_msg(struct cw_MsgSet * set, int type);




#endif
