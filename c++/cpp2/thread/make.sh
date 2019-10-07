#!/bin/sh
# echo $#
# echo $1
# echo $@
filename=$1
prefix=${filename%.*}
suffix=${filename#*.}
if [ "${suffix}" == "c" ]; then
gcc -g -Wall -o $prefix $filename
fi

if [ "${suffix}" == "cpp" ]; then
g++ -g -Wall --std=c++11 -o $prefix $filename -lrt -pthread -lboost_thread -lboost_system
fi

exit 0
