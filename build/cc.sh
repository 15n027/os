#!/bin/bash

VERBOSE=$1
shift

FILENAME=
args=( "$@" )

for i in $(seq ${#args[*]}); do
    arg="${args[$i]}"
    case "$arg" in
        *.c)
            MSG="Compiling $arg"
            ;;
        *.[Ss])
            MSG="Assembling $arg"
            ;;
        *.elf)
            MSG="Linking $arg"
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
