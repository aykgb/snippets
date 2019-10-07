package main

import "net/http"

func main() {
	go func() {
		http.ListenAndServe(":8001", &fooHandler{})
	}()

	//the last call is outside goroutine to avoid that program just exit
	http.ListenAndServe(":8002", &barHandler{})
}

type fooHandler struct {
}

func (m *fooHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("Listening on 8001: foo "))
}

type barHandler struct {
}

func (m *barHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	w.Write([]byte("Listening on 8002: bar "))
}
