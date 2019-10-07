cid=0
echo $cid
cid=$((cid + 1))
echo $cid
cid=$((cid + 1))
echo $cid
echo "test_$((cid + 1))"
cid=$((cid + 1))
echo $((cid % 2))
echo $((cid - 1))

for i in 1234 1235 1236 1237 1238; do
    echo $i
done

a=1 b=2 c=3
echo $a $b $c
