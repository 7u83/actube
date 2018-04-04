#include "cw.h"



static int get_psk(struct conn * conn,const char * username, uint8_t **psk, unsigned int *len)
{
	char key[CW_KTV_MAX_KEY_LEN];
	cw_KTV_t * result;
	sprintf(key,"%s/%s","psk",username);
	result = cw_ktv_get(conn->local_cfg,key,CW_TYPE_BSTR16);
	if (result == NULL){
		if (conn->dtls_psk != NULL){
			*psk = bstr16_data(conn->dtls_psk);
			*len = bstr16_len(conn->dtls_psk);
			return 1;
		}
		return 0;
	}
		
	if (result == NULL)
		return 0;
	*psk = result->type->data(result);
	*len = result->type->len(result);
	return 1;
}


/**
 * @brief Setup DTLS parameters from config
 * @param conn
 * @param cfg
 * @param prefix
 * @param default_cipher
 * @return 
 */
int cw_setup_dtls(struct conn * conn, mavl_t cfg, const char *prefix, char  * default_cipher)
{
	char key[CW_KTV_MAX_KEY_LEN];
	char *ssl_cert,*ssl_key;
	uint8_t security;
	
	security = 0;
	
	sprintf(key,"%s/%s",prefix,"ssl-cipher");
	conn->dtls_cipher = cw_ktv_get_str(cfg,key, default_cipher);
	
	
	sprintf(key,"%s/%s",prefix,"ssl-psk");
	conn->dtls_psk  = cw_ktv_get_bstr16(cfg,key,NULL);
	
	sprintf(key,"%s/%s",prefix,"ssl-psk-enable");
	conn->dtls_psk_enable = cw_ktv_get_bool(cfg,key,0);
	
	if (conn->dtls_psk_enable ){
		security |= CAPWAP_FLAG_AC_SECURITY_S;
	}
	
		

	sprintf(key,"%s/%s",prefix,"ssl-certfile");
	ssl_cert = cw_ktv_get_str(conn->local_cfg,key,NULL);
	sprintf(key,"%s/%s",prefix,"ssl-keyfile");
	ssl_key = cw_ktv_get_str(conn->local_cfg,key,NULL);
	
	if (ssl_cert != NULL && ssl_key != NULL){
		conn->dtls_cert_file = ssl_cert;
		conn->dtls_key_file = ssl_key;
		sprintf(key,"%s/%s",prefix,"ssl-keypass");
		conn->dtls_key_pass = cw_ktv_get_str(cfg,key,NULL);
		security |= CAPWAP_FLAG_AC_SECURITY_X;
	}

	sprintf(key,"%s/%s",prefix,"ssl-dhbits");
	conn->dtls_dhbits = cw_ktv_get_word(cfg,key,1024);
	
	conn->dtls_get_psk = get_psk;

	return security;
}