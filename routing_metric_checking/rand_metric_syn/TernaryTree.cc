#include "TernaryTree.hh"

namespace TernaryTree {

  std::vector<std::string> unitaryOperators, binaryOperators, ternaryOperators, operands;

  std::set<Node *> internalNodes, leafNodes;

  Node::Node() : parent(nullptr), children(std::vector<Node *>(3, nullptr)), childrenSize(0) {}

  Node::Node(const unsigned short &id) : parent(nullptr), id(id), level(0), children(std::vector<Node *>(3, nullptr)), childrenSize(0) {}

  // Node::Node(const Node &node) : parent(node.parent), id(node.id), children(node.children) {}
  Node::Node(const unsigned short &id, const unsigned long &level) : parent(nullptr), id(id), level(level), children(std::vector<Node *>(3, nullptr)), childrenSize(0) {}

  Node::Node(const Node &node) {
    parent = node.parent;
    id = node.id;
    level = node.level;
    children = node.children;
    childrenSize = node.childrenSize;
  }

  Node::~Node() {
    if (nullptr != parent)
      parent = nullptr;
    for (auto &&child : children) {
      auto pos_internal(internalNodes.find(child)),
	pos_leaf(leafNodes.find(child));
      if (nullptr != child
	  && (pos_internal != internalNodes.end()
	      || pos_leaf != leafNodes.end())) {
	delete child;
	if (pos_internal != internalNodes.end())
	  internalNodes.erase(pos_internal);
	if (pos_leaf != leafNodes.end())
	  leafNodes.erase(pos_leaf);
	child = nullptr;
      }
    }  // std::vector<Node *>().swap(children);
  }

  Node *
  Node::addChild(const unsigned short &id) {
    auto pos(leafNodes.find(children[id]));
    if (pos != leafNodes.end()) {
      leafNodes.erase(pos);
      internalNodes.insert(children[id]);
    }
    if (nullptr != children[id])
      return children[id];
    Node *node = new Node(id, this->level + 1);
    node->parent = this;
    internalNodes.insert(node);
    children[id] = node;
    ++childrenSize;
    return node;
  }

  void
  Node::dump(const std::size_t &level) {
    if (level) {
      std::cout << " ====> " << id << " : " << expr;
    }
    else
      std::cout << id << " : " << expr;
    bool newline(true);
    for (auto &&child : children)
      if (nullptr != child) {
    	child->dump(level + 1);
    for (std::size_t i = 1; i < level; ++i)
      std::cout << "        ";

	newline = false;
      }
    if (newline) {
	puts("\n");
    }
  }

  void
  Node::setExpr(const std::string &str) {
    expr = str;
  }

  unsigned short
  Node::getChildrenSize() const {
    return childrenSize;
  }

  bool
  Node::is_nullptr(const unsigned short &id) const {
    return nullptr == children[id];  
  }

  unsigned long
  Node::getLevel() const {
    return level;
  }

  bool
  Node::is_assignable() const {
    if (nullptr != parent
	&& 2 == parent->childrenSize
	&& 1 != parent->parent->childrenSize)
      return true;
    return false;
  }

  // Node *
  // Node::iterate() const {
  //   // if (nullptr != this) {
  //   for (auto &&child : children)
  //     if (nullptr != child)
  // 	child->iterate();
  // 	// }
  // }

  unsigned short
  Node::getPeerSize() const {
    return (nullptr == parent) ? 0 : parent->childrenSize;
  }

  std::string
  Node::buildExpr() {
    std::string ret("(");
    switch (childrenSize) {
    case 0:
      return expr;
    case 1:
      if (nullptr != children[0]) {
	if (0 == children[0]->childrenSize) {
	  ret += children[0]->buildExpr();
	  ret += expr;
	} else {
	  ret += expr;
	  ret += children[0]->buildExpr();
	}
      }
      else if (nullptr != children[1]) {
	ret += expr;
	ret += children[1]->buildExpr();
      }
      else {
	ret += expr;
	ret += children[2]->buildExpr();
      }
      break;
    case 2: {
      bool is_operator_added(false);
      for (auto &&child : children) {
	if (nullptr != child) {
	  ret += child->buildExpr();
	  if (!is_operator_added) {
	    ret += " ";
	    ret += expr;
	    ret += " ";
	    is_operator_added = true;
	  }
	}
      }
    }
      break;
    case 3:
      ret += children[0]->buildExpr();
      ret += " ? ";
      ret += children[1]->buildExpr();
      ret += " : ";
      ret += children[2]->buildExpr();
      break;
    default:
      break;
    }
    ret += ")";

    return ret;
  }

