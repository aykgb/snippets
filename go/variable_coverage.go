package main

var a1 string

func testVariableCoverage() {
	a1 = "G"
	print(a1)
	f1()
}

func f1() {
	a1 := "O"
	print(a1)
	f2()
}

func f2() {
	print(a1)
}
