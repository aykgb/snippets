package main

import (
	"errors"
	"fmt"
	"strconv"
	"strings"
	"time"
)

type SchedulingRecord struct {
	ttl            int64
	TimeStamp      int64  // record 记录时间
	TableName      string // 表名
	DictNames      string // 字典名，拼接
	FileSize       int64  // 文件总大小，单位：Byte
	LineCount      int64  // 总行数
	ErrLineCount   int64  // 错误行数
	TimeBuildStart int64
	TimeBuildEnd   int64
	TimeSyncStart  int64
	TimeSyncEnd    int64
	TimeLoadStart  int64
	TimeLoadEnd    int64
	IsFailed       bool
}

const TTLDAY = 8

var table2SchedulingLog map[string]map[string][]*SchedulingRecord

func appendAndTrimRecordToLog(tableName, key string, record *SchedulingRecord) {
	if table2SchedulingLog == nil {
		table2SchedulingLog = make(map[string]map[string][]*SchedulingRecord)
	}
	if table2SchedulingLog[tableName] == nil {
		table2SchedulingLog[tableName] = make(map[string][]*SchedulingRecord)
	}
	if table2SchedulingLog[tableName][key] == nil {
		table2SchedulingLog[tableName][key] = make([]*SchedulingRecord, 0)
	}
	fmt.Printf("append key:%s\n", key)
	table2SchedulingLog[tableName][key] = append(table2SchedulingLog[tableName][key], record)

	if len(table2SchedulingLog[tableName]) > TTLDAY {
		for i := 0; i < len(table2SchedulingLog[tableName])-TTLDAY; i++ {
			year, month, day, _ := parseKey2Date(key)
			dateKey := time.Date(year, time.Month(month), day, 0, 0, 0, 0, time.Local)
			keyTrim := dateTransform(dateKey.Add(-24*time.Hour*time.Duration(TTLDAY+i)), "YYYYMMDD")
			fmt.Printf("#### cal keyTrim:%s\n", keyTrim)
			_, exist := table2SchedulingLog[tableName][keyTrim]
			if exist {
				fmt.Printf("#### exe keyTrim:%s\n", keyTrim)
				delete(table2SchedulingLog[tableName], keyTrim)
			}
		}
	}
}

func dumpTable2SchedulingLog() {
	fmt.Println("[Reports||dumpTable2SchedulingLog] ########## start...")
	for table, log := range table2SchedulingLog {
		fmt.Printf("[Reports||dumpTable2SchedulingLog] -----table:%s-----\n", table)
		for key, records := range log {
			for _, record := range records {
				fmt.Printf("[Reports||dumpTable2SchedulingLog] key:%s record:%+v\n", key, record)
			}
		}
	}
	fmt.Println("[Reports||dumpTable2SchedulingLog] xxxxxxxxxx end.")
}

func dateTransform(t time.Time, format string) string {
	res := strings.Replace(format, "MM", t.Format("01"), -1)
	res = strings.Replace(res, "M", t.Format("1"), -1)
	res = strings.Replace(res, "DD", t.Format("02"), -1)
	res = strings.Replace(res, "D", t.Format("2"), -1)
	res = strings.Replace(res, "YYYY", t.Format("2006"), -1)
	res = strings.Replace(res, "YY", t.Format("06"), -1)
	res = strings.Replace(res, "HH", fmt.Sprintf("%02d", t.Hour()), -1)
	res = strings.Replace(res, "H", fmt.Sprintf("%d", t.Hour()), -1)
	res = strings.Replace(res, "hh", t.Format("03"), -1)
	res = strings.Replace(res, "h", t.Format("3"), -1)
	res = strings.Replace(res, "mm", t.Format("04"), -1)
	res = strings.Replace(res, "m", t.Format("4"), -1)
	res = strings.Replace(res, "ss", t.Format("05"), -1)
	res = strings.Replace(res, "s", t.Format("5"), -1)
	return res
}

func parseKey2Date(key string) (year, month, day int, err error) {
	keyFormat := "YYYYMMDD"
	if len(keyFormat) != len(key) {
		fmt.Printf("[parseKey2Time] key:%s failed.\n", key)
		return 1970, 1, 1, errors.New("wrong key format.")
	}

	year, err = strconv.Atoi(key[0:4])
	if err != nil {
		fmt.Printf("[parseKey2Time] key:%s failed.\n", key)
		return 1970, 1, 1, err
	}

	month, err = strconv.Atoi(key[4:6])
	if err != nil {
		fmt.Printf("[parseKey2Time] key:%s failed.\n", key)
		return 1970, 1, 1, err
	}

	day, err = strconv.Atoi(key[6:len(key)])
	if err != nil {
		fmt.Printf("[parseKey2Time] key:%s failed.\n", key)
		return 1970, 1, 1, err
	}

	return
}

func main() {
	appendAndTrimRecordToLog("table0", "20180601", &SchedulingRecord{TableName: "table0"})
	appendAndTrimRecordToLog("table0", "20180602", &SchedulingRecord{TableName: "table0"})
	appendAndTrimRecordToLog("table0", "20180603", &SchedulingRecord{TableName: "table0"})
	appendAndTrimRecordToLog("table0", "20180604", &SchedulingRecord{TableName: "table0"})
	appendAndTrimRecordToLog("table0", "20180605", &SchedulingRecord{TableName: "table0"})
	appendAndTrimRecordToLog("table0", "20180606", &SchedulingRecord{TableName: "table0"})
	appendAndTrimRecordToLog("table0", "20180607", &SchedulingRecord{TableName: "table0"})
	appendAndTrimRecordToLog("table0", "20180608", &SchedulingRecord{TableName: "table0"})
	appendAndTrimRecordToLog("table0", "20180609", &SchedulingRecord{TableName: "table0"})
	appendAndTrimRecordToLog("table0", "20180610", &SchedulingRecord{TableName: "table0"})
	appendAndTrimRecordToLog("table0", "20180611", &SchedulingRecord{TableName: "table0"})
	dumpTable2SchedulingLog()

	year, month, day, _ := parseKey2Date("20180611")
	fmt.Printf("year:%4d month:%2d day:%2d\n", year, month, day)
}
