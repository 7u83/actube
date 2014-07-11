#include "wtpdrv.h"


int wtpdrv_get_num_radios()
{
	return 2;
}


/*
static int wtpdrv_get_radioinfo(int rid,struct radioinfo * radioinfo)
{
	struct wpa_driver_ops * drv = wpa_drivers[0];

	struct hostapd_hw_modes * hwm;
	uint16_t nummodes;
	uint16_t fl;

	radioinfo->rid=rid;

	hwm = drv->get_hw_feature_data(drv_priv[rid-1],&nummodes,&fl);

	int i;
	for (i=0; i<nummodes; i++){
		printf ("Mode %d\n",i);
		printf ("------------------------\n");
		printf ("Num channels: %d\n",hwm[i].num_channels);



		printf ("Hwmod: %d\n",hwm[i].mode);

		int mode = hwm[i].mode;
		switch(mode){
			case HOSTAPD_MODE_IEEE80211B:
				radioinfo->type|=CWRADIO_TYPE_B;
				printf("Mode B\n");
				break;
			case HOSTAPD_MODE_IEEE80211G:
				radioinfo->type|=CWRADIO_TYPE_G;
				printf("Mode G\n");
				break;
			case HOSTAPD_MODE_IEEE80211A:
				radioinfo->type|=CWRADIO_TYPE_A;
				printf("Mode A\n");
				break;
			case NUM_HOSTAPD_MODES:
				printf("NIMA AP MODES\n");
				break;

		}
		if (hwm[i].ht_capab)
			radioinfo->type|=CWRADIO_TYPE_N;

		printf ("ht_capab: %d\n",hwm[i].ht_capab);
		printf ("flags: %d\n",hwm[i].flags);
		printf("\n");
	}



//////////////	printf("hem %p\n",hwm);

//	drv
	


}

*/