  void
  Node::swap(const std::size_t &lhs, const std::size_t &rhs) {
    std::swap(children[lhs], children[rhs]);
  }

  Tree::Tree(const unsigned long &max_depth) : root(new Node()), max_depth(max_depth) {
    internalNodes.insert(root);

    // Unitary operators
    unitaryOperators.emplace_back("++");
    unitaryOperators.emplace_back("--");
    unitaryOperators.emplace_back("~");
    unitaryOperators.emplace_back("!");
    // unitaryOperators.emplace_back("+");
    unitaryOperators.emplace_back("-");

    // Binary operators
    binaryOperators.emplace_back("+");
    binaryOperators.emplace_back("-");
    binaryOperators.emplace_back("*");
    // binaryOperators.emplace_back("/");
    // binaryOperators.emplace_back("%");
    binaryOperators.emplace_back("&");
    binaryOperators.emplace_back("|");
    binaryOperators.emplace_back(">");
    binaryOperators.emplace_back("^");
    binaryOperators.emplace_back(">=");
    binaryOperators.emplace_back("==");
    binaryOperators.emplace_back("<");
    binaryOperators.emplace_back("<=");
    binaryOperators.emplace_back("!=");
    // binaryOperators.emplace_back("||");
    // binaryOperators.emplace_back("&&");
    // binaryOperators.emplace_back("<<");
    // binaryOperators.emplace_back(">>");

    // Ternary operators
    ternaryOperators.emplace_back("?:");

  }

  Tree::~Tree() {
    for (auto &&node : internalNodes)
      if (nullptr != node) {
	delete node;
      }
    std::set<Node *>().swap(internalNodes);
    for (auto &&node : leafNodes)
      if (nullptr != node) {
	delete node;
      }
    std::set<Node *>().swap(leafNodes);
  }

  void
  Tree::insert(const std::vector<unsigned short> &trace) {
    auto currentNode(root);
    for (auto iter = trace.begin(); iter != trace.end(); ++iter)
      currentNode = currentNode->addChild(*iter);
    internalNodes.erase(currentNode);
    leafNodes.insert(currentNode);
  }

  void Tree::dump() {
    root->dump(0);
    puts("\n");
  }

  void
  Tree::assignOperators() {
    srand(time(NULL));
    for (auto &&node : internalNodes) {
      switch (node->getChildrenSize()) {
      case 1:
	if (node->getLevel() + 2 < getMaxDepth()) {
	  if (!node->is_nullptr(0))
	    node->swap(0, 1);
	  auto idx(rand() % unitaryOperators.size());
	  if (idx < 2)
	    idx += 2;
	  node->setExpr(unitaryOperators[idx]);
	}
	else {
	  if (!node->is_nullptr(0))
	    node->setExpr(unitaryOperators[rand() % 2]);
	  else
	    node->setExpr(unitaryOperators[rand() % unitaryOperators.size()]);
	}
	break;
      case 2:
	node->setExpr(binaryOperators[rand() % binaryOperators.size()]);
	break;
      case 3:
	node->setExpr(ternaryOperators.front());
      default:
	break;
      }
    }
  }

  void
  Tree::assignOperands(const tMetric &type, const std::size_t &max_size, const unsigned short &argc) {
    srand(time(NULL));
    switch (type) {
    case MULTIHOP: {
      for (auto &&node : leafNodes) {
	std::string str;
	if (node->is_assignable()) {
	  if (rand() % 2) {
	    str += "path";
	    if (2 == argc)
	      str += itoa(rand() % 2);
	    str += "[";
	    str += itoa(rand() % max_size);
	    str += "]";
	  }
	  else
	    str += itoa(rand() % UINT_MAX);
	}
	else {
	  str += "path";
	  if (2 == argc)
	    str += itoa(rand() % 2);
	  str += "[";
	  str += itoa(rand() % max_size);
	  str += "]";
	}
	node->setExpr(str);
      }
    }
      break;
    case GEO: {
      std::string operands[] = {
	"source[0]",
	"source[1]",
	"target[0]",
	"target[1]"
      };
      for (auto &&node : leafNodes) {
	std::string str;
	if (node->is_assignable()) {
	  if (rand() % 2)
	    str = operands[rand() % 4];
	  else
	    str = itoa(rand() % UINT_MAX);
	}
	else
	  str = operands[rand() % 4];
	node->setExpr(str);
      }
    }
      break;
    default:
      break;
    }
  }

  unsigned long
  Tree::getMaxDepth() const {
    return max_depth;
  }

  std::string
  Tree::buildExpr() {
    return root->buildExpr();
  }

} // namespace TernaryTree
