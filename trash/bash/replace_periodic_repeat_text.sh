# cat 20190715.balance_all_table.json  | jq ".[1169]"
fname=$1
target_host=$2

usage() {
    echo Usage: rebalance.sh {fname} {target_host}
}

if [ -z "$fname" ]; then
    echo paras of [fname] is empty.
    usage
    exit -1
fi

if [ -z "$target_host" ]; then
    echo paras of [target_host] is empty.
    usage
    exit -1
fi

ofname=$fname.json
rm -f $ofname
touch $ofname
echo "[" >>$ofname

flag=0
count=0
myArray=()
cat "$fname" | while read line; do
    if ((count == 0)); then
        ((count++))
        continue
    fi
    # echo $count
    myArray+=($line)
    if [ $((count % 16)) = '7' ]; then
        # echo = $line
        if ! [ "$line" = "\"ChunkAddr\": \"$target_host:7090\"" ]; then
            myArray=()
            ((count++))
            continue
        fi
        flag=1
    fi
    if [ $((count % 16)) = '7']; then
        if [ "$flag" = "1" ]; then
            # echo = ${myArray[@]}
            echo ${myArray[@]} >>$ofname
            flag=0
            myArray=()
        else
            myArray=()
        fi
    fi
    ((count++))
done

last_line=$(sed -n '$p' $ofname)
echo "$last_line"
last_char=${last_line:-1}
echo "$last_char"
if [ "$last_char" = "," ]; then
    sed -i "$ s/.$//" $ofname
fi

echo "]" >>$ofname
