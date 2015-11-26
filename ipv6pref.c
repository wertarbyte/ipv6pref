#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/ipv6.h>

#include <linux/in.h>
#include <linux/in6.h>

#define __USE_GNU
#include <dlfcn.h>

static int get_addr_preference(void) {
	char *pref = getenv("IPV6PREF_ADDR");
	int v = 0;
	if (!pref) {
		v = -1;
	} else
	if (strcasecmp(pref, "pub") == 0 ||
	    strcasecmp(pref, "public") == 0) {
		v = IPV6_PREFER_SRC_PUBLIC;
	} else
	if (strcasecmp(pref, "temp") == 0 ||
	    strcasecmp(pref, "tmp") == 0) {
		v = IPV6_PREFER_SRC_TMP;
	}
	return v;
}

int socket(int domain, int type, int protocol) {
	static int (*socket_real)(int, int, int) = NULL;
	if (!socket_real) socket_real = dlsym(RTLD_NEXT, "socket");

	int fd = socket_real(domain, type, protocol);

	if (fd >= 0 && domain == AF_INET6) {
		int pref = get_addr_preference();
		if (pref > 0) {
			setsockopt(fd, IPPROTO_IPV6, IPV6_ADDR_PREFERENCES,
			           &pref, sizeof(pref));
		}
	}
	return fd;
}
