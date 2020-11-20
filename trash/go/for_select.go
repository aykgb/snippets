package main

import (
	"fmt"
	"time"
)

func testForSelect() {
	i := 0
ForEnd:
	for {
		select {
		case <-time.After(time.Second * 1):
			fmt.Println("case 1 second.")
			i++
			if i > 3 {
				break ForEnd
			}
		}
	}

	fmt.Println("exit.")
}
