package main

import "fmt"
import "time"

func printTimeNow() {
	now := time.Now()
	year, mon, day := now.UTC().Date()
	hour, min, sec := now.UTC().Clock()
	zone, _ := now.UTC().Zone()
	fmt.Printf("UTC 时间是 %d-%d-%d %02d:%02d:%02d %s\n",
		year, mon, day, hour, min, sec, zone)

	year, mon, day = now.Date()
	hour, min, sec = now.Clock()
	zone, _ = now.Zone()
	fmt.Printf("本地时间是 %d-%d-%d %02d:%02d:%02d %s\n",
		year, mon, day, hour, min, sec, zone)
}

func main() {
	printTimeNow()

	dateFormat := "20140624" //时间字符串

	func() {
		t, _ := time.ParseInLocation("20060102", dateFormat, time.Local) //t被转为本地时间的time.Time
		fmt.Printf("转换为Local时间 %d%d%d\n", t.Year(), t.Month(), t.Day())
	}()

	func() {
		t, _ := time.Parse("20060102", dateFormat) //t被转为UTC时间的time.Time
		fmt.Printf("转换为UTC时间 %d%d%d\n", t.Year(), t.Month(), t.Day())
	}()
}
