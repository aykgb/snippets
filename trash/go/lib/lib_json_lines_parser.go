package lib

import (
	"bufio"
	"bytes"
	"compress/gzip"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

type FileReader interface {
	EOF() bool
	Close()
	ReadBytes(int) ([]byte, error)
}

type GZFileReader struct {
	file   *os.File
	reader *gzip.Reader
	buffer []byte
	isEOF  bool
}

func NewGZFileReader(fn string) *GZFileReader {
	var err error
	r := new(GZFileReader)
	if r.file, err = os.Open(fn); err != nil {
		fmt.Printf("open file %s error %v\n", fn, err)
		return nil
	}

	if r.reader, err = gzip.NewReader(r.file); err != nil {
		fmt.Printf("parse gzip file header error %v\n", err)
		return nil
	}

	return r
}

func (gfr *GZFileReader) EOF() bool {
	return gfr.isEOF
}

func (gfr *GZFileReader) Close() {
	gfr.file.Close()
	gfr.file = nil
	gfr.reader.Close()
	gfr.reader = nil
}

func (gfr *GZFileReader) ReadBytes(target int) ([]byte, error) {
	if target > 4096*1024 {
		target = 4096 * 1024 // limit read buffer
	}
	if len(gfr.buffer) < target {
		gfr.buffer = make([]byte, target)
	}
	size, err := gfr.reader.Read(gfr.buffer)
	if err != nil {
		if err == io.EOF {
			gfr.isEOF = true
		} else {
			return nil, err
		}
	}

	return gfr.buffer[0:size], nil
}

type TextFileReader struct {
	file   *os.File
	reader *bufio.Reader
	buffer []byte
	isEOF  bool

	scanner *bufio.Scanner
}

func NewTextFileReader(fn string) *TextFileReader {
	var err error
	r := new(TextFileReader)
	if r.file, err = os.Open(fn); err != nil {
		fmt.Printf("open file %s error %v\n", fn, err)
		return nil
	}
	r.scanner = bufio.NewScanner(r.file)
	r.reader = bufio.NewReader(r.file)

	return nil
}

func (tfr *TextFileReader) EOF() bool {
	return tfr.isEOF
}

func (tfr *TextFileReader) Close() {
	tfr.file.Close()
	tfr.file = nil
	tfr.scanner = nil
	tfr.reader = nil
}

func (tfr *TextFileReader) ReadBytes(target int) ([]byte, error) {
	if target > 1024*4096 {
		target = 1024 * 4096
	}
	if len(tfr.buffer) < target {
		tfr.buffer = make([]byte, target)
	}
	size, err := tfr.reader.Read(tfr.buffer)
	if err != nil && err != io.EOF {
		if err == io.EOF {
			tfr.isEOF = true
		} else {
			return nil, err
		}
	}

	return tfr.buffer[0:size], nil

}

func (tfr *TextFileReader) ReadLines(n int) []string {
	var lines []string
	for i := 0; i < n; i++ {
		tfr.scanner.Scan()
		lines = append(lines, tfr.scanner.Text())

		if tfr.scanner.Err() != nil {
			fmt.Printf("file scanner error %v\n", tfr.scanner.Err())
			return nil
		}
	}

	return lines
}

type FileLineReader struct {
	fileReader       FileReader
	hasTruncatedLine bool
	lines            []string
	lstTruncatedLine string
	left             int
	offset           int
	bufferSize       int
	readLineCounter  int
}

func NewFileLineReader(fr FileReader) *FileLineReader {
	r := new(FileLineReader)
	r.offset = 0
	r.left = 0
	r.lstTruncatedLine = ""
	r.hasTruncatedLine = false
	r.bufferSize = 4096 // initialize value
	r.fileReader = fr

	return r
}

func (flr *FileLineReader) ReadLines(target int) (result []string, err error) {
	remain := target
	for {
		lines, err := flr.TryReadLines(remain)
		if err != nil {
			return nil, err
		}
		result = append(result, lines...)
		remain = target - len(result)
		if remain == 0 || flr.fileReader.EOF() {
			break
		}
	}
	return
}

func (flr *FileLineReader) TryReadLines(target int) (result []string, err error) {
	if flr.left == 0 {
		if flr.fileReader.EOF() {
			return nil, nil
		}
		data, err := flr.fileReader.ReadBytes(flr.bufferSize)
		if err != nil {
			return nil, err
		}
		if len(data) == flr.bufferSize {
			flr.bufferSize += 4096
		}
		flr.ParseLinesFromBytes(data)

		return flr.TryReadLines(target)
	} else if flr.left >= target {
		result = flr.lines[flr.offset : flr.offset+target]
		flr.left -= target
		flr.offset += target
	} else { // flr.left < n
		result = flr.lines[flr.offset : flr.offset+flr.left]
		flr.offset += flr.left
		flr.left = 0
	}
	flr.readLineCounter += len(result)

	return result, nil
}

func (flr *FileLineReader) ParseLinesFromBytes(data []byte) error {
	defer func() {
		if r := recover(); r != nil {
			fmt.Printf("ParseLinesFromBytes %+v", *flr)
			os.Exit(-1)
		}
	}()
	if data[len(data)-1] != byte('\n') && !flr.fileReader.EOF() {
		flr.hasTruncatedLine = true
	} else {
		flr.hasTruncatedLine = false
	}
	arrs := bytes.Split(data, []byte("\n"))
	for idx, raw := range arrs {
		if len(raw) == 0 {
			if idx == 0 {
				// the data is "\n{"tablename":...", and do nothing
			} else {
				break
			}
		}
		flr.left++
		if idx < len(flr.lines) {
			flr.lines[idx] = string(raw)
		} else {
			flr.lines = append(flr.lines, string(raw))
		}
	}
	flr.offset = 0
	// take the last truncated line
	if flr.lstTruncatedLine != "" {
		flr.lines[0] = flr.lstTruncatedLine + flr.lines[0]
		flr.lstTruncatedLine = ""
	}
	// skip & record the truncated line at this time
	if flr.hasTruncatedLine {
		flr.left--
		flr.lstTruncatedLine = flr.lines[flr.left]
	}
	return nil
}

func testChanStringArr() {
	source := make(chan []string, 1024)

	go func() {
		for i := 0; i < 10; i++ {
			lines := []string{strconv.Itoa(i) + "111", "222"}
			source <- lines
		}
	}()

	for i := 0; i < 10; i++ {
		lines := <-source
		for _, line := range lines {
			fmt.Println(line)
		}
	}
}

func testGzipfileReader(fn string) {
	gZFileReader := NewGZFileReader(fn)
	fileLineReader := NewFileLineReader(gZFileReader)
	for {
		lines, err := fileLineReader.ReadLines(1000)
		if err != nil {
			fmt.Printf("read line from gzip file error %v\n", err)
			return
		}
		if len(lines) == 0 {
			return
		}
		printlines := func(lines []string) {
			for _, line := range lines {
				fmt.Println(line)
			}
		}
		printlines(lines)
	}
}

type GizzleFormatObject struct {
	RowKey      string   `json:"key"`
	FieldValues []string `json:"fieldvalues"`
	TableName   string   `json:"tablename"`
	TTL         string   `json:"ttl"`
}

type TableRow struct {
	gfo         *GizzleFormatObject // gizzle format object
	FieldKeys   []string
	FieldValues []string
}

func (tr *TableRow) Parse(jsonLine string) error {
	tr.gfo = &GizzleFormatObject{}
	err := json.Unmarshal([]byte(jsonLine), tr.gfo)
	if err != nil {
		fmt.Printf("Parse Json failed. line:%s error:%v\n", jsonLine, err)
		return err
	}
	for _, fv := range tr.gfo.FieldValues {
		pair := strings.Split(fv, "\t")
		if len(pair) < 2 {
			return errors.New("Invalid fieldvalues format")
		}
		tr.FieldKeys = append(tr.FieldKeys, pair[0])
		tr.FieldValues = append(tr.FieldValues, pair[1])
	}

	return nil
}

func ParseJsonLines(jsonLines []string, fidKeyCnt *int64) (rows []*TableRow, err error) {
	rows = make([]*TableRow, len(jsonLines))
	for idx, jsonLine := range jsonLines {
		if jsonLine == "" {
			continue
		}
		rows[idx] = &TableRow{}
		rows[idx].gfo = &GizzleFormatObject{}
		err = rows[idx].Parse(jsonLine)
		if err != nil {
			return
		}
		*fidKeyCnt += int64(len(rows[idx].FieldKeys))
	}
	return
}

func testParseJsonLines(fn string) {
	gZFileReader := NewGZFileReader(fn)
	fileLineReader := NewFileLineReader(gZFileReader)
	for {
		lines, err := fileLineReader.ReadLines(1000)
		if err != nil {
			fmt.Printf("read line from gzip file error %v\n", err)
			return
		}
		if len(lines) == 0 {
			return
		}
		var fidKeyCnt int64
		rows, err := ParseJsonLines(lines, &fidKeyCnt)
		if err != nil {
			fmt.Printf("parse json lines error %v\n", err)
			return
		}
		for _, row := range rows {
			fmt.Printf("row_key:%s row:%+v\n", row.gfo.RowKey, row)
		}
	}
}

func testTableRowParse() {
	file, err := os.Open("json.txt")
	if err != nil {
		fmt.Printf("open file %s failed\n", "json.txt")
		return
	}
	reader := bufio.NewReader(file)
	jsonLine, err := reader.ReadString('\n')
	if err != nil {
		fmt.Printf("read a line error %v\n", err)
		return
	}
	var row TableRow
	if row.Parse(jsonLine) != nil {
		fmt.Printf("parse line %s failed.\n", jsonLine)
		return
	}
	fmt.Println("parse success")
	fmt.Printf("%+v\n", row)
	fmt.Printf("%+v", row.gfo)

	file.Close()
}
