#ifndef __H_BINARY_TREE_
#define __H_BINARY_TREE_

#include <string>
#include <iostream>
#include <vector>
#include "TraceInfo.h"

namespace BinaryTree {

  class Node {
  public:
    Node();

    Node(const TraceInfo &data);

    virtual ~Node();

    /* const Node *insertLeftChild(const Node *&currentNode, const TraceInfo &data); */

    void insertLeftChild(const TraceInfo &traceInfo, const unsigned &level = 0);

    /* const Node *insertRightChild(const Node *&currentNode, const TraceInfo &data); */

    void insertRightChild(const TraceInfo &traceInfo, const unsigned &level = 0);

  private:
    Node *parent;
    Node *leftChild;
    Node *rightChild;
    TraceInfo data;
  };
  
  class Tree {
  public:
    Tree();

    virtual ~Tree();

    void insertLeftChild(const TraceInfo &traceInfo);

    void insertRightChild(const TraceInfo &traceInfo);

  private:
    Node *root;
  };


  /* typedef const Node* NodePtr; */
} /* namespace BinaryTree */

#endif	/* __H_BINARY_TREE_ */
