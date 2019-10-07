package main

import "fmt"
import "syscall"

func tst() (err error) {
	return syscall.EPIPE
}

func main() {
	fmt.Println("vim-go")

	if err := tst(); err == syscall.EPIPE {
		fmt.Println("find EPIPE")
	}

	return
}
