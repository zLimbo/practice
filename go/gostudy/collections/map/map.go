package main

import (
	"fmt"
	"math/rand"
)

func main() {

	mp := map[int]string{
		1: "a",
		2: "b",
		3: "c",
	}

	for k, v := range mp {
		fmt.Println(k, v)
	}

	rand.Shuffle(len(mp), func(i, j int) {
		mp[i], mp[j] = mp[j], mp[i]
	})

	for k, v := range mp {
		fmt.Println(k, v)
	}

}
