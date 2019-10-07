package main

import (
	"fmt"
	"reflect"
)

func TestArgs(args ...interface{}) {
	fmt.Println(reflect.TypeOf(args).Elem().Kind())
	fmt.Printf("len:%d\n", len(args))
}

func main() {
	nums := []interface{}{"1111", "2222", "3333"}
	TestArgs(nums)
}
