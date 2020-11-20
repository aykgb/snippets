#! /usr/bin/sh
for f in $(find . -name '*.c' -or -name '*.cpp' -type f)
do
    # exp: astyle --style=stroustrup $f
    astyle --indent=spaces=4 $f
done

# remove '*.orig'
for f in $(find . -name '*.orig' -type f)
do
    rm $f
done
