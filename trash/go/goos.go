package main

import (
	"fmt"
	"os"
)

func testGoos() {
	var goos string = os.Getenv("GOOS")
	fmt.Printf("Ther operating system is: %s\n", goos)
	path := os.Getenv("PATH")
	fmt.Printf("Path is %s\n", path)
}
