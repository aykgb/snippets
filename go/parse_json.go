package main

import "io"
import "os"
import "fmt"
import "encoding/json"
import "bufio"

var rowKeys []string

func main() {
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
				rowKeys = append(rowKeys, vv)
			}
		}
	}

	fmt.Printf("slice len(rowKeys) %d\n", len(rowKeys))
	for index := range rowKeys {
		fmt.Printf("%s ", rowKeys[index])
	}

}
