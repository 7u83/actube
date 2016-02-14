

#include "module.h"


static int init()
{
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
