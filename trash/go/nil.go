package main

import "fmt"

type MagicError struct{}

func (MagicError) Error() string {
	return "[Magic]"
}

func Generate() *MagicError {
	return nil
}

func Test() error {
	return Generate()
}

func Test1() []string {
	return nil
}

func testNil() {
	if Test() != nil {
		fmt.Println("Hello, Mr. Pike!")
	}

	err := Test()
	if err != nil {
		fmt.Println("Hello, Mr. Pike!!")
	}

	var tableName []string
	tableName = Test1()
	if tableName == nil {
		fmt.Println("tableName is nil")
	} else {
		fmt.Println("tableName is not nil")
	}
}
