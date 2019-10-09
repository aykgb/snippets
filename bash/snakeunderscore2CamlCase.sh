#!/bin/bash
dirname=$1
[ -z $dirname ] && {
    echo "empty dirname"
    exit -1
}

for filename in $(ls $dirname); do
    echo "---"
    echo "filename:"$filename
    target_filename=${filename##tst_}
    echo "target_filename:"$target_filename
    mv $filename $target_filename

    replacement=$(echo $target_filename | gsed -r 's/(^|_)([a-z])/\U\2/g' | gsed -r 's/.go//g')
    replacement="test${replacement}()"
    echo "replacement:"$replacement
    gsed -i "s/main()/$replacement/g" $target_filename
done
