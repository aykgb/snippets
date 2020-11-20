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
#/usr/local/gcc-620/bin/g++ -g -Wall --std=c++11 -o $prefix $filename -lboost_thread -lboost_system -lrt -lpthread
g++ -g -Wall --std=c++11 -o ${prefix}.exe $filename -lboost_thread -lboost_system -lboost_regex -lrt -lpthread
fi

exit 0
