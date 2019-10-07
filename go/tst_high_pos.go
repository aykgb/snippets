package main

import (
	"fmt"
	"strconv"
)

func main() {
	numb := 5678
	numbStr := []byte(strconv.Itoa(numb))
	fmt.Printf("numbStr[0]:%s\n", numbStr[0])
}
