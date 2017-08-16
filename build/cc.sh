#!/bin/sh

VERBOSE=$1
shift

FILENAME=
for arg in "$@"; do
    case "$arg" in
        *.c)
            MSG="Compiling $arg"
            ;;
        *.s)
            MSG="Assembling $arg"
            ;;
        *.elf)
            MSG="Linking $arg"
            ;;
        *.o)
            mkdir -p `dirname "$arg"`
            ;;
        *)
            ;;
    esac
done

if [ $VERBOSE -eq 0 ]; then
    echo $MSG
    "$@" > /dev/null
else
    "$@"
fi
