#include "BinaryTree.hh"

namespace BinaryTree {
  Node::Node() : parent(NULL), leftChild(NULL), rightChild(NULL) {}

  Node::Node(const TraceInfo &data) : parent(NULL), leftChild(NULL), rightChild(NULL), data(data) {}

  Node::~Node() {
    if (NULL != leftChild)
      delete leftChild;
    if (NULL != rightChild)
      delete rightChild;
    leftChild = NULL;
    rightChild = NULL;
  }

  // const Node *Node::insertLeftChild(const Node *&currentNode, const TraceInfo &data) {
  //   Node *node = new Node(data);
  //   currentNode->leftChild = node;

  //   return node;
  // }

  void Node::insertLeftChild(const TraceInfo &traceInfo, const unsigned &level) {
    std::vector<int> positions = traceInfo.getCmpVal();
    if (level == positions.size()) {
      this->leftChild = new Node(traceInfo);
      return;
    }
    if (-1 == positions[level])
      insertLeftChild(traceInfo, level + 1);
    if (1 == positions[level])
      insertRightChild(traceInfo, level + 1);
  }

  void Node::insertRightChild(const TraceInfo &traceInfo, const unsigned &level) {
    std::vector<int> positions = traceInfo.getCmpVal();
    if (level == positions.size()) {
      this->rightChild = new Node(traceInfo);
      return;
    }
    if (-1 == positions[level])
      insertLeftChild(traceInfo, level + 1);
    if (1 == positions[level])
      insertRightChild(traceInfo, level + 1);
  }

  // const Node *Node::insertRightChild(const Node *&currentNode, const TraceInfo &data) {
  //   Node *node = new Node(data);
  //   currentNode->rightChild = node;

  //   return node;
  // }

  Tree::Tree() : root(new Node()) {}

  Tree::~Tree() {
    if (NULL != root)
      delete root;
    root = NULL;
  }

  void Tree::insertLeftChild(const TraceInfo &traceInfo) {
    if (NULL == root) {
      std::cout << "Fault!\n";
      return;
    }
    root->insertLeftChild(traceInfo);
  }

  void Tree::insertRightChild(const TraceInfo &traceInfo) {
    if (NULL == root) {
      std::cout << "Fault!\n";
      return;
    }
    root->insertRightChild(traceInfo);
  }

} // namespace BinaryTree
