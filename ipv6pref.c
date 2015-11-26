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

int socket(int domain, int type, int protocol) {
	static int (*socket_real)(int, int, int) = NULL;
	if (!socket_real) socket_real = dlsym(RTLD_NEXT, "socket");

	int fd = socket_real(domain, type, protocol);

	char *pref = getenv("IPV6PREF_ADDR");
	if (fd >= 0 && domain == AF_INET6 && pref) {
		int value = 0;
		if (strcasecmp(pref, "pub") == 0 ||
		    strcasecmp(pref, "public") == 0) {
			value = IPV6_PREFER_SRC_PUBLIC;
		} else
		if (strcasecmp(pref, "temp") == 0 ||
		    strcasecmp(pref, "tmp") == 0) {
			value = IPV6_PREFER_SRC_TMP;
		}
		if (value) {
			setsockopt(fd, IPPROTO_IPV6, IPV6_ADDR_PREFERENCES,
			           &value, sizeof(value));
		}
	}
	return fd;
}
