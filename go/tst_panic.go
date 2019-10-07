package main

import "fmt"

type trace struct{}

func main() {
	Example("hello")
}

func Example(str string) (t *trace, err error) {
	defer func() {
		panic("Want stack trace")
	}()

	t = &trace{}

	fmt.Printf("t:%p\n", t)
	return t, err
}
