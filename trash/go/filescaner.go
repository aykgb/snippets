package main

import (
	"bufio"
	"flag"
	"fmt"
	"os"
)

func testFileScanner() {
	filename := flag.String("file", "input.txt", "name of input file")
	flag.Parse()

	file, err := os.Open(*filename)
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	/*
		for scanner.Scan() {
			fmt.Println(scanner.Text())
		}
	*/
	scanner.Scan()
	fmt.Println(scanner.Text())
	scanner.Scan()
	fmt.Println(scanner.Text())

	if err := scanner.Err(); err != nil {
		fmt.Printf("error %+v", err)
	}
}
