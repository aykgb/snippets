package main

import "fmt"

func testSwitch2() {
	var num1 int = 7

	switch {
	case num1 < 0:
		fmt.Printf("Number is negative.")
	case num1 > 0 && num1 < 10:
		fmt.Printf("Number is between 0 and 10.")
	default:
		fmt.Printf("Number is 10 or greater.")
	}
}
