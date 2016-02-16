

#include "module.h"


static int init()
{
//	regn = cw_register_actions_cipwap_ac(&capwap_actions);
	return 1;
}



static struct ac_module module = {
	.name="Cipwap",
	.init= init,
	.detect_by_discovery = 0
	

};




struct ac_module * mod_cipwap()
{

	return &module;
}
