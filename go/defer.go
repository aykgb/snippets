package main

import "fmt"

func testDefer() {
	function1()

	aa()
	bb()
}

func function1() {
	fmt.Printf("In function1 at the top\n")
	defer function2()
	fmt.Printf("In function1 at the bottom\n")
}

func function2() {
	fmt.Printf("function2: deferred until the end of the calling function!")
}

func aa() {
	i := 0
	defer fmt.Printf("\n%d\n", i)
	i++
	return
}

func bb() {
	for i := 0; i < 5; i++ {
		defer fmt.Printf("%d ", i)
	}
}
