#ifndef GRIPP_HH
#define GRIPP_HH

#include "relational_db.h"
#include "strop.h"

struct index_t {
  // std::string node;
  std::size_t pre;
  std::size_t post;

  friend inline bool 
  operator == (const index_t &lhs, const index_t &rhs) {
    return lhs.pre == rhs.pre && lhs.post == rhs.post;
  }

  friend inline bool 
  operator != (const index_t &lhs, const index_t &rhs) {
    return !(lhs == rhs);
  }
};

struct used_node_t {
  used_node_t(const String &node, const std::size_t &pre, const std::size_t &post) : node(node), pre(pre), post(post) {}

  friend inline bool 
  operator< (const used_node_t &lhs, const used_node_t &rhs) {
    return lhs.pre < rhs.pre ? : (lhs.pre == rhs.pre ? lhs.node < rhs.node : false);
  }

  String node;
  std::size_t pre;
  std::size_t post;
};

#endif	// GRIPP_HH
