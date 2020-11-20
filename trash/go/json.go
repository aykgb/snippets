package main

import (
	"encoding/json"
	"fmt"
)

type BusinessPeakPeriod struct {
	Begin int `json:"begin,omitempty"`
	End   int `json:"end,omitempty"`
}

func testJson() {
	var period BusinessPeakPeriod
	strJson := `{"begin":7,"end":10}`
	err := json.Unmarshal([]byte(strJson), &period)
	if err != nil {
		fmt.Printf("err:%v json unmarshal failed.\n", err)
		return
	}
	fmt.Printf("period:%+v\n", period)
}
