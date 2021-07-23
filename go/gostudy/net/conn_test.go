package net

import (
	"encoding/json"
	"fmt"
	"log"
	"net"
	"os"
	"testing"
	"time"
)

type MessageType int

const (
	mtRequest MessageType = iota
	mtPrePrepare
	mtPrepare
	mtCommit
	mtReply
)

type Message struct {
	MsgType   MessageType `json:"msgType"`
	Seq       int64       `json:"seq"`
	NodeId    int64       `json:"nodeId"`
	Timestamp int64       `json:"timestamp"`

	Digest string `json:"digest"`
	Sign   []byte `json:"sign"`

	Ext []byte   `json:"ext"`
	Req *Message `json:"req"`
}

func client(addr string) {

	conn, err := net.Dial("tcp", addr)
	if err != nil {
		log.Panic(err)
	}
	for i := 0; i <= 1000; i++ {
		msg := &Message{Seq: int64(i)}
		msgBytes, err := json.Marshal(msg)
		if err != nil {
			log.Panic("err:", err)
		}
		// msg := strconv.Itoa(i)
		// fmt.Println(msg)
		n, err := conn.Write(msgBytes)
		if err != nil {
			fmt.Println("err:", err)
		}
		fmt.Println("n:", n)
		time.Sleep(time.Microsecond)
	}
	conn.Close()
}

func server(addr string) {
	fmt.Println(addr, "listen...")
	listen, err := net.Listen("tcp", addr)
	if err != nil {
		log.Panic(err)
	}
	defer listen.Close()

	conn, err := listen.Accept()
	if err != nil {
		log.Panic(err)
	}
	for {
		buffer := make([]byte, 1024)
		n, err := conn.Read(buffer)
		if err != nil {
			fmt.Println(err)
		}
		fmt.Println("n:", n)
		msgBytes := buffer[:n]
		msg := new(Message)
		if err := json.Unmarshal(msgBytes, msg); err != nil {
			fmt.Println(err)
		}
		fmt.Println("msg:", msg)
		if n == 0 {
			break
		}
	}

	conn.Close()
}

func TestConn(t *testing.T) {

	name := os.Args[1]

	if name == "server" {
		server("127.0.0.1:8001")
	} else if name == "client" {
		client("127.0.0.1:8001")
	} else {
		fmt.Println("parameter error!")
	}
}
