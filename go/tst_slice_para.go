package main

import "fmt"

type Element struct {
	id int
}

func tst(e []*Element) []*Element {
	e = append(e, &Element{id: 0})
	e = append(e, &Element{id: 1})
	e = append(e, &Element{id: 2})
	e = append(e, &Element{id: 3})
	return e
}

func main() {
	ele := make([]*Element, 0)
	fmt.Println(len(ele))
	ele = tst(ele)
	fmt.Println(len(ele))
}
