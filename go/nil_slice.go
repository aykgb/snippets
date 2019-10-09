package main

import "fmt"

func testNilSlice() {
	arr := make([]int, 0)
	arr = nil

	fmt.Printf("len(arr)=%d\n", len(arr))

	for item := range arr {
		fmt.Printf("item:%d\n", item)
	}

	return
}
