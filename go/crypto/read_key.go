package main

import (
	"fmt"
	"io/ioutil"
	"strings"
)

func main() {

	keyDir := "./node.txt"

	data, _ := ioutil.ReadFile(keyDir)

	s := string(data)

	// fmt.Println(s)

	ioutil.WriteFile("./node2.txt", []byte(s), 0644)

	ss := strings.Split(s, "\n")
	fmt.Println("ss:", ss)
	fmt.Println("len:", len(ss))
}
