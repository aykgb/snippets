package main

import "testing"

func TestGenTestData(t *testing.T) {
	type args struct {
		lines int
	}
	tests := []struct {
		name string
		args args
	}{
		// TODO: Add test cases.
		{name: "500000", args: args{lines: 500000}},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			GenTestData(tt.args.lines)
		})
	}
}
