#!/bin/sh
LIB=/usr/local/lib/ipv6pref.so

if [ -e "$LIB" ]; then
	export LD_PRELOAD="$LIB"
else
	echo "Unable to preload library '$LIB'" >&2
fi

dbg() {
	[ "${IPV6PREF_DEBUG:-}" ] || return
	echo "ipv6pref:" "$@" >&2
}

WRAPPER="$(basename $0)"

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

exec "$@"
