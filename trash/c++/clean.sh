#!/bin/bash
for file in  $(ls | grep -vE ".cpp|.cc|.sh|.hpp|.md|.txt|.h")
do
[ -f $file ] && {
    rm -f $file
}

[ -d $file ] && {
    if [[ "$file" = *\.dSYM ]]; then
        echo $file
        rm -rf $file
    fi
}
done

rm -rf *.dSYM