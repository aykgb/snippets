package main

import "testing"

func TestRequestLimiterTest(t *testing.T) {
	type args struct {
		limitQPS int64
		reqQPS   int64
		sec      int64
	}
	args00 := args{limitQPS: 100, reqQPS: 20, sec: 2}
	args01 := args{limitQPS: 10, reqQPS: 100, sec: 1}
	want := func(args args) int64 {
		if args.limitQPS > args.reqQPS*args.sec {
			return args.reqQPS * args.sec
		}
		return args.limitQPS
	}
	tests := []struct {
		name        string
		args        args
		wantCounter int64
	}{
		// TODO: Add test cases.
		{name: "case_001", args: args00, wantCounter: want(args00)},
		{name: "case_002", args: args01, wantCounter: want(args01)},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			if gotCounter := RequestLimiterTest(tt.args.limitQPS, tt.args.reqQPS, tt.args.sec); gotCounter <= tt.wantCounter {
				t.Errorf("RequestLimiterTest() = %v, want %v", gotCounter, tt.wantCounter)
			}

		})
	}
}
