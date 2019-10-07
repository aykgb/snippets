package main

import "fmt"
import "unicode/utf8"

func main() {
	str0 := "asSASA ddd dsjkdsjs dk"
	str1 := "asSASA ddd dsjkdsjsこん dk"
	fmt.Printf("%s\n", str0)
	fmt.Printf("%s\n", str1)

	_, len0 := utf8.DecodeRuneInString(str0)
	_, len1 := utf8.DecodeRuneInString(str1)

	fmt.Printf("%d\n", len0)
	fmt.Printf("%d\n", len1)
}
