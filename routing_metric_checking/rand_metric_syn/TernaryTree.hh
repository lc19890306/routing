#ifndef TERNARYTREE_HH
#define TERNARYTREE_HH

#include "string.hh"
#include "definitions.hh"

#include <vector>
#include <set>
#include <iostream>
#include <climits>

namespace TernaryTree {

  class Node {
  public:
    Node();
    Node(const unsigned short &id);
    Node(const unsigned short &id, const unsigned long &level);
    Node(const Node &node);
    ~Node();
    Node * addChild(const unsigned short &id);
    void dump(const std::size_t &level);
    void setExpr(const std::string &str);
    unsigned short getChildrenSize() const;
    bool is_nullptr(const unsigned short &id) const;
    unsigned long getLevel() const;
    // Node * iterate() const;
    unsigned short getPeerSize() const;
    std::string buildExpr();
    bool is_assignable() const;
    void swap(const std::size_t &lhs, const std::size_t &rhs);

  private:
    Node *parent;
    unsigned short id;
    unsigned long level;
    std::string expr;
    std::vector<Node *> children;
    unsigned short childrenSize;
  };

  class Tree {
  public:
    // Tree();
    Tree(const unsigned long &max_depth);
    ~Tree();
    void insert(const std::vector<unsigned short> &trace);
    void dump();
    void assignOperators();
    void assignOperands(const tMetric &type, const std::size_t &max_size, const unsigned short &argc);
    unsigned long getMaxDepth() const;
    std::string buildExpr();

  private:
    Node *root;
    unsigned long max_depth;
  };

} // namespace TernaryTree

#endif	// TERNARYTREE_HH
