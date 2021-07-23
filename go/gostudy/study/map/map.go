package main

import "fmt"

func main() {

	// var mapname map[keytype]valuetype
	var mp1 map[int64]string
	mp1 = make(map[int64]string)
	mp2 := make(map[string]int64)

	mp1[0] = "范维"
	mp1[1] = "江南"
	mp1[2] = "唐海波"
	mp1[3] = "杜晓凡"
	mp1[4] = "阙奇峰"

	for k, v := range mp1 {
		fmt.Printf("k = %d, v = %s\n", k, v)
		mp2[v] = k
	}

	for v := range mp2 {
		fmt.Println("v =", v)
	}
}
