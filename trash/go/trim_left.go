package main

import (
	"fmt"
	"strings"
)

func testTrimLeft() {
	fmt.Print(strings.TrimLeft("/moss/Order/AddOrder", "/"))
}
