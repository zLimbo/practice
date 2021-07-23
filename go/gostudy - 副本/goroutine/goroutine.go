package main

import (
	"fmt"
	"strconv"
)

func main() {
	for i := 0; i < 16; i++ {
		fmt.Println("go " + strconv.Itoa(i))
		go func(id int) {
			k := 0
			for {
				k++
				if k%1000000 == 0 {
					fmt.Printf("goroutine %d: k = %d\n", id, k)
				}
			}
		}(i)
	}

	for {

	}
}
