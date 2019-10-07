package main

import "fmt"

func extendSlice(s []int, factor int) []int {
	return s[0 : len(s)*factor]
}

func isEven(s int) bool {
	if s%2 == 0 {
		return true
	}

	return false
}

func filter(s []int, fn func(int) bool) []int {
	var even []int
	for i := range s {
		if fn(s[i]) {
			even = append(even, s[i])
		}
	}

	return even
}

func main() {
	s1 := []byte{'p', 'o', 'e', 'm'}
	s2 := s1[2:]
	fmt.Printf("%c\n", s2[0])
	s2[1] = 't'
	fmt.Printf("%c\n", s1[3])

	fmt.Printf("len(s1[2:2]) is %d\n", len(s1[2:2]))
	fmt.Printf("len(s1[1:2]) is %d\n", len(s1[1:2]))

	fmt.Println("Case1:")
	s := make([]int, 0, 10)
	fmt.Printf("len(s): %d\n", len(s))
	s = extendSlice(s, 2)
	fmt.Printf("factor:2 len(s): %d\n", len(s))
	s = s[0:1]
	fmt.Printf("len(s): %d\n", len(s))
	s = extendSlice(s, 2)
	fmt.Printf("factor:2 len(s): %d\n", len(s))

	s = extendSlice(s, 2)
	fmt.Printf("factor:2 len(s): %d\n", len(s))

	for i := 0; i < 10; i++ {
		s = s[0 : i+1]
		s[i] = i
	}

	fmt.Printf("len(s):%d\n", len(s))
	s3 := s
	s = filter(s, isEven)

	for i := range s {
		fmt.Printf("%d ", s[i])
	}

	fmt.Printf("\n")
	fmt.Printf("len(s):%d\n", len(s))
	fmt.Printf("s[1]:%d s3[1]:%d\n", s[1], s3[1])

	ss := "\u00ff\u754c"
	for i, c := range ss {
		fmt.Printf("%d:%c ", i, c)
	}

	fmt.Println("")
	tst_main()
}

func tst_main() {
	arr1 := [5]int{1, 2, 3, 4, 5}
	slice1 := arr1[1:2]
	slice1 = append(slice1, 6, 7, 8)
	fmt.Println("slice1:", slice1)
	fmt.Println("arr1:", arr1)

	arr2 := [5]int{1, 2, 3, 4, 5}
	slice2 := arr2[1:3]
	slice2 = append(slice2, 6, 7, 8)
	fmt.Println("slice2:", slice2)
	fmt.Println("arr2:", arr2)
}
