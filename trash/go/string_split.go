package main

import "fmt"
import "strings"

func tst_split() {
	strs := "kkkk,kkkkk"
	strs1 := "xxxxxx#bbbbb"

	fmt.Printf("%+v\n", strings.Split(strs, ","))
	fmt.Printf("%+v\n", strings.Split(strs1, "#"))
}

func testStringSplit() {
	var field_keys string = "fid_k0 fid_k1 fid_k2"

	keys := strings.Fields(field_keys)

	for index := range keys {
		fmt.Println(keys[index])
	}

	tst_split()

	return
}
