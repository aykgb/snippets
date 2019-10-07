package lib

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"math/rand"
	"reflect"
	"strconv"
	"strings"
	"time"
)

func PaddingZeroForward(aplhaNum []byte, needLen int) (ret []byte) {
	if len(aplhaNum) >= needLen {
		return aplhaNum
	}
	padLen := needLen - len(aplhaNum)
	ret = make([]byte, needLen)
	copy(ret, bytes.Repeat([]byte("0"), padLen))
	copy(ret[padLen:], aplhaNum)

	return
}

func RandPassengerID(pid uint64) uint64 {
	rand.Seed(time.Now().UnixNano())
	return pid ^ uint64(rand.Uint64())>>3
}

func GenRandomInt64() int64 {
	rand.Seed(time.Now().UnixNano())
	return rand.Int63()
}

func GenRandomInt32() int32 {
	rand.Seed(time.Now().UnixNano())
	return rand.Int31()
}

func GetUnixMilliString() string {
	return strconv.FormatUint(uint64(time.Now().UnixNano())/1e6, 10)
}

func GenOrderID(pasID uint64, shardingID int) uint64 {
	unixTimeStampStr := strconv.FormatInt(time.Now().Unix(), 10)

	shardingIDStr := string(
		PaddingZeroForward(
			[]byte(strconv.FormatInt(int64(shardingID), 10)), 3))

	milliSeconStr := GetUnixMilliString()

	passIDRandStr := string(
		PaddingZeroForward(
			[]byte(strconv.FormatUint(RandPassengerID(pasID), 10)), 3))
	/*
		randStr := string(
			PaddingZeroForward(
				[]byte(strconv.FormatInt(GenRandomInt64(), 10)), 2))
	*/

	orderIDStr := unixTimeStampStr + shardingIDStr[0:3] +
		milliSeconStr[len(milliSeconStr)-3:] +
		passIDRandStr[0:3]

	orderID, err := strconv.ParseUint(orderIDStr, 10, 64)
	if err != nil {
		return 0
	}

	return orderID
}

func ExtractTablesFeatures(obj interface{},
	features map[string]map[string]interface{}) error {
	objT := reflect.TypeOf(obj)
	objV := reflect.ValueOf(obj)

	if objT.Kind() == reflect.Ptr {
		objT = objT.Elem()
		objV = objV.Elem()
	}

	for i := 0; i < objT.NumField(); i++ {
		fieldT := objT.Field(i)
		fieldV := objV.Field(i)
		if fieldT.Type.Kind() == reflect.Ptr && fieldV.IsNil() {
			continue
		}
		pTag := fieldT.Tag.Get("orm")
		if pTag == "" && fieldT.Type.Kind() != reflect.Struct {
			continue
		}
		if fieldT.Type.Kind() == reflect.Struct {
			if err := ExtractTablesFeatures(
				fieldV.Interface(), features); err != nil {
				return err
			}
			continue
		}

		tags := strings.Split(pTag, ",")
		if len(tags) < 2 {
			return errors.New("tags less than 2")
		}

		var value interface{}
		if fieldT.Type.Kind() == reflect.Ptr {
			value = fieldV.Elem().Interface()
		} else {
			value = fieldV.Interface()
		}

		for _, t := range tags[1:] {
			if features[t] == nil {
				features[t] = make(map[string]interface{})
			}
			features[t][tags[0]] = value
		}
	}

	return nil
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
		tag := fieldT.Tag.Get("json")
		if tag == "" {
			continue
		}

		if !fieldV.CanSet() {
			continue
		}

		if srcVal, ok := input[tag]; ok {
			if reflect.TypeOf(srcVal).Kind() == fieldT.Type.Kind() {
				fieldV.Set(reflect.ValueOf(srcVal))
			} else {
				errMsg := fmt.Sprintf("src filed:%s type:%v val:%v "+
					"can not set to dst type:%v", tag,
					reflect.TypeOf(srcVal).Kind(), reflect.ValueOf(srcVal),
					fieldT.Type.Kind())
				return errors.New(errMsg)
			}
		}
	}

	return nil
}

func DecodeStruct2Map2(i interface{}, fields []string) (
	o map[string]interface{}, err error) {

	data, e := json.Marshal(i)
	if e != nil {
		return nil, e
	}
	outp := make(map[string]interface{})
	decoder := json.NewDecoder(bytes.NewReader(data))
	decoder.UseNumber()
	if e = decoder.Decode(&outp); e != nil {
		return nil, e
	}

	if len(fields) == 0 {
		return outp, nil
	}

	o = make(map[string]interface{})
	for _, field := range fields {
		if _, ok := outp[field]; ok {
			o[field] = outp[field]
		}
	}

	return o, nil
}

func DecodeStruct2Map(i interface{}, fields []string) (
	o map[string]interface{}, err error) {

	o = make(map[string]interface{})
	if len(fields) == 0 {
		err = decodeStruct2Map(i, o, false)
		return
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
			if fieldV.IsNil() {
				continue
			}
			fieldV = fieldV.Elem()
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

func MakeStructFromMap(vals map[string]interface{}) (interface{}, error) {
	var sfs []reflect.StructField
	for k, v := range vals {
		sf := reflect.StructField{
			Name: fmt.Sprintf("Field_%s", k),
			Type: reflect.TypeOf(v),
			Tag:  reflect.StructTag(fmt.Sprintf("json:\"%s\"", k)),
		}
		sfs = append(sfs, sf)
	}
	st := reflect.StructOf(sfs)
	so := reflect.New(st)

	if e := EncodeMap2Struct(vals, so.Interface()); e != nil {
		return nil, e
	}

	return so.Interface(), nil
}

func GetStructSpec(input interface{}) string {
	var output string
	features, e := DecodeStruct2Map(input, nil)
	if e != nil {
		return ""
	}

	for k, v := range features {
		output += k + ":"
		output += fmt.Sprint(v) + " "
	}
	return output
}
