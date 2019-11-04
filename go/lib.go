package main

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"math"
	"math/rand"
	"os"
	"reflect"
	"regexp"
	"strconv"
	"strings"
	"sync/atomic"
	"time"
)

func getHostID(host string) (int64, error) {
	reg := regexp.MustCompile("\\d+")
	if reg == nil {
		return -1, errors.New("MustCompile failed")
	}
	data := reg.Find([]byte(host))
	if data == nil {
		return -1, errors.New("Find nil")
	}
	return strconv.ParseInt(string(data), 10, 64)
}

func GetHostID() (int64, error) {
	host, err := os.Hostname()
	if err != nil {
		return -1, err
	}
	return getHostID(host)
}

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

var incNumber uint64

func init() {
	incNumber = uint64(time.Now().UnixNano() / 1e6)
}

func GetIncNumber() uint64 {
	atomic.AddUint64(&incNumber, 1)
	return incNumber
}

var magicNumber int64 = 0xE89

// 2^53 - 1
var maxOrderIDNumber uint64 = 0x1FFFFFFFFFFFFF

func GenOrderID53() uint64 {
	timeStampStr := strconv.FormatInt(time.Now().Unix()+magicNumber, 10)
	incNumberStr := strconv.Itoa(int(GetIncNumber() % 1000))
	sequenceStr := timeStampStr + incNumberStr

	sequenceNum, e := strconv.ParseUint(sequenceStr, 10, 64)
	if e != nil {
		return 0
	}
	hostID, e := GetHostID()
	if e != nil {
		hostID = 0
	}

	orderID := sequenceNum + uint64(hostID*int64(math.Pow10(len(sequenceStr))))
	if orderID > maxOrderIDNumber {
		return 0
	}
	return orderID
}

func GenOrderID53Second() uint64 {
	timeStampStr := strconv.FormatInt(time.Now().Unix()+magicNumber, 10)

	incNumberStr := string(
		PaddingZeroForward(
			[]byte(strconv.Itoa(int(GetIncNumber()%1000))),
			3))

	hostID, e := GetHostID()
	if e != nil {
		hostID = 0
	}
	hostIDStr := string(
		PaddingZeroForward(
			[]byte(strconv.FormatInt(hostID, 10)),
			3))

	orderIDStr := timeStampStr + hostIDStr + incNumberStr

	orderID, e := strconv.ParseUint(orderIDStr, 10, 53)
	if e != nil {
		return 0
	}
	if orderID > maxOrderIDNumber {
		return 0
	}
	return orderID
}

