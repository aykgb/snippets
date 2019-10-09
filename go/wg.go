package main

import (
	"fmt"
	"sync"
)

func xRequestLoadTrp(i int) error {
	fmt.Printf("[RequestLoadTrp] %d", i)
	return nil
}

func testWg() {
	for i := 0; i < 3; i++ {
		wg := sync.WaitGroup{}
		wg.Add(10)
		for j := 0; j < 10; j++ {
			go func() {
				fmt.Printf("[=> DEBUG trpvLoad] %dth RequestLoadTrp.\n", j)
				err := xRequestLoadTrp(j)
				if err != nil {
					fmt.Printf("[DEBUG trpvLoad] err:%+v %dth failed to RequestLoadTrp.\n", err, j)
				}
				wg.Done()
			}()

		}
		wg.Wait()
		fmt.Printf("[==> DEBUG trpvLoad] finised RequestLoadTrp.\n")
		break
	}
}
