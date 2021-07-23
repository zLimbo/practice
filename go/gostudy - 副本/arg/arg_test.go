package main

import (
	"fmt"
	"os"
	"testing"
)

func TestMain(t *testing.T) {
	fmt.Println(os.Args)
	x, y := os.Args[1], os.Args[2]

	fmt.Println(x, y)

	fmt.Println(x, y)

	s := fmt.Sprint(8000 + 100)

	fmt.Println(s)
}
