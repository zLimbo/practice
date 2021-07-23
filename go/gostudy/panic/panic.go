package main

import (
	"fmt"
	"log"
)

func main() {
	fmt.Println("before")

	log.Panicln("abc")

	fmt.Println("after")
}
