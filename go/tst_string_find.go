package main

import "fmt"
import "strings"

func main() {
	str := "<!DOCTYPE html>"
	i := strings.Index(str, "html")

	fmt.Println(i)
}
