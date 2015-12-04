#include "RandMetric.hh"

RandMetric::RandMetric(const tMetric &type, const std::size_t &depth, const std::size_t &max_size) : type(type), depth(depth), max_size(max_size) {
  TernaryTree::Tree tree(generateTree());
  auto max_size_str(itoa(max_size));
  switch (type) {
  case MULTIHOP: {
    program.emplace_back("\n");
    program.emplace_back("int weight1(int path[");
    program.emplace_back(max_size_str);
    program.emplace_back("]) {\n");
    program.emplace_back("return ");
    program.emplace_back(generateExprTree(tree, 1));
    program.emplace_back(";\n");
    program.emplace_back("}\n\n");
    program.emplace_back("int weight2(int path0[");
    program.emplace_back(max_size_str);
    program.emplace_back("], int path1[");
    program.emplace_back(max_size_str);
    program.emplace_back("]) {\n");
    program.emplace_back("return ");
    program.emplace_back(generateExprTree(tree, 2));
    program.emplace_back(";\n");
    program.emplace_back("}\n\n");
  }
    break;
  case GEO: {
    program.emplace_back("int weight(unsigned source[2], unsigned target[2]) {\n");
    program.emplace_back("return ");
    program.emplace_back(generateExprTree(tree, 1));
    program.emplace_back(";\n");
    program.emplace_back("}\n\n");
  }
    break;
  default:
    break;
  }
}

RandMetric::~RandMetric() {
  std::list<std::string>().swap(program);
  std::map<unsigned long, std::vector<unsigned short> >().swap(traces);
}

std::list<std::string>
RandMetric::getProgram() const {
  return program;
}

std::size_t
RandMetric::getDepth() const {
  return depth;
}

std::size_t
RandMetric::getMaxSize() const {
  return max_size;
}

tMetric
RandMetric::getMetricType() const {
  return type;  
}

TernaryTree::Tree
RandMetric::generateTree() {
  srand(time(NULL));
  unsigned mod(1 << depth);
  std::size_t maxTracesSize(2 * depth);
  while (traces.size() != maxTracesSize) {
    auto num(rand() % mod);
    auto size(rand() % depth);
    while (!size)
      size = rand() % depth;
    traces.insert(std::make_pair(num, decToTernary(num, size)));
  }

  TernaryTree::Tree tree(depth);
  for (auto &&pair : traces) {
    // std::cout << pair.first << ": ";
    tree.insert(pair.second);
    // for (auto && num: pair.second)
    //   std::cout << num;
    // std::cout << std::endl;
  }

  tree.assignOperators();
  // tree.dump();
  return tree;
}

std::string
RandMetric::generateExprTree(TernaryTree::Tree &tree, const unsigned short &argc) {
  tree.assignOperands(type, max_size, argc);
  return tree.buildExpr();
}

std::vector<unsigned short>
RandMetric::decToTernary(unsigned long num, const std::size_t &size) {
  std::vector<unsigned short> ret;
  if (!num) {
    while (ret.size() != size)
      ret.push_back(0);
    return ret;
  }
  while (num) {
    ret.push_back(num % 3);
    num /= 3;
  }
  while (ret.size() < size)
    ret.push_back(0);

  return ret;
}
