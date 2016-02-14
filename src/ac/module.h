#ifndef __MODULE_H
#define __MODULE_H

struct ac_module
{
	const char *name;
	int (*init)();
	int (*detect_by_raw)(const char *msg, int len);
	int (*detect_by_discovery)(const char*);
};


#endif
