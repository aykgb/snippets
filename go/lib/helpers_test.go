package lib

import (
	"common"
	"encoding/json"
	"reflect"
	"testing"
)

func TestGenOrderID(t *testing.T) {
	type args struct {
		pasID      uint64
		shardingID int
	}
	tests := []struct {
		name string
		args args
		want string
	}{
		// TODO: Add test cases.
		{name: "normal", args: args{12345, 1}, want: "0"},
		{name: "normal", args: args{12345, 1}, want: "0"},
		{name: "normal", args: args{12345, 1}, want: "0"},
		{name: "normal", args: args{12345, 1}, want: "0"},
		{name: "normal", args: args{12345, 1}, want: "0"},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			t.Log(GenOrderID(tt.args.pasID, tt.args.shardingID))
		})
	}
}

func TestGetProduct(t *testing.T) {
	type args struct {
		productId int
	}
	tests := []struct {
		name string
		args args
		want string
	}{
		// TODO: Add test cases.
		{name: "normal", args: args{10}, want: ""},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			t.Logf(common.GetProduct(tt.args.productId))
		})
	}
}

func TestPaddingZeroForward(t *testing.T) {
	type args struct {
		aplhaNum []byte
		needLen  int
	}
	tests := []struct {
		name    string
		args    args
		wantRet []byte
	}{
		// TODO: Add test cases.
		{name: "normal", args: args{[]byte("12"), 3}, wantRet: []byte("012")},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if gotRet := PaddingZeroForward(tt.args.aplhaNum, tt.args.needLen); !reflect.DeepEqual(gotRet, tt.wantRet) {
				t.Errorf("PaddingZeroForward() = %v, want %v", gotRet, tt.wantRet)
			}
		})
	}
}

func TestRandPassengerID(t *testing.T) {
	type args struct {
		pid uint64
	}
	tests := []struct {
		name string
		args args
		want uint64
	}{
		// TODO: Add test cases.
		{"normal", args{1234}, 0},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			t.Log(RandPassengerID(tt.args.pid))
		})
	}
}

type RequestAddOrder struct {
	ProductID int          `json:"product_id" valid:"required" orm:"product_id,base"` // 产品线ID
	OrderInfo OrderInfoNew `json:"order_info" valid:"required"`                       // 新建订单信息
}

type OrderInfoNew struct {
	BubbleID             string   `json:"bubble_id" valid:"required" orm:"bubble_id,base"`                           // 冒泡ID
	RouteID              *uint64  `json:"route_id" valid:"required" orm:"route_id,base"`                             // 线路ID
	CompanyID            *int     `json:"company_id" valid:"required" orm:"company_id,base"`                         // 客企ID
	CompanyName          *string  `json:"company_name" valid:"required" orm:"company_name,base"`                     // 客企名称
	PassengerID          *uint64  `json:"passenger_id" valid:"required" orm:"passenger_id,base"`                     // 乘客ID
	PassengerPhone       *string  `json:"passenger_phone" valid:"required,alphanum" orm:"passenger_phone,base"`      // 乘客手机号
	Type                 int8     `json:"type" valid:"required" orm:"type,base"`                                     // 订单类型，0：预约，1：实时
	SourceType           int8     `json:"source_type" valid:"required" orm:"source_type,base"`                       // 订单来源
	City                 *int     `json:"city" valid:"required" orm:"city,base"`                                     // 出发城市
	ToCity               *int     `json:"to_city" valid:"required" orm:"to_city,base"`                               // 目的城市
	FromName             *string  `json:"fname" valid:"required" orm:"fname,base"`                                   // 出发地名称
	FromLng              *float64 `json:"flng" valid:"required" orm:"flng,base"`                                     // 出发地经度
	FromLat              *float64 `json:"flat" valid:"required" orm:"flat,base"`                                     // 出发地维度
	ToName               *string  `json:"tname" valid:"required" orm:"tname,base"`                                   // 目的地名称
	ToLng                *float64 `json:"tlng" valid:"required" orm:"tlng,base"`                                     // 目的地经度
	ToLat                *float64 `json:"tlat" valid:"required" orm:"tlat,base"`                                     // 目的地维度
	AppointmentBeginTime *uint64  `json:"appointment_begin_time" valid:"required" orm:"appointment_begin_time,base"` // 乘客预约出发起始时间
	AppointmentEndTime   *uint64  `json:"appointment_end_time" valid:"required" orm:"appointment_end_time,base"`     // 乘客预约出发最晚时间
	Capacity             int8     `json:"capacity" valid:"required" orm:"capacity,base"`                             // 订单座位数
	TotalPrice           int32    `json:"total_price" valid:"required" orm:"total_price,base"`                       // 订单总价，单位分
	ExtendInfo           *string  `json:"extend_info" valid:"optional,json" orm:"extend_info,base"`                  // 扩展字段，json格式
}

