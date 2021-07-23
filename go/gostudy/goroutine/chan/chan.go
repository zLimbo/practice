package main

import (
	"fmt"
	"time"
)

func main() {
	// 构建一个通道
	ch := make(chan int)
	// 开启一个并发匿名函数
	for i := 0; i < 2; i++ {
		go func(id int) {
			// 遍历接收通道数据
			for data := range ch {
				// 打印通道数据
				fmt.Println(id, data)
				// 当遇到数据0时, 退出接收循环
				if data == 0 {
					break
				}
			}

		}(i)
	}

	// 从3循环到0
	for i := 10; i >= 0; i-- {
		// 发送3到0之间的数值
		ch <- i
		// 每次发送完时等待
		time.Sleep(time.Second)
	}
	fmt.Println("done.")
}
