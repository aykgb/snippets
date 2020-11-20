function00() {
    for i in 0 1 2 3 4
    do
        echo $i
    done

    echo "para 0:${0}"
    echo "para 1:${1}"
    echo "para 2:${2}"
}

echo exec func
function00 x y
echo end
