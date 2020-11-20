package main

import "time"
import "fmt"

func main() {
    ticker := time.NewTicker(1 * time.Second)
    defer ticker.Stop()

    for{
        select {
        case <-ticker.C:
            fmt.Println("tick..")
        }
    }
}
