package room

import "fmt"

type Light struct{}


func(l *Light) TurnOn() {
	fmt.Println("The light is on")
}

func (l *Light) TurnOff() {
	fmt.Println("The light is off")
}