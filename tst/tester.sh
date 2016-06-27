#!/bin/sh
set -e
rm -rf "$2"
cp -r "$1" "$2"
RET="$3"
printf "Expecting output : '%s'\n" "$RET"
shift 3
OUT="$("$@")"
printf "Got output       : '%s'\n" "$OUT"
[ "$OUT" == "$RET" ]
