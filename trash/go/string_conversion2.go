package main

import (
	"fmt"
	"strconv"
)

func testStringConversion2() {
	var orig string = "ABC"
	var newS string

	an, err := fmt.Printf("The size of int is: %d\n", strconv.IntSize)
	if err != nil {
		fmt.Printf("The orig %s is not an integer -- existing with error\n", orig)
		return
	}

	fmt.Printf("The integer is %d\n", an)
	an = an + 5
	newS = strconv.Itoa(an)
	fmt.Printf("The new string is: %s\n", newS)
}
