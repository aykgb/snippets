package main

import (
	"os"

	"github.com/sirupsen/logrus"
)

var log = logrus.New()

func testLogrus() {
	log.Out = os.Stdout

	log.WithFields(logrus.Fields{
		"animal": "walrus",
		"size":   10,
	}).Info("A group of walrus emerges from the ocean")
}
