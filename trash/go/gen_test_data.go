package main

import (
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"strings"
	"time"
)

var line = `{"tablename": "table_A", "fieldvalues": ["field00\t{\"param00\": 111, \"param01\": 222, \"param02\": 333}","field02\t0.022"], "key": "key00", "ttl": "2592000"}`

func GenTestData(lines int) {
	f, _ := os.Create("./input")
	for i := 0; i < lines; i++ {
		rand.Seed(time.Now().UnixNano())
		param00 := rand.Intn(lines)
		target := strings.Replace(line, "111", strconv.Itoa(param00), -1)
		param01 := rand.Intn(lines)
		target = strings.Replace(target, "222", strconv.Itoa(param01), -1)
		param02 := rand.Intn(lines)
		target = strings.Replace(target, "333", strconv.Itoa(param02), -1)
		field02 := rand.Float32()
		target = strings.Replace(target, "0.022", fmt.Sprintf("%3.3f", field02), -1)
		istr := strconv.Itoa(i)
		target = strings.Replace(target, "key00", "test_key_"+istr, -1)
		f.WriteString(target + "\n")
	}
}
