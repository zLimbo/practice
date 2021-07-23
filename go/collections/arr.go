package main

import "fmt"

func f(b []int) {
	b[0] = 3
	fmt.Println(b)
}

func main() {
	a := []int{1, 2, 3}
	b := append(a, 4)
	fmt.Println(a, b)
}
