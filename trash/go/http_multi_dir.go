package main

import (
	"fmt"
	"net/http"
)

type String string
type Struct struct {
	Greeting string
	Punct    string
	Who      string
}

func (s String) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	fmt.Fprint(w, s)
}
func (s *Struct) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	fmt.Fprint(w, s.Greeting, s.Punct, s.Who)
}

func testHttpMultiDir() {
	http.Handle("/string", String("Hey i'm a string!"))
	http.Handle("/struct", &Struct{"Hello", ",", "GOPHERS!"})
	err := http.ListenAndServe("localhost:4000", nil)
	if err != nil {
		log.Fatal(err)
	}
}
