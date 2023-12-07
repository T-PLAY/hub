#! /bin/bash -e


#echo "start launcher"

# "$@" > "$@".log &
echo background process : "$@"
"$@" &

#echo "end launcher"
