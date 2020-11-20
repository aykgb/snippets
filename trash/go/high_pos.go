package main

import (
	"fmt"
	"strconv"
)

func testHighPos() {
	numb := 5678
	numbStr := []byte(strconv.Itoa(numb))
	fmt.Printf("numbStr[0]:%c\n", numbStr[0])
}
