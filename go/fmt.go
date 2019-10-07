package main

import "fmt"

type TableInfo struct {
	tablename  string
	modetype   int64
	buildtype  int64
	switchtype int64
	time       int64
}

func main() {
	tableInfo := &TableInfo{
		tablename:  "table_00",
		modetype:   0,
		buildtype:  1,
		switchtype: 1,
		time:       20180506,
	}

	fmt.Printf("%+v\n", tableInfo)
}
