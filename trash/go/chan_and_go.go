package main

import "fmt"
import "sync/atomic"

func test() {
	chanEnd := make(chan bool, 1)
	counter := int32(10)
	for i := 0; i < 10; i++ {
		go func() {
			atomic.AddInt32(&counter, -1)
		}()

	}
	fmt.Println("waiting for loop end.")
	go func() {
		for {
			c := atomic.LoadInt32(&counter)
			if c == 0 {
				fmt.Println("loop end")
				chanEnd <- true
				return
			}
		}
	}()
	<-chanEnd
	fmt.Println("test exit.")
}

func testChanAndGo() {
	test()
	fmt.Println("vim-go")
}
