package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"os/signal"
	"syscall"
)

var rowKeys []string
var fidKeys []string
var maxRows = 1000

func readRowKeysFromFile(filename string) (err error) {
	err = nil

	inputFile, inputError := os.Open(filename)
	if inputError != nil {
		fmt.Printf("An error occurred on opening the input file.\n" +
			"Dose the file exists?\n" +
			"Have you got access to it?\n")
		return
	}
	defer func() {
		fmt.Printf("%d keys have read.\n", len(rowKeys))
		inputFile.Close()
	}()

	inputReader := bufio.NewReader(inputFile)
	for i := 0; i < maxRows; i++ {
		inputString, readerError := inputReader.ReadString('\n')
		if readerError == io.EOF {
			return io.EOF
		}

		var f interface{}
		err = json.Unmarshal([]byte(inputString), &f)
		if err != nil {
			fmt.Println("Parse Json failed.")
			return
		}

		m := f.(map[string]interface{})
		for k, v := range m {
			switch vv := v.(type) {
			case string:
				if k == "key" {
					rowKeys = append(rowKeys, vv)
				}
			case []interface{}:
				if k == "fieldvalues" {
					for i, u := range vv {
						fmt.Println(i, u)
					}
				}
				fmt.Println()
			default:
				fmt.Println("Unknow type.")
			}
		}
	}

	return
}

func main() {
	sigs := make(chan os.Signal, 1)
	done := make(chan bool, 1)
	exit := false
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	go func() {
		sig := <-sigs
		fmt.Println()
		fmt.Println(sig)
		done <- true
		exit = true
	}()

	err := readRowKeysFromFile("./gizzle_dict_drunken_model")
	if err != nil && err != io.EOF {
		fmt.Println(err)
		fmt.Printf("%s\n", "read row keys from file failed.")
		return
	}

	go func() {
		for index := range rowKeys {
			if exit == true {
				fmt.Printf("%s\n", "Flow testing exit.")
				return
			}
			var mgetParas []string
			mgetParas = append(mgetParas, rowKeys[index])
			// processMget(mgetParas, proxyClient)
			fmt.Println(rowKeys[index])
		}
	}()

	fmt.Println("awaiting...")
	<-done
	fmt.Println("exit.")
	return
}
