echo "LINE:$LINENO"
echo "LINE:$LINENO"

{
    echo hello world
    echo "LINE:$LINENO"
    exit
}
echo hello world || echo "LINE:$LINENO"
exit
touch from
cp ./from ./to || echo "LINE:$LINENO"
exit
cp ./from ./to || echo "LINE:$LINENO"
exit

echo "LINE:$LINENO"
echo "LINE:$LINENO"
