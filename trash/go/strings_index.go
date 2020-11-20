package main

import (
	"fmt"
	"strings"
)

func testStringsIndex() {
	fmt.Println(strings.Index("SELECT * FROM model_order_bases", "FROM"))
}
