package main

import "fmt"

type Stage int

const (
	Idle        Stage = iota // Node is created successfully, but the consensus process is not started yet.
	PrePrepared              // The ReqMsgs is processed successfully. The node is ready to head to the Prepare stage.
	Prepared                 // Same with `prepared` stage explained in the original paper.
	Committed                // Same with `committed-local` stage explained in the original paper.
)

type Node struct {
	val int
}

func NewNode() *Node {
	return &Node{
		val: 0,
	}
}

func main() {
	mp := make(map[int]*Node)

	mp[5] = NewNode()
	x := mp[5]
	x.val = 6

	fmt.Println(mp[5].val, 2)
}
