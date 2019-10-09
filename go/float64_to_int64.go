package main

import (
	"fmt"
	"time"
)

func TestFloat64ToInt64() {
	qps := 1000
	beg := time.Now()
	time.Sleep(time.Millisecond * 2)
	for i := 0; i < 1000; i++ {
	}
	duration := time.Now().Sub(beg)
	fmt.Printf("==> duration:%d\n", int64(duration))
	delta := float64(qps) / float64(time.Second) * float64(duration)
	fmt.Printf("==> delta:%f\n", delta)
	fmt.Printf("==> int64(delta):%d\n", int64(delta))
}
