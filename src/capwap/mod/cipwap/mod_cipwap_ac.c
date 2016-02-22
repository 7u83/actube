
#include "capwap/mod.h"
#include "mod_cipwap.h"



int cipwap_init()
{
	return 0;
}


static struct mod_ac cipwap_ac = {
//	.init = cipwap_init()

};

struct mod_ac * mod_cipwap_ac(){
	return &cipwap_ac;
};

