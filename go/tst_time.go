package main

import (
	"fmt"
	"time"
)

func main() {
	t := time.Now()
	fmt.Println(t)

	layout := "5:00PM"
	t1, err := time.Parse(layout, "8:00AM")
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println(t1)
	}
}
