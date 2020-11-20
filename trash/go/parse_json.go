package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"io"
	"os"
)

var rowKeys2 []string

func testParseJson() {
	inputFile, inputError := os.Open("./dict.json")
	if inputError != nil {
		fmt.Printf("open file error.")
		return
	}
	defer inputFile.Close()

	inputReader := bufio.NewReader(inputFile)
	for {
		inputString, readError := inputReader.ReadString('\n')
		if readError == io.EOF {
			break
		}

		var f interface{}
		// fmt.Println(inputString)
		err := json.Unmarshal([]byte(inputString), &f)
		if err != nil {
			fmt.Printf("Json parse failed.")
			return
		}

		m := f.(map[string]interface{})
		for k, v := range m {
			// fmt.Printf("%s:%s ", k, v)
			if k == "key" {
				vv := v.(string)
				fmt.Println(vv)
				rowKeys2 = append(rowKeys2, vv)
			}
		}
	}

	fmt.Printf("slice len(rowKeys2) %d\n", len(rowKeys2))
	for index := range rowKeys2 {
		fmt.Printf("%s ", rowKeys2[index])
	}

}
