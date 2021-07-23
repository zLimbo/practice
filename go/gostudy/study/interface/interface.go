package main

import (
	"fmt"
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

func main() {
	msgs := make([]Messager, 4)

	msgs[0], msgs[1], msgs[2], msgs[3] = &RequestMsg{
		ID: 0,
	}, &PrePrepareMsg{
		ID: 1,
	}, &PrepareMsg{
		ID: 2,
	}, &CommitMsg{
		ID: 3,
	}

	LogMsg(msgs[0])

	for i := 0; i < len(msgs); i++ {
		LogMsg(msgs[i])
	}
}