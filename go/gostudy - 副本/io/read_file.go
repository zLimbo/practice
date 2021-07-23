package main

import (
	"bufio"
	"fmt"
	"io"
	"math"
	"os"
	"strings"
)

func main() {
	file, err := os.OpenFile("node.txt", os.O_RDONLY, 0444)
	if err != nil {
		fmt.Println(err)
		return
	}

	stat, err := file.Stat()
	if err != nil {
		fmt.Println(err)
		return
	}

	fmt.Println("file size=", stat.Size())

	buf := bufio.NewReader(file)
	for {
		line, err := buf.ReadString('\n')
		line = strings.TrimSpace(line)
		fmt.Println(line)
		if err == io.EOF {
			break
		} else if err != nil {
			fmt.Println(err)
			return
		}
	}

	fmt.Println("max int64:", math.MaxInt64)

	ip, _ := test()
	fmt.Println(ip)
}

func test() (string, error) {
	file, err := os.OpenFile("node.txt", os.O_RDONLY, 0444)
	if err == nil {
		buf := bufio.NewReader(file)
		line, err := buf.ReadString('\n')
		fmt.Println("line:", line)
		if err == nil {
			ip := strings.TrimSpace(line)
			fmt.Println("** get ip from local_ip.txt, ip:", ip)
			file.Close()
			return ip, nil
		} else {
			fmt.Println("read fail:", err)
		}
	} else {
		fmt.Println("open fail:", err)
	}
	file.Close()
	return "err", nil
}
