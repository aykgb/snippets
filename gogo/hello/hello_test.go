package hello

import "testing"

func TestHello(t *testing.T) {
	want := "Hello, go"
	if got := Hello(); got != want {
		t.Errorf("Hello() = %q, wang %q", got, want)
	}
}
