#ifndef __WINSTORE_H

#define __WINSTORE_H


int verify_host_key(
				const char *hostname,
				int port,
				const char *keytype,
				const char *key );

int lookup_host_key(
				const char *hostname,
				int port,
				const char *keytype,
				char **pkey );

void store_host_key(
				const char *hostname,
				int port,
				const char *keytype,
				const char *key );


#endif /*__WINSTORE_H*/
