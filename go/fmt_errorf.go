package main

import "fmt"

func main() {
	err := fmt.Errorf("msg = [%s]\n", "this is a error msg.")
	fmt.Println(err)
}
