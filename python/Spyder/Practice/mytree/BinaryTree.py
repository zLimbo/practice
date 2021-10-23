# -*- coding: utf-8 -*-

class BinaryTree:
    
    def __init__(self, value):
        self.__left = None
        self.__right = None
        self.__data = value
        
    def insertLeftChild(self, value):
        if self.__left:
            print('left child tree already exist.')
        else:
            self.__left = BinaryTree(value)
            return self.__left
        
    def insertRightChild(self, value):
        if self.__right:
            print('right child tree already exist.')
        else:
            self.__right = BinaryTree(value)
            return self.__right
        
    def show(self):
        print(self.___data)
        
    def preOrder(self):
        print(self.__data, end=' ')
        if self.__left:
            self.__left.preOrder()
        if self.__right:
            self.__right.preOrder()
            
    def postOrder(self):
        if self.__left:
            self.__left.postOrder()
        if self.__right:
            self.__right.postOrder()
        print(self.__data, end=' ')
        
    def inOrder(self):
        if self.__left:
            self.__left.inOrder()
        print(self.__data, end=' ')
        if self.__right:
            self.__right.inOrder()
            
            
if __name__ == '__main__':
    print('Please use me as a module')