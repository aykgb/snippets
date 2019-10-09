package main

import (
	"fmt"
	"time"
)

func StartTimer(F func()) {
	go func() {
		for {
			F()
			now := time.Now()
			next := now.Add(time.Second * 1)
			fmt.Printf("next.Hour() %d next.Minute() %d next.Second() %d\n", next.Hour(), next.Minute(), next.Second())
			next = time.Date(next.Year(), next.Month(), next.Day(), next.Hour(), next.Minute(), next.Second(), 0, next.Location())
			t := time.NewTimer(next.Sub(time.Now()))
			<-t.C
		}
	}()
}

var cnt int64 = 0

func printNow() {
	cnt++
	fmt.Printf("cnt:%d this is wtf\n", cnt)
}

func testTimedExecution() {
	done := make(chan bool, 1)
	StartTimer(printNow)
	<-done
}
