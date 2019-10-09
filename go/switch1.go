package main

import "fmt"

func testSwitch1() {
	var num1 int = 98

	switch num1 {
	case 98, 99:
		fmt.Printf("It's equal to 98 or 99.")
	case 100:
		fmt.Printf("It's equal to 100.")
	default:
		fmt.Printf("It's not equal to 98 99 or 100.")
	}
}
