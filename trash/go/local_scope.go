package main

import "fmt"

var aaa = "G"

func testLocalScope() {
	n1()
	m1()
	n1()
}

func n1() { print(aaa) }
func m1() {
	aaa := "O"
	print(aaa)
	fmt.Printf(aaa)
}
