package main

import "fmt"

func testFunctionLitreal() {
	f()
}

func f() {
	for i := 0; i < 4; i++ {
		g := func(i int) { fmt.Printf("%d ", i) }
		g(i)
		fmt.Printf(" - g is of type %T and has value %p\n", g, g)
	}
}
