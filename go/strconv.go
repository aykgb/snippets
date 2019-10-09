package main

import "fmt"
import "strconv"
import "strings"

func testStrconv() {
	sendTime := "07:30:00"
	strs := strings.Split(sendTime, ":")
	hour, err := strconv.Atoi(strs[0])
	if err != nil {
		fmt.Println("failed to parse hour field.")
		return
	}

	min, err := strconv.Atoi(strs[1])
	if err != nil {
		fmt.Println("failed to parse min field.")
		return
	}

	sec, err := strconv.Atoi(strs[2])
	if err != nil {
		fmt.Println("failed to parse sec field.")
		return
	}

	fmt.Printf("hour:%d, min:%d, sec:%d\n", hour, min, sec)
}
