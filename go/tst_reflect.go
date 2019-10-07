package main

import (
	"errors"
	"fmt"
	"reflect"
	"strings"
)

type Foo struct {
	Id   int
	Name string
	Note *string
}

func snakeString(s string) string {
	data := make([]byte, 0, len(s)*2)
	j := false
	num := len(s)
	for i := 0; i < num; i++ {
		d := s[i]
		if i > 0 && d >= 'A' && d <= 'Z' && j {
			data = append(data, '_')
		}
		if d != '_' {
			j = true
		}
		data = append(data, d)
	}
	return strings.ToLower(string(data[:]))
}

func EncodeMap2Struct(input map[string]interface{}, output interface{}) error {
	outpT := reflect.TypeOf(output)
	outpV := reflect.ValueOf(output)
	if outpT.Kind() == reflect.Ptr {
		outpT = outpT.Elem()
		outpV = outpV.Elem()
	}

	if outpT.Kind() != reflect.Struct {
		return errors.New("output must be a struct type")
	}
	for i := 0; i < outpT.NumField(); i++ {
		fieldT := outpT.Field(i)
		fieldV := outpV.Field(i)

		if fieldV.CanSet() {
			// fieldV = fieldV.Elem()
		} else {
			fmt.Printf("[EncodeMap2Struct] pointer field:%s type:%s "+
				"can not set.\n",
				fieldT.Type.Kind(), fieldV.Type().Kind())
			continue
		}

		fmt.Printf("[EncodeMap2Struct] fieldT.Name:%s fieldV:%v\n",
			fieldT.Name, fieldV.Interface())

		if srcVal, ok := input[snakeString(fieldT.Name)]; ok {
			fmt.Printf("srcVal type kind:%+v, fieldT type kind:%+v\n", reflect.TypeOf(srcVal).Kind(), fieldT.Type.Kind())
			if reflect.TypeOf(srcVal).Kind() == fieldT.Type.Kind() {
				fieldV.Set(reflect.ValueOf(srcVal))
			}
		} else {
			fmt.Printf("fieldT.Name:%s snake name:%s can not be found in map.", fieldT.Name, snakeString(fieldT.Name))
		}
	}

	return nil
}

func main() {
	nt := "write a note"
	foo := &Foo{Id: 111, Name: "wangchenglin", Note: &nt}

	fmt.Printf("foo:%+v", foo)
	fmt.Println(reflect.TypeOf(foo).Kind())
	fmt.Println(reflect.TypeOf(foo).Elem().Kind())

	fooT := reflect.TypeOf(foo).Elem()
	fooV := reflect.ValueOf(foo).Elem()

	fmt.Println("--------")
	for i := 0; i < fooT.NumField(); i++ {
		fieldT := fooT.Field(i)
		fieldV := fooV.Field(i)
		fmt.Println(fieldT)
		fmt.Println(fieldV)
	}
	fmt.Println("--------")

	nt1 := "I'm John Snow"
	feature := make(map[string]interface{})
	feature["id"] = 10
	feature["name"] = "John"
	feature["note"] = nt1

	EncodeMap2Struct(feature, foo)

	fmt.Println("--------")
	fmt.Printf("foo:%+v foo.Note:%s", foo, *foo.Note)

	return
}
