#!/bin/sh
#
# Wrapper script to advice programs to use the temporary or public IPv6 addresses
#
# Symlink to "v6tmp" and "v6pub" and adjust library locations
#
# By Stefan Tomanek <stefan.tomanek@wertarbyte.de>
#    https://github.com/wertarbyte/ipv6pref/
#
# You have permission to copy, modify, and redistribute under the
# terms of the GPLv3 or any later version.
# For full license terms, see COPYING.

LIB_NAME="ipv6pref/ipv6pref.so"
# directories to search for the library
LIB_DIRS="/usr/local/lib/ /usr/lib/"

dbg() {
	[ "${IPV6PREF_DEBUG:-}" ] || return
	echo "ipv6pref:" "$@" >&2
}

find_lib() {
	for D in $LIB_DIRS; do
		LIB="${D}/${LIB_NAME}"
		if [ -e "$LIB" ]; then
			echo "$LIB"
			return 0
		fi
	done
	echo "Unable to locate library '$LIB_NAME' in $LIB_DIRS" >&2
	return 1
}

if ! [ "${IPV6PREF_WAS_HERE:-}" ]; then
	# locate the library
	LIB="$(find_lib)"
	if [ "$?" -eq 0 ]; then
		export LD_PRELOAD="${LIB} ${LD_PRELOAD}"
	fi

	# check wrapper name
	WRAPPER="$(basename $0)"
	case "$WRAPPER" in
		v6pub)
			dbg "Using public address by default"
			export IPV6PREF_ADDR="pub"
			;;
		v6tmp)
			dbg "Using temporary address by default"
			export IPV6PREF_ADDR="tmp"
			;;
	esac
	# check for argument
	case "${1:-}" in
		"-p")
			dbg "Using public address"
			export IPV6PREF_ADDR="pub"
			shift
			;;
		"-t")
			dbg "Using temporary address"
			export IPV6PREF_ADDR="tmp"
			shift
			;;
	esac

fi

IPV6PREF_WAS_HERE=1 exec "$@"
