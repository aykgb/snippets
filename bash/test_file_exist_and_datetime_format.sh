time=$(date '+%Y%m%d')
mkdir -p backup_data
src_file=local_data/some_data.gz
dst_file=backup_data/some_data.${time}.gz
[ ! -f $src_file ] && {
    echo "$time $src_file not exist"
    exit -1
}
mv $src_file $dst_file
