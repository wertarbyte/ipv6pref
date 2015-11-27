#!/bin/sh
LIB=/usr/local/lib/ipv6pref.so

if [ -e "$LIB" ]; then
	export LD_PRELOAD="$LIB"
else
	echo "Unable to preload library '$LIB'" >&2
fi

WRAPPER="$(basename $0)"

case "$WRAPPER" in
	v6pub)
		[ "${IPV6PREF_DEBUG:-0}" -eq 1 ] && echo "ipv6pref: Using public address" >&2
		export IPV6PREF_ADDR="pub"
		;;
	v6tmp)
		[ "${IPV6PREF_DEBUG:-0}" -eq 1 ] && echo "ipv6pref: Using temporary address" >&2
		export IPV6PREF_ADDR="tmp"
		;;
esac

exec "$@"
