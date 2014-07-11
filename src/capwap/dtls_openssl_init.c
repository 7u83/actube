


int dtls_openssl_init(struct conn * conn,const SSL_METHOD * method)
{
	conn->dtls_data = malloc(sizeof(struct dtls_data));
	struct dtls_data * dtls_data = conn->dtls_data;
	if (!dtls_data)
		return 0;
	dtls_data->pos=0;
	dtls_data->len=0;


	SSL_CTX *ctx;
	SSL *ssl;
	BIO *bio = BIO_new(&bio_methods);
	bio->ptr = conn;
	printf("Assigning conn %p\n",bio->ptr);

	OpenSSL_add_ssl_algorithms();
	SSL_load_error_strings();

	printf("Getting CTX\n");
	ctx = SSL_CTX_new(method); //DTLSv1_client_method());
	printf("CTX: %p\n",ctx);
	int rc = SSL_CTX_set_cipher_list(ctx, "PSK-AES128-CBC-SHA");
	printf("Set cipher RC: %d\n",rc);
	SSL_CTX_set_psk_client_callback( ctx, psk_cb);
	SSL_CTX_set_psk_server_callback( ctx, psk_server_cb);
	SSL_CTX_set_read_ahead(ctx, 1);
	SSL_CTX_set_cookie_generate_cb(ctx, generate_cookie);
	SSL_CTX_set_cookie_verify_cb(ctx, verify_cookie);

	ssl = SSL_new(ctx);
	SSL_set_bio(ssl, bio, bio);
	printf("New rc %p\n",ssl);
	dtls_data->ssl=ssl;
	return 0;
}


