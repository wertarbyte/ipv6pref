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

#define on_error_return(e, msg, val) { if (e) { perror(msg); return val; } }

static int get_addr_flags(int *add_flags, int *del_flags) {
	char *pref = getenv("IPV6PREF_ADDR");
	if (!pref) {
		return -1;
	} else
	if (strcasecmp(pref, "pub") == 0 ||
	    strcasecmp(pref, "public") == 0) {
		*add_flags = IPV6_PREFER_SRC_PUBLIC;
		*del_flags = ( IPV6_PREFER_SRC_TMP |
		               IPV6_PREFER_SRC_PUBTMP_DEFAULT );
		return 1;
	} else
	if (strcasecmp(pref, "tmp") == 0 ||
	    strcasecmp(pref, "temp") == 0) {
		*add_flags = IPV6_PREFER_SRC_TMP;
		*del_flags = ( IPV6_PREFER_SRC_PUBLIC |
		               IPV6_PREFER_SRC_PUBTMP_DEFAULT );
		return 1;
	}
	return 0;
}

static int modify_addr_preference(int fd, int add_flags, int del_flags) {
	int prefs;
	int err;
	socklen_t len = sizeof(prefs);
	err = getsockopt(fd, IPPROTO_IPV6, IPV6_ADDR_PREFERENCES, &prefs, &len);
	on_error_return(err, "getsockopt()", -1);

	prefs |= add_flags;
	prefs &= ~del_flags;
	err = setsockopt(fd, IPPROTO_IPV6, IPV6_ADDR_PREFERENCES, &prefs, len);
	on_error_return(err, "setsockopt()", -1);

	return 0;
}

int socket(int domain, int type, int protocol) {
	static int (*socket_real)(int, int, int) = NULL;
	if (!socket_real) socket_real = dlsym(RTLD_NEXT, "socket");

	int fd = socket_real(domain, type, protocol);

	if (fd >= 0 && domain == AF_INET6) {
		int add_flags = 0;
		int del_flags = 0;
		if (get_addr_flags(&add_flags, &del_flags) == 1) {
			modify_addr_preference(fd, add_flags, del_flags);
		}
	}
	return fd;
}
