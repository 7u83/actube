#ifndef __DEBUG_H
#define __DEBUG_H

/**
 * Debug levels
 */ 
enum cw_dbg_levels{
	/** Show headers of incomming CAPWAP packets */
	DBG_PKT_IN=0,
	/** Show headers of outgoing CAPWAP packets */
	DBG_PKT_OUT,

	/** Incomming CAPWAP packets with errors, wich would
	    usually silently discarded */ 
	DBG_PKT_ERR,

	/** Dump content of incomming packets */
	DBG_PKT_DMP,

	/** Display incomming CAPWAP/LWAPP messages */
	DBG_MSG_IN,

	/** Display outgoing CAPWAP/LWAPP messages */
	DBG_MSG_OUT,

	/** Message errors */
	DBG_MSG_ERR,

	/** Show message elements  */
	DBG_ELEM,

	/** Show message element details  */
	DBG_ELEM_DETAIL,

	/** Error in msg elements */
	DBG_ELEM_ERR,

	/** Show subelements */
	DBG_SUBELEM,

	/** Show dump of subelements */
	DBG_SUBELEM_DMP,

	/** hex dump elements */	
	DBG_ELEM_DMP,

	/** General infos, like CAPWAP state */
	DBG_INFO,	

	/** Misc. warnings */
	DBG_WARN,

	/** RFC related */
	DBG_RFC,

	/** DTLS related messages */
	DBG_DTLS,

	/** DTLS BIOs in/out */
	DBG_DTLS_BIO,

	/** Dump DTLS BIO i/o */
	DBG_DTLS_BIO_DMP,

	/** Show DTLS Details */
	DBG_DTLS_DETAIL,

	/** Debug Mods */
	DBG_MOD,
	
	DBG_X
};


#define DBG_LN __FILE__,__LINE__


struct dbg_Context{
	int level;
};


void cw_debug( const char * file, int line, struct dbg_Context *ctx, int level,
		const char * format, ...);


#define DBG_START(ctx,level) cw_debug(__FILE__,__LINE__,ctx,level,
#define DBG_END );



#endif
