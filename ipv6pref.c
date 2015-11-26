#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
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
	if (fd >= 0 && domain == AF_INET6) {
		int value = IPV6_PREFER_SRC_PUBLIC;
		setsockopt(fd, IPPROTO_IPV6, IPV6_ADDR_PREFERENCES, &value, sizeof(value));
	}
	return fd;
}
