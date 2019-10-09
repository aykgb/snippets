package main

import "fmt"

func partition(arr []int, begin int, end int) int {
	pivot := arr[begin]
	pos := begin
	for i := begin + 1; i != end; i++ {
		if arr[i] <= pivot {
			pos++
			if i != pos {
				arr[i], arr[pos] = arr[pos], arr[i]
			}
		} else {

		}
	}
	arr[begin], arr[pos] = arr[pos], arr[begin]
	return pos
}

func partition2(arr []int, begin int, end int) int {
	pivot := arr[begin]
	for begin < end {
		for begin < end {
			end--
			if arr[end] >= pivot {
			} else {
				break
			}
		}
		arr[begin] = arr[end]
		for begin < end {
			begin++
			if arr[begin] <= pivot {
			} else {
				break
			}
		}
		arr[end] = arr[begin]
	}
	arr[begin] = pivot
	return begin
}

// keep len(arr) > 2
func sumOf2Max(arr []int) int {
	left := arr[0]
	right := arr[1]
	if left > right {
		left, right = right, left
	}
	for _, v := range arr[2:len(arr)] {
		if v > left && v <= right {
			left = v
		}

		if v > right {
			left = right
			right = v
		}
	}
	fmt.Printf("left:%d , right:%d\n", left, right)

	return left + right
}

func printArr(arr []int) {
	for _, v := range arr {
		fmt.Printf("%d ", v)
	}
	fmt.Printf("\n")
}

func testPartition() {
	arr := []int{21, 40, 16, 32, 9, 51, 10, 34}
	pos := partition(arr, 0, len(arr))
	fmt.Printf("pos: %d\n", pos)
	printArr(arr)

	sumOf2Max(arr)
}
