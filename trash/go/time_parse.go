package main

import (
	"fmt"
	"time"
)

func dateTimestampParse(date string) time.Time {
	var year, mon, day, hour, minu, sec int
	fmt.Sscanf(date, "%d-%02d-%02d %02d:%02d:%02d", &year, &mon, &day, &hour, &minu, &sec)

	return time.Date(year, time.Month(mon), day, hour, minu, sec, 0, time.Local)
}

func testTimeParse() {
	t := dateTimestampParse("2019-07-26 18:00:00")

	fmt.Println(t.Unix())
}
