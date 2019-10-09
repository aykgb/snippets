package main

import (
	"testing"
	"unicode/utf8"
)

func TestCountCountCharAndRune(t *testing.T) {
	str0 := "asSASA ddd dsjkdsjs dk"
	str1 := "asSASA ddd dsjkdsjsこん dk"
	t.Logf("%s\n", str0)
	t.Logf("%s\n", str1)
	// UTF-8

	_, len0 := utf8.DecodeRuneInString(str0)
	_, len1 := utf8.DecodeRuneInString(str1)

	t.Logf("%d\n", len0)
	t.Logf("%d\n", len1)
}
