array=("./tst" "./tst0")
for ele in ${array[@]}
do
    echo $ele
done
sleep 1
echo ${array[0]}
