package main

import (
	"fmt"
	"strings"
)

func main() {
	fmt.Println(strings.Index("SELECT * FROM model_order_bases", "FROM"))
}
