package main

import (
	"fmt"
	"math/rand"
	"time"
)

type Messager interface {
	LogMsg()
}

type RequestMsg struct {
	ID       int64
	received bool
}

func (msg *RequestMsg) LogMsg() {
	fmt.Println("RequestMsg id =", msg.ID)
}

type PrePrepareMsg struct {
	ID       int64
	received bool
}

func (msg *PrePrepareMsg) LogMsg() {
	fmt.Println("PrePrepareMsg id =", msg.ID)
}

type PrepareMsg struct {
	ID       int64
	received bool
}

func (msg *PrepareMsg) LogMsg() {
	fmt.Println("PrepareMsg id =", msg.ID)
}

type CommitMsg struct {
	ID       int64
	received bool
}

func (msg *CommitMsg) LogMsg() {
	fmt.Println("CommitMsg id =", msg.ID)
}

func LogMsg(msg Messager) {
	msg.LogMsg()
}

const typeNum = 4

func Produce(msgChan chan Messager, num int) {
	for i := 0; i < num; i++ {
		id := int64(i)
		rnd := rand.Int() % typeNum
		fmt.Print("\033[31m[Produce]\033[0m ")
		switch rnd {
		case 0:
			fmt.Println("RequestMsg id=", id)
			msgChan <- &RequestMsg{
				ID: id,
			}
		case 1:
			fmt.Println("PrePrepareMsg id=", id)
			msgChan <- &PrePrepareMsg{
				ID: id,
			}
		case 2:
			fmt.Println("PrepareMsg id=", id)
			msgChan <- &PrepareMsg{
				ID: id,
			}
		case 3:
			fmt.Println("CommitMsg id=", id)
			msgChan <- &CommitMsg{
				ID: id,
			}
		}
	}
}

func main() {

	msgChan := make(chan Messager)

	// 启动一个 goruntine 生产者
	go Produce(msgChan, 20)

	count := make(map[string]int)

	// 启动一个 goruntine 消费者，匿名函数，拥有count的闭包
	go func() {
		for rawMsg := range msgChan {
			
			fmt.Print("\033[32m[Receive]\033[0m ")
			LogMsg(rawMsg)
			switch msg := rawMsg.(type) {
			case *RequestMsg:
				count["RequestMsg"]++
				msg.received = true
			case *PrePrepareMsg:
				count["PrePrepareMsg"]++
				msg.received = true
			case *PrepareMsg:
				count["PrepareMsg"]++
				msg.received = true
			case *CommitMsg:
				count["CommitMsg"]++
				msg.received = true
			}
		}
	}()

	// 睡 1 s
	time.Sleep(time.Second)

	// 打印接收各种信息的数量
	for k, v := range count {
		fmt.Println(k, v)
	}
}
