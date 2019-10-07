package main

import "fmt"
import "os"
import "os/signal"
import "syscall"

type Req struct {
	i int
}

var reqChan = make(chan *Req, 1024)
var sigs = make(chan os.Signal, 1)
var done = make(chan bool, 1)

func consumer(id int) {
	for req := range reqChan {
		fmt.Printf("consumer_id:%d Req.i:%d\n", id, req.i)
	}
}

func main() {
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	go func() {
		sig := <-sigs
		fmt.Println(sig)
		done <- true
	}()

	go consumer(0)
	go consumer(1)
	go consumer(2)
	go consumer(3)
	go consumer(4)

	for i := 0; i < 1024; i++ {
		req := &Req{i}
		reqChan <- req
	}

	fmt.Println("await..")
	<-done
	close(reqChan)
	fmt.Println("exit.")
}
