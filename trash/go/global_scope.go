package main

var a = "G"

func testGlobalScope() {
	n()
	m()
	n()
}

func n() { print(a) }
func m() {
	a = "O"
	print(a)
}
