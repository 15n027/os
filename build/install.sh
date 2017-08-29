#!/bin/bash

VERBOSE="$1"
OBJDIRPREFIX=`realpath -m "$2"`
DESTDIR=`realpath -m "$3"`
shift
shift
shift

DISPDESTDIR=${DESTDIR//$OBJDIRPREFIX}

for target in "$@"; do
    treal=`realpath "$target" || echo "$target"`

    if [ "$VERBOSE" -gt 1 ]; then
        echo install -m 644 -D -t "$DESTDIR" "$target"
    elif [ "$VERBOSE" -eq 1 ]; then
        echo "Installing $treal -> $DISPDESTDIR"
    fi
    install -m 644 -D -t "$DESTDIR" "$target"
done
