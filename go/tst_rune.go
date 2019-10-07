package main

import (
	"fmt"
	"unicode/utf8"
)

func byteSlice(b []byte) []byte {
	return b
}

func runeSlice(r []rune) []rune {
	return r
}

func main() {
	b := []byte{0, 1}
	u8 := []uint8{2, 3}
	fmt.Printf("%T %T \n", b, u8)
	fmt.Println(byteSlice(b))
	fmt.Println(byteSlice(u8))

	r := []rune{4, 5}
	i32 := []int32{6, 7}
	fmt.Printf("%T %T \n", r, i32)
	fmt.Println(runeSlice(r))
	fmt.Println(runeSlice(i32))

	str := "手机尾号1915"
	fmt.Println(len(str))
	l4 := utf8.RuneCountInString(str)
	fmt.Println(l4)
}
