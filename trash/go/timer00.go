package main

import "fmt"
import "time"

var duration = 1 // second

func task(seq int64, done chan bool) {
	if seq == 10 {
		done <- true
	}
	time.Sleep(time.Duration(duration) * time.Second)
	go task(seq+1, done)
	fmt.Printf("## seq:%d this task exit...\n", seq)
}

func testTimer00() {
	done := make(chan bool, 1)
	go task(0, done)

	fmt.Println("awaiting task done.")
	<-done
	fmt.Println("task done exiting.")
}
