package main

import "fmt"

func main() {
	fibonacci(10)
}

func fibonacci(n int) (res int) {
	intra_func := func() func() int {
		a, b := -1, 1
		return func() int {
			a, b = b, a+b
			return b
		}
	}()

	for i := 0; i <= n; i++ {
		fmt.Printf("%d ", intra_func())
	}

	return
}
