#include "cw.h"
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
	char *ssl_psk,*ssl_cert,*ssl_key;
	uint8_t security;
	
	security = 0;
	
	sprintf(key,"%s/%s",prefix,"ssl-cipher");
	conn->dtls_cipher = cw_ktv_get_str(cfg,key, default_cipher);
	
	sprintf(key,"%s/%s",prefix,"ssl-psk");
	ssl_psk = cw_ktv_get_str(cfg,key,NULL);
	if (ssl_psk != NULL){
		conn->dtls_psk=ssl_psk;
		conn->dtls_psk_len=strlen(ssl_psk);
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

	return security;
}