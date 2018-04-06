#!/bin/bash

VERBOSE=$1
DEBUG=$2
shift
shift

FILENAME=
args=( "$@" )
if [ "$DEBUG" = "1" ]; then
    dbg="DEBUG"
else
    dbg="RELEASE"
fi
for i in $(seq ${#args[*]}); do
    arg="${args[$i]}"
    case "$arg" in
        *.c)
            MSG="Compiling ($ARCH, $dbg) $arg"
            ;;
        *.[Ss])
            MSG="Assembling ($ARCH, $dbg) $arg"
            ;;
        *.elf)
            MSG="Linking ($ARCH, $dbg) $arg"
            ;;
        *.[od])
            mkdir -p `dirname "$arg"`
            ;;
        *)
            ;;
    esac
done

if [ $VERBOSE -eq 1 ]; then
    echo $MSG
elif [ $VERBOSE -gt 1 ]; then
    echo "$@"
fi
"$@"
