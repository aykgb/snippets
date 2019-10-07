package main

import (
	"fmt"
	"strings"
)

func main() {
	fmt.Print(strings.TrimLeft("/moss/Order/AddOrder", "/"))
}
