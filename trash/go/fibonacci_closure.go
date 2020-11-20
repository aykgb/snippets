package main

import "fmt"

func fibonacciClosure(n int) (res int) {
	intraFunc := func() func() int {
		a, b := -1, 1
		return func() int {
			a, b = b, a+b
			return b
		}
	}()

	for i := 0; i <= n; i++ {
		fmt.Printf("%d ", intraFunc())
	}

	return
}
