package main

import "fmt"

func Secret() (int, error) {
	return 42, nil
}

func testInside() {
	number := 0

	fmt.Println("before", number) // 0

	// {
	// meet the shadowing
	number, err := Secret()
	if err != nil {
		panic(err)
	}

	fmt.Println("inside", number) // 42
	// }

	fmt.Println("after", number) // 0
}
