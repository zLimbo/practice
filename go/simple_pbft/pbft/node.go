package pbft

import (
	"bytes"
	"encoding/json"
	"fmt"
	"log"
	"net/http"
	"strconv"
	"time"
)

const f = 1
const leaderId = 1

var clientUrl = "localhost:8880/getReply"
var NodeTable map[int64]string

func init() {
	NodeTable = make(map[int64]string)
	NodeTable[1] = "localhost:8881"
	NodeTable[2] = "localhost:8882"
	NodeTable[3] = "localhost:8883"
	NodeTable[4] = "localhost:8884"

	fmt.Println("network peers:")
	for k, v := range NodeTable {
		fmt.Printf("node id: %d, node url: %s\n", k, v)
	}
	fmt.Println()
}

//type SendTask struct {
//	url string
//	msg []byte
//}

type Node struct {
	id          int64
	msgRecvChan chan interface{}
	//msgSendChan chan *SendTask
	msgCert   map[int64]*MsgCert
	latestSeq int64
}

func NewNode(id int64) *Node {
	node := &Node{
		id:          id,
		msgRecvChan: make(chan interface{}, 10000),
		msgCert:     make(map[int64]*MsgCert),
		latestSeq:   -1,
	}

	go node.routeMsg()
	//go node.sendMsg()
	return node
}

func (node *Node) routeMsg() {
	for rawMsg := range node.msgRecvChan {
		switch msg := rawMsg.(type) {
		case *RequestMsg:
			node.handleRequestMsg(msg)
		case *PrePrepareMsg:
			node.handlePrePrepareMsg(msg)
		case *PrepareMsg:
			node.handlePrepareMsg(msg)
		case *CommitMsg:
			node.handleCommitMsg(msg)
		}
	}
}

func (node *Node) getMsgCert(seq int64) *MsgCert {
	msgCert := node.msgCert[seq]
	if msgCert == nil {
		msgCert = NewMsgCert()
		node.msgCert[seq] = msgCert
	}
	return msgCert
}

func (node *Node) handleRequestMsg(msg *RequestMsg) {
	log.Println("<node handleRequestMsg> msg:", msg)
	if msg.Seq <= node.latestSeq {
		log.Printf("The requested seq(=%d) is lower than the latest seq(=%d)\n", msg.Seq, node.latestSeq)
		return
	}
	node.latestSeq = msg.Seq
	msgCert := node.getMsgCert(msg.Seq)
	if msgCert.RequestMsg != nil {
		log.Printf("The request(seq=%d) has been accepted!\n", msg.Seq)
		return
	}
	digest, err := Digest(msg)
	if err != nil {
		fmt.Println(err)
		return
	}

	msgCert.Seq = msg.Seq
	msgCert.Digest = digest
	msgCert.RequestMsg = msg
	msgCert.SendPrePrepare = WaitSend

	node.sendPrePrepare(msgCert)
}

func (node *Node) handlePrePrepareMsg(msg *PrePrepareMsg) {
	log.Println("<node handlePrePrepareMsg> msg:", msg)
	if msg.NodeId != leaderId {
		log.Println("PrePrepareMsg is not coming from the leader!")
		return
	}
	node.latestSeq = msg.Seq
	msgCert := node.getMsgCert(msg.Seq)
	if msgCert.SendPrepare == HasSend {
		return
	}

	msgCert.Seq = msg.Seq
	msgCert.Digest = msg.Digest
	msgCert.RequestMsg = msg.RequestMsg
	msgCert.PrePrepareMsg = msg
	msgCert.SendPrepare = WaitSend

	node.sendPrepare(msgCert)
}

func (node *Node) handlePrepareMsg(msg *PrepareMsg) {
	msgCert := node.getMsgCert(msg.Seq)
	if msgCert.SendCommit == HasSend {
		return
	}
	log.Println("<node handlePrepareMsg> msg:", msg)
	node.recvPrepareMsg(msgCert, msg)
	node.maybeSendCommit(msgCert)
}

func (node *Node) handleCommitMsg(msg *CommitMsg) {
	msgCert := node.getMsgCert(msg.Seq)
	if msgCert.SendReply == HasSend {
		return
	}
	log.Println("<node handleCommitMsg> msg:", msg)
	node.recvCommitMsg(msgCert, msg)
	node.maybeSendReply(msgCert)
}

