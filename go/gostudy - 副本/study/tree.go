package main

import "fmt"

type TreeNode struct {
	val   int64
	left  *TreeNode
	right *TreeNode
}

func NewTreeNode(val int64, left *TreeNode, right *TreeNode) *TreeNode {
	return &TreeNode{
		val:   val,
		left:  left,
		right: right,
	}
}

func (node *TreeNode) InOrder(data []int64) []int64 {
	if node == nil {
		return data
	}
	data = node.left.InOrder(data)
	data = append(data, node.val)
	data = node.right.InOrder(data)
	return data
}

func main() {

	root := NewTreeNode(0, nil, nil)
	root.left = NewTreeNode(-1, nil, nil)
	root.right = NewTreeNode(1, nil, nil)

	data := make([]int64, 0)
	data = root.InOrder(data)

	fmt.Println("in-order:", data)
}
