package main

import (
	"html/template"
	"os"
)

func testTemplate() {
	type Msg struct {
		TripID int    `json:"trip_id"`
		ToCity string `json:"to_city"`
		Eta    int    `json:"eta"`
	}

	msg := Msg{TripID: 1001, ToCity: "天津", Eta: 5}

	tmpl := template.New("行程开始前5分钟提醒司机")
	tmpl.Parse("本次行程编号{{.TripID}}，终点城市{{.ToCity}}，预估时长{{.Eta}}分钟，离预定出发时间还有5分钟，请司机师傅做好发车前准备。")
	tmpl.Execute(os.Stdout, msg)
}
