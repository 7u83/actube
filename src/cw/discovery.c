
#include "discovery.h"

#include "dbg.h"

static int cmp(const void *v1, const void *v2)
{
	int rc;
	struct cw_DiscoveryResults_elem *e1,*e2;
	e1=(struct cw_DiscoveryResults_elem *)v1;
	e2=(struct cw_DiscoveryResults_elem *)v2;

	rc = e1->prio - e2->prio;
	if (rc !=  0)
		return rc;
	rc = e1->wtps - e2->wtps;
	if (rc !=  0)
		return rc;

	return e1->ctr-e2->ctr;
}

static void del(void *r)
{
	struct cw_DiscoveryResults_elem *e = (struct cw_DiscoveryResults_elem *)r;
	cw_cfg_destroy(e->cfg);
}

struct cw_DiscoveryResults *cw_discovery_results_create()
{
	struct cw_DiscoveryResults *dis;

	dis = malloc(sizeof(struct cw_DiscoveryResults));
	if (dis == NULL)
		return NULL;

	dis->nr = 0;
	dis->list =
	    mavl_create(cmp, del, sizeof(struct cw_DiscoveryResults_elem));
	if (dis->list == NULL) {
		cw_discovery_results_destroy(dis);
		return NULL;
	}
	return dis;
}

void cw_discovery_results_destroy(struct cw_DiscoveryResults *r)
{
	if (r->list)
		mavl_destroy(r->list);
	free(r);
}

void cw_discovery_results_add(struct cw_DiscoveryResults *dis,
			      cw_Cfg_t * ac_cfg, cw_Cfg_t * wtp_cfg)
{

	const char *acname;
	char key[CW_CFG_MAX_KEY_LEN];
	struct cw_DiscoveryResults_elem e;
	int i;

	/* get ac name */
	acname = cw_cfg_get(ac_cfg, "capwap/ac-name", NULL);
	if (acname == NULL) {
		/* this should not happen, because AC Name is a
		 * amndatory message element */
		e.prio = 255;
	} else {
		/* Get priority for AC from 
		 * ac-name-with-priority list */
		sprintf(key, "ac-name-with-priority/%s", acname);
		e.prio = cw_cfg_get_byte(wtp_cfg, key, 255);
	}

	/* for each control ip address the AC has sent */
	i = 0;
	do {
		const char *ipval;

		sprintf(key, "capwap-control-ip-address.%d/wtps", i);
		e.wtps = cw_cfg_get_word(ac_cfg, key, 65535);


//		cw_dbg(DBG_X, "WTPS %d, K: %s", e.wtps);
		if (e.wtps == 65535)
			break;

		sprintf(key, "capwap-control-ip-address.%d/address", i);
		ipval = cw_cfg_get(ac_cfg, key, "");

		sprintf(key, "%04d%05d%04d", e.prio, e.wtps, dis->nr);

		e.ctr=dis->list->count;

		i++;
		e.cfg = cw_cfg_create();
		if (e.cfg == NULL)
			continue;

		cw_cfg_copy(ac_cfg,e.cfg);
		
		strcpy(e.ip,ipval);

		mavl_insert(dis->list,&e,NULL);

		
	} while (i < 255);
	dis->nr++;

}



