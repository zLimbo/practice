package main

import (
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net"
	"os"
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

func sendMsg(addr string, msg *Message) {
	fmt.Println(addr, "send msg, seq =", msg.Seq)
	conn, err := net.Dial("tcp", addr)
	if err != nil {
		log.Panic(err)
	}
	msgBytes, err := json.Marshal(msg)
	if err != nil {
		log.Panic(err)
	}
	if _, err := conn.Write(msgBytes); err != nil {
		log.Fatal(err)
	}
	conn.Close()
}

func listenMsg(addr string) {
	fmt.Println(addr, "listen...")
	listen, err := net.Listen("tcp", addr)
	if err != nil {
		log.Panic(err)
	}
	defer listen.Close()

	for {
		conn, err := listen.Accept()
		if err != nil {
			log.Panic(err)
		}
		msgBytes, err := ioutil.ReadAll(conn)

		conn.Close()

		if err != nil {
			log.Panic(err)
		}
		msg := new(Message)
		if err := json.Unmarshal(msgBytes, msg); err != nil {
			log.Panic(err)
		}

		fmt.Println("# msg:", msg)
	}
}

func main() {

	name := os.Args[1]

	if name == "server" {
		listenMsg("127.0.0.1:8001")
	} else if name == "client" {
		for i := 0; i < 10; i++ {
			time.Sleep(time.Second * 3)
			msg := &Message{Seq: int64(i)}
			sendMsg("127.0.0.1:8001", msg)
		}
	} else {
		fmt.Println("parameter error!")
	}
}
