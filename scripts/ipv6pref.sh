#!/bin/sh
#
# Wrapper script to advice programs to use the temporary or public IPv6 addresses
#
# Symlink to "v6tmp" and "v6pub" and adjust library location
#
# By Stefan Tomanek <stefan.tomanek@wertarbyte.de>
#    https://github.com/wertarbyte/ipv6pref/

LIB=/usr/local/lib/ipv6pref.so

dbg() {
	[ "${IPV6PREF_DEBUG:-}" ] || return
	echo "ipv6pref:" "$@" >&2
}

if ! [ "${IPV6PREF_WAS_HERE:-}" ]; then
	if [ -e "$LIB" ]; then
		export LD_PRELOAD="$LIB"
	else
		echo "Unable to preload library '$LIB'" >&2
	fi

	WRAPPER="$(basename $0)"
	PREF=""
	case "$WRAPPER" in
		v6pub)
			dbg "Using public address"
			export IPV6PREF_ADDR="pub"
			;;
		v6tmp)
			dbg "Using temporary address"
			export IPV6PREF_ADDR="tmp"
			;;
	esac
fi

IPV6PREF_WAS_HERE=1 exec "$@"
