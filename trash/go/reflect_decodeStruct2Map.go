package main

import (
	"errors"
	"fmt"
	"reflect"
	"strconv"
	"time"
)

func DecodeStruct2Map(i interface{}, fields []string) (
	o map[string]interface{}, err error) {

	o = make(map[string]interface{})
	if len(fields) == 0 {
		err = decodeStruct2Map(i, o, false)
	}
	for _, field := range fields {
		o[field] = new(interface{})
	}
	err = decodeStruct2Map(i, o, true)
	return
}

func decodeStruct2Map(
	input interface{}, output map[string]interface{}, modera bool) error {

	inpT := reflect.TypeOf(input)
	inpV := reflect.ValueOf(input)

	if inpT.Kind() == reflect.Ptr {
		inpT = inpT.Elem()
		inpV = inpV.Elem()
	}

	if inpT.Kind() != reflect.Struct {
		return errors.New("input must be a struct type")
	}

	for i := 0; i < inpT.NumField(); i++ {
		fieldT := inpT.Field(i)
		fieldV := inpV.Field(i)
		fieldG := fieldT.Tag.Get("json")
		if fieldG == "" {
			fieldG = "anymous_" + fieldT.Name
		}
		if fieldT.Type.Kind() == reflect.Ptr {
			fieldV = fieldV.Elem()
			if fieldV.IsNil() {
				continue
			}
		}
		switch fieldT.Type.Kind() {
		case reflect.Struct:
			if fieldT.Type.Name() == "Time" {
				tag := fieldT.Tag.Get("json")
				if tag == "" {
					continue
				}
				value := fieldV.Interface().(time.Time)
				if value.IsZero() {
					continue
				}
				if modera {
					if _, ok := output[tag]; ok {
						output[tag] = value.String()
					}
				} else {
					output[tag] = value.String()
				}
			} else {
				if e := decodeStruct2Map(fieldV.Interface(), output, modera); e != nil {
					return e
				}
			}
		case reflect.Slice:
			if reflect.TypeOf(fieldV).Kind() == reflect.Struct {
				s := reflect.ValueOf(fieldV.Interface())
				for i = 0; i < s.Len(); i++ {
					key := fieldG + "_slice_indexof_" + strconv.Itoa(i)
					outpTmp := make(map[string]interface{})
					if e := decodeStruct2Map(s.Index(i).Interface(),
						outpTmp, modera); e != nil {
						return e
					}
					output[key] = outpTmp
				}
			}
		default:
			if modera {
				if _, ok := output[fieldG]; ok {
					output[fieldG] = fieldV.Interface()
				}
			} else {
				output[fieldG] = fieldV.Interface()
			}
		}
	}

	return nil
}

func testReflectDecodeStruct2Map() {
	type UserInfoExtend struct {
		Name   string `json:"name"`
		Age    int    `json:"age"`
		Gender int    `json:"gender"`
		Phone  string `json:"phone"`
	}
	type User struct {
		ID       int    `json:"id"`
		NickName string `json:"name"`
		Extend   UserInfoExtend
	}

	var LRU struct {
		Shard int
		Users []*User
	}

	LRU.Shard = 0
	LRU.Users = make([]*User, 0)
	LRU.Users = append(LRU.Users, &User{ID: 111, NickName: "Vector",
		Extend: UserInfoExtend{
			Name: "WangChenglin", Age: 26, Gender: 1, Phone: "18817391915"}})

	outp := make(map[string]interface{})
	if e := decodeStruct2Map(LRU, outp, false); e != nil {
		fmt.Printf("e:%+v decode struct failed.", e)
		return
	}
	fmt.Printf("outp:%+v", outp)
	return
}
