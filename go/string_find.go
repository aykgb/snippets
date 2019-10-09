package main

import "fmt"
import "strings"

func testStringFind() {
	str := "<!DOCTYPE html>"
	i := strings.Index(str, "html")

	fmt.Println(i)
}