var reqJSON = `
{
	"product_id":10,
	"order_info":{
		"bubble_id":"11111",
		"route_id":33333,
		"company_id":44444,
		"company_name":"xiaojukeji",
		"passenger_id":11111,
		"passenger_phone":"18817391915",
		"type":1,
		"source_type":1,
		"city":100,
		"to_city":200,
		"fname":"shanghai",
		"flng":116.7187367605,
		"flat":39.7071806231,
	    "tname":"beijing",
		"tlng":121.4648585865,
		"tlat":30.9399455508,
		"appointment_begin_time":1555048800,
		"appointment_end_time":1555052400,
		"capacity":1,
		"total_price":4230,
		"extend_info":"{}"
	}
}`

func BenchmarkDecodeStruct2Map(b *testing.B) {
	req := new(RequestAddOrder)
	if e := json.Unmarshal([]byte(reqJSON), req); e != nil {
		b.Errorf("error:%+v", e)
	}
	for i := 0; i < b.N; i++ {
		DecodeStruct2Map(req, nil)
	}
}

func BenchmarkDecodeStruct2MapWithJson(b *testing.B) {
	req := new(RequestAddOrder)
	if e := json.Unmarshal([]byte(reqJSON), req); e != nil {
		b.Errorf("error:%+v", e)
	}
	for i := 0; i < b.N; i++ {
		data, e := json.Marshal(req)
		if e != nil {
			b.Errorf("error:%+v", e)
		}
		res := make(map[string]interface{})
		if e = json.Unmarshal(data, &res); e != nil {
			b.Errorf("error:%+v", e)
		}
	}
}

func BenchmarkEncodeMap2Struct(b *testing.B) {
	req := new(RequestAddOrder)
	out := make(map[string]interface{})
	if e := json.Unmarshal([]byte(reqJSON), &out); e != nil {
		b.Errorf("error:%+v", e)
	}
	for i := 0; i < b.N; i++ {
		EncodeMap2Struct(out, req)
	}
}

func BenchmarkEncodeMap2StructWithJson(b *testing.B) {
	req := new(RequestAddOrder)
	inp := make(map[string]interface{})
	if e := json.Unmarshal([]byte(reqJSON), &inp); e != nil {
		b.Errorf("error:%+v", e)
	}
	for i := 0; i < b.N; i++ {
		data, e := json.Marshal(inp)
		if e != nil {
			b.Errorf("e:%+v", e)
		}
		if e = json.Unmarshal(data, req); e != nil {
			b.Errorf("e:%+v", e)
		}
	}
}

func BenchmarkMakeStructFromMap(b *testing.B) {
	inp := make(map[string]interface{})
	if e := json.Unmarshal([]byte(reqJSON), &inp); e != nil {
		b.Errorf("error:%+v", e)
	}
	for i := 0; i < b.N; i++ {
		MakeStructFromMap(inp)
	}
}

/*
# go test -v -bench=. -benchmem -run=none
goos: darwin
goarch: amd64
pkg: lib
BenchmarkDecodeStruct2Map-4               200000              5504 ns/op            2747 B/op         45 allocs/op
BenchmarkDecodeStruct2MapWithJson-4       100000             12781 ns/op            4088 B/op         81 allocs/op
BenchmarkEncodeMap2Struct-4              2000000               943 ns/op             168 B/op          7 allocs/op
BenchmarkEncodeMap2StructWithJson-4       100000             21968 ns/op            3569 B/op         81 allocs/op
BenchmarkMakeStructFromMap-4              500000              2761 ns/op            1072 B/op         21 allocs/op
PASS
ok      lib     9.214s
*/
ackage lib
