package main

import (
	"runtime"
	"sync"
)

const (
	maxUsedCPUCores = 48
)

func loop(id int, wg *sync.WaitGroup) {
	var counter int64
	for {
		counter++
	}
}

func testCPUBusy() {
	runtime.GOMAXPROCS(maxUsedCPUCores)

	wg := &sync.WaitGroup{}
	wg.Add(maxUsedCPUCores)

	for i := 0; i < maxUsedCPUCores; i++ {
		go loop(i, wg)
	}

	wg.Wait()
}
