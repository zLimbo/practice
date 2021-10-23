# -*- coding: utf-8 -*-

from mytree import BinaryTree

root = BinaryTree.BinaryTree('root')
b = root.insertRightChild('B')
a = root.insertLeftChild('A')
c = a.insertLeftChild('C')
e = c.insertLeftChild('E')
d = b.insertRightChild('D')
f = d.insertRightChild('F')

root.inOrder()
print()
root.preOrder()
print()
root.postOrder()
print()