func (node *Node) recvPrepareMsg(msgCert *MsgCert, msg *PrepareMsg) {
	count := 1
	for _, preMsg := range msgCert.Prepares {
		if preMsg.NodeId == msg.NodeId {
			return
		}
		if preMsg.Digest == msg.Digest {
			count++
		}
	}
	msgCert.Prepares = append(msgCert.Prepares, msg)
	log.Printf("same prepare msg count=%d\n", count)
	if count >= 2*f {
		msgCert.SendCommit = WaitSend
	}
}

func (node *Node) recvCommitMsg(msgCert *MsgCert, msg *CommitMsg) {
	count := 1
	for _, preMsg := range msgCert.Commits {
		if preMsg.NodeId == msg.NodeId {
			return
		}
		if preMsg.Digest == msg.Digest {
			count++
		}
	}
	msgCert.Commits = append(msgCert.Commits, msg)
	log.Printf("same commit msg count=%d\n", count)
	if count >= 2*f+1 {
		msgCert.SendReply = WaitSend
	}
}

func (node *Node) sendPrePrepare(msgCert *MsgCert) {
	prePrepareMsg := &PrePrepareMsg{
		Seq:        msgCert.Seq,
		NodeId:     node.id,
		RequestMsg: msgCert.RequestMsg,
		Digest:     msgCert.Digest,
	}
	msgCert.PrePrepareMsg = prePrepareMsg

	node.broadcast(prePrepareMsg, "/getPrePrepare")
	msgCert.SendPrePrepare = HasSend
	log.Println("[pre-prepare] msg has been sent.")
	node.maybeSendCommit(msgCert)
}

func (node *Node) sendPrepare(msgCert *MsgCert) {
	prepareMsg := &PrepareMsg{
		Seq:    msgCert.Seq,
		NodeId: node.id,
		Digest: msgCert.Digest,
	}
	node.recvPrepareMsg(msgCert, prepareMsg)

	log.Println("<broadcast> prepare")
	node.broadcast(prepareMsg, "/getPrepare")
	msgCert.SendPrepare = HasSend
	log.Println("[prepare] msg has been sent.")
	node.maybeSendCommit(msgCert)
}

func (node *Node) maybeSendCommit(msgCert *MsgCert) {
	if msgCert.SendCommit != WaitSend {
		return
	}
	commitMsg := &CommitMsg{
		Seq:    msgCert.Seq,
		NodeId: node.id,
		Digest: msgCert.Digest,
	}
	node.recvCommitMsg(msgCert, commitMsg)
	node.broadcast(commitMsg, "/getCommit")
	msgCert.SendCommit = HasSend
	log.Println("[commit] msg has been sent.")
	node.maybeSendReply(msgCert)
}

func (node *Node) maybeSendReply(msgCert *MsgCert) {
	if msgCert.SendReply != WaitSend {
		return
	}
	replyMsg := &ReplyMsg{
		Seq:    msgCert.Seq,
		NodeId: node.id,
		Digest: msgCert.Digest,
	}
	jsonMsg, err := json.Marshal(replyMsg)
	if err != nil {
		fmt.Println(err)
	}
	PostJson(clientUrl, jsonMsg)
	msgCert.SendReply = HasSend
	log.Printf("[reply] msg has been sent, seq=%d\n", msgCert.PrePrepareMsg.Seq)
}

func (node *Node) broadcast(msg interface{}, path string) map[int64]error {
	errorMap := make(map[int64]error)

	for nodeId, url := range NodeTable {
		if nodeId == node.id {
			continue
		}
		jsonMsg, err := json.Marshal(msg)
		if err != nil {
			errorMap[nodeId] = err
			continue
		}
		PostJson(url+path, jsonMsg)
		//node.msgSendChan <- &SendTask{
		//	url: url + path,
		//	msg: jsonMsg,
		//}
	}
	if len(errorMap) == 0 {
		return nil
	} else {
		return errorMap
	}
}

//func (node *Node) sendMsg() {
//	for sendTask := range node.msgSendChan {
//		postJson(sendTask.url, sendTask.msg)
//	}
//}

func PostTest(server *Server) {
	if server.node.id != leaderId {
		return
	}
	time.Sleep(time.Second * 5)
	for i := 0; i < 100; i++ {
		seq := int64(i)
		msg := &RequestMsg{
			Seq:      seq,
			Operator: "op" + strconv.Itoa(i),
		}
		jsonMsg, err := json.Marshal(msg)
		if err != nil {
			fmt.Println(err)
			continue
		}
		PostJson("localhost:8881/getRequest", jsonMsg)
	}
}

func PostJson(url string, msg []byte) {
	buf := bytes.NewBuffer(msg)
	go http.Post("http://"+url, "application/json", buf)
}