func GenOrderID64(pasID uint64, shardingID int) uint64 {
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

type ExtendInfo struct {
	Fields map[string]interface{}
}

func NewExtendInfo() *ExtendInfo {
	ei := &ExtendInfo{}
	ei.Fields = make(map[string]interface{})
	return ei
}

func (ei *ExtendInfo) Decode() string {
	extendInfoBytes, _ := json.Marshal(ei.Fields)
	return string(extendInfoBytes)
}

func (ei *ExtendInfo) Encode(extInfoStr string) error {
	if extInfoStr == "" || extInfoStr == "{}" {
		return nil
	}
	return json.Unmarshal([]byte(extInfoStr), &ei.Fields)
}

func (ei *ExtendInfo) MergeEncode(extInfoStr string) error {
	extInfo := NewExtendInfo()
	if e := extInfo.Encode(extInfoStr); e != nil {
		return e
	}
	ei.Merge(extInfo.Fields)
	return nil
}

func (ei *ExtendInfo) Merge(fields map[string]interface{}) {
	for key, val := range fields {
		ei.Fields[key] = val
	}
}

func (ei *ExtendInfo) Get(key string) interface{} {
	return ei.Fields[key]
}

func (ei *ExtendInfo) Update(key string, val interface{}) {
	ei.Fields[key] = val
}

type SqlBuilder struct {
	tbname     string
	sqlRaw     string
	subSQLInit bool
}

func NewSqlBuilder(tbname string) *SqlBuilder {
	s := new(SqlBuilder)
	s.tbname = tbname
	return s
}

func (s *SqlBuilder) Get() string { return s.sqlRaw }

func (s *SqlBuilder) Clone() *SqlBuilder {
	c := NewSqlBuilder(s.tbname)
	c.sqlRaw = s.sqlRaw
	return c
}

func (s *SqlBuilder) AddSelects(fields []string) *SqlBuilder {
	head := "SELECT "
	if len(fields) == 0 {
		head += fmt.Sprintf("* FROM %s ", s.tbname)
		s.sqlRaw = head + s.sqlRaw
		return s
	}
	for idx, field := range fields {
		head += field
		if idx != len(fields)-1 {
			head += ","
		}
	}
	head += fmt.Sprintf(" FROM %s ", s.tbname)
	s.sqlRaw = head + s.sqlRaw
	return s
}

func (s *SqlBuilder) AddSelectDistinctCount(field string, alias string) *SqlBuilder {
	head := ""
	if !strings.Contains(s.sqlRaw, "SELECT") {
		head += fmt.Sprintf("SELECT COUNT(DISTINCT %s) AS %s FROM %s ", field, alias, s.tbname)
		s.sqlRaw = head + s.sqlRaw
		return s
	}
	idxFrom := strings.Index(s.sqlRaw, "FROM")
	s.sqlRaw = s.sqlRaw[:idxFrom] +
		fmt.Sprintf(",COUNT(DISTINCT %s) AS %s ", field, alias) +
		s.sqlRaw[idxFrom:]
	if s.subSQLInit {
		s.sqlRaw = strings.Replace(s.sqlRaw, ",", "", 1)
		s.subSQLInit = false
	}

	return s
}

func (s *SqlBuilder) AddSelectAvg(field string, alias string) *SqlBuilder {
	head := ""
	if !strings.Contains(s.sqlRaw, "SELECT") {
		head += fmt.Sprintf("SELECT AVG(%s) AS %s FROM %s ", field, alias, s.tbname)
		s.sqlRaw = head + s.sqlRaw
		return s
	}
	idxFrom := strings.Index(s.sqlRaw, "FROM")
	s.sqlRaw = s.sqlRaw[:idxFrom] +
		fmt.Sprintf(",AVG(%s) AS %s ", field, alias) +
		s.sqlRaw[idxFrom:]
	if s.subSQLInit {
		s.sqlRaw = strings.Replace(s.sqlRaw, ",", "", 1)
		s.subSQLInit = false
	}

	return s
}

func (s *SqlBuilder) AddSelectSum(field string, alias string) *SqlBuilder {
	head := ""
	if !strings.Contains(s.sqlRaw, "SELECT") {
		head += fmt.Sprintf("SELECT SUM(%s) AS %s FROM %s ", field, alias, s.tbname)
		s.sqlRaw = head + s.sqlRaw
		return s
	}
	idxFrom := strings.Index(s.sqlRaw, "FROM")
	s.sqlRaw = s.sqlRaw[:idxFrom] +
		fmt.Sprintf(",SUM(%s) AS %s ", field, alias) +
		s.sqlRaw[idxFrom:]
	if s.subSQLInit {
		s.sqlRaw = strings.Replace(s.sqlRaw, ",", "", 1)
		s.subSQLInit = false
	}

	return s
}

func (s *SqlBuilder) AddSelectFromSubSQL(subSQL string, alias string) *SqlBuilder {
	s.sqlRaw = fmt.Sprintf("SELECT FROM (%s) AS %s", subSQL, alias)
	s.subSQLInit = true
	return s
}

func (s *SqlBuilder) AddCount() *SqlBuilder {
	head := "SELECT COUNT(*)"
	head += fmt.Sprintf(" FROM %s ", s.tbname)
	s.sqlRaw = head + s.sqlRaw
	return s
}

func (s *SqlBuilder) AddWhereIns(ins map[string][]interface{}) *SqlBuilder {
	for field, arrs := range ins {
		s.AddWhereIn(field, arrs)
	}
	return s
}

func (s *SqlBuilder) AddWhereIn(field string, array []interface{}) *SqlBuilder {
	if len(array) == 0 {
		return s
	}
	if !strings.Contains(s.sqlRaw, "WHERE") {
		s.sqlRaw += "WHERE "
	} else {
		s.sqlRaw += "AND "
	}

	s.sqlRaw += fmt.Sprintf("%s IN ( ", field)
	for i, elem := range array {
		if i < len(array)-1 {
			s.sqlRaw += fmt.Sprintf("%v,", elem)
		} else {
			s.sqlRaw += fmt.Sprintf("%v ) ", elem)
		}
	}
	s.sqlRaw = strings.Replace(s.sqlRaw, "[", "", -1)
	s.sqlRaw = strings.Replace(s.sqlRaw, "]", "", -1)

	return s
}

func (s *SqlBuilder) AddWhereEquals(equals map[string]interface{}) *SqlBuilder {
	if len(equals) == 0 {
		return s
	}
	if !strings.Contains(s.sqlRaw, "WHERE") {
		s.sqlRaw += "WHERE "
	} else {
		s.sqlRaw += "AND "
	}
	cnt := 1
	for key, val := range equals {
		if cnt < len(equals) {
			if reflect.TypeOf(val).Kind() == reflect.String {
				s.sqlRaw += fmt.Sprintf("%s = '%v' AND ", key, val)
			} else {
				s.sqlRaw += fmt.Sprintf("%s = %v AND ", key, val)
			}
		} else {
			if reflect.TypeOf(val).Kind() == reflect.String {
				s.sqlRaw += fmt.Sprintf("%s = '%v' ", key, val)
			} else {
				s.sqlRaw += fmt.Sprintf("%s = %v ", key, val)
			}
		}
		cnt++
	}
	return s
}

func (s *SqlBuilder) AddWhereNotEquals(equals map[string]interface{}) *SqlBuilder {
	if len(equals) == 0 {
		return s
	}
	if !strings.Contains(s.sqlRaw, "WHERE") {
		s.sqlRaw += "WHERE "
	} else {
		s.sqlRaw += "AND "
	}
	cnt := 1
	for key, val := range equals {
		if cnt < len(equals) {
			if reflect.TypeOf(val).Kind() == reflect.String {
				s.sqlRaw += fmt.Sprintf("%s != '%v' AND ", key, val)
			} else {
				s.sqlRaw += fmt.Sprintf("%s != %v AND ", key, val)
			}
		} else {
			if reflect.TypeOf(val).Kind() == reflect.String {
				s.sqlRaw += fmt.Sprintf("%s != '%v' ", key, val)
			} else {
				s.sqlRaw += fmt.Sprintf("%s != %v ", key, val)
			}
		}
		cnt++
	}
	return s
}

func (s *SqlBuilder) AddWhereRanges(ranges map[string][]interface{}) *SqlBuilder {
	for field, interval := range ranges {
		s.AddWhereBetween(field, interval)
	}
	return s
}

func (s *SqlBuilder) AddWhereBetween(field string, interval []interface{}) *SqlBuilder {
	if len(interval) != 2 {
		return s
	}
	if !strings.Contains(s.sqlRaw, "WHERE") {
		s.sqlRaw += "WHERE "
	} else {
		s.sqlRaw += "AND "
	}
	s.sqlRaw += fmt.Sprintf("%s BETWEEN '%v' AND '%v' ",
		field, interval[0], interval[1])

	return s
}

func (s *SqlBuilder) AddWhereLikesAnd(likes map[string]string) *SqlBuilder {
	if len(likes) == 0 {
		return s
	}
	if !strings.Contains(s.sqlRaw, "WHERE") {
		s.sqlRaw += "WHERE "
	} else {
		s.sqlRaw += "AND "
	}
	cnt := 1
	for key, val := range likes {
		if cnt < len(likes) {
			s.sqlRaw += fmt.Sprintf("%s LIKE \"%%%s%%\" AND ", key, val)
		} else {
			s.sqlRaw += fmt.Sprintf("%s LIKE \"%%%s%%\" ", key, val)
		}
		cnt++
	}
	return s
}

func (s *SqlBuilder) AddWhereLikesOR(likes map[string]string) *SqlBuilder {
	if len(likes) == 0 {
		return s
	}
	if !strings.Contains(s.sqlRaw, "WHERE") {
		s.sqlRaw += "WHERE "
	} else {
		s.sqlRaw += "AND "
	}
	cnt := 1
	for key, val := range likes {
		if cnt == 1 {
			s.sqlRaw += fmt.Sprintf("( %s LIKE \"%%%s%%\" OR ", key, val)
		} else if cnt < len(likes) {
			s.sqlRaw += fmt.Sprintf("%s LIKE \"%%%s%%\" OR ", key, val)
		} else {
			s.sqlRaw += fmt.Sprintf("%s LIKE \"%%%s%%\") ", key, val)
		}
		cnt++
	}
	return s
}

func (s *SqlBuilder) AddGroupBy(field string) *SqlBuilder {
	if !strings.Contains(s.sqlRaw, "GROUP") {
		s.sqlRaw += fmt.Sprintf("GROUP BY %s", field)
	} else {
		s.sqlRaw += fmt.Sprintf(",%s", field)
	}
	return s
}

func (s *SqlBuilder) AddOrderBy(sorts map[string]bool) *SqlBuilder {
	if len(sorts) == 0 {
		return s
	}
	if !strings.Contains(s.sqlRaw, "ORDER") {
		s.sqlRaw += "ORDER BY "
	}
	// 增加排序语句
	count := 1
	for sortField, desc := range sorts {
		if count < len(sorts) {
			if desc {
				s.sqlRaw += sortField + " DESC, "
			} else {
				s.sqlRaw += ", "
			}
		} else {
			if desc {
				s.sqlRaw += sortField + " DESC "
			} else {
				s.sqlRaw += " "
			}
		}
	}
	return s
}

func (s *SqlBuilder) AddLimitOffset(limit, offset int) *SqlBuilder {
	if offset < 0 {
		s.sqlRaw += fmt.Sprintf("LIMIT %d ", limit)
	} else {
		s.sqlRaw += fmt.Sprintf("LIMIT %d OFFSET %d ", limit, offset)
	}
	return s
}
