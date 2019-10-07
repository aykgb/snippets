package main

import "fmt"

func main() {
	arrInt := make([]int, 10)
	arrInt = append(arrInt, 1)

	fmt.Printf("len %d, arrInt[0] %d, arrInt[len -1] %d\n", len(arrInt), arrInt[0], arrInt[len(arrInt)-1])
}
