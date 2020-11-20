package main

import (
	"fmt"
)

const FirstCharOfLine = byte('{')
const LastCharOfLine = byte('}')

func testGzipfileReaderress() {
	gZFileReader := NewGZFileReader("input.gz")
	fileLineReader := NewFileLineReader(gZFileReader)
	for i := 0; i < 100; i++ {
		if fileLineReader.readLineCounter > 3800 {
			i--
		}
		lines, err := fileLineReader.ReadLines(1000)
		if err != nil {
			fmt.Printf("read line from gzip file error %v\n", err)
			return
		}
		if len(lines) == 0 {
			return
		}
		printlines := func(lines []string) {
			for _, line := range lines {
				fmt.Println(line)
			}
		}
		printlines(lines)
	}
}

func testReadlines() {
	// testTableRowParse()
	testGzipfileReaderress()
	return
}
