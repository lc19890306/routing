#include "TestProgram.hh"

TestProgram::TestProgram(const RandMetric &metric) : max_size(metric.getMaxSize()), type(metric.getMetricType()) {
  fout.open("test.c", std::ios_base::out);
  if (type == GEO)
    program.emplace_back("#define PHI 0\n\n");
  auto metricProgram(metric.getProgram());
  program.insert(program.end(), metricProgram.begin(), metricProgram.end());
  program.emplace_back("int main() {\n");
  insertTestStmts();
  program.emplace_back("return 0;\n");
  program.emplace_back("}\n");
}

TestProgram::~TestProgram() {
  writeIntoFile();
  fout.close();
  std::list<std::string>().swap(program);
}

void
TestProgram::insertTestStmts() {
  std::string max_size_str(itoa(max_size));
  switch (type) {
  case MULTIHOP: {
    for (std::size_t i = 0; i != 3; ++i) {
      program.emplace_back("int path");
      program.emplace_back(itoa(i));
      program.emplace_back("[");
      program.emplace_back(max_size_str);
      program.emplace_back("];\n");
    }

    program.emplace_back("\n");

    program.emplace_back("__ESBMC_assume(");
    for (std::size_t i = 0; i != max_size; ++i) {
      for (std::size_t j = 0; j != 3; ++j) {
	std::string str_i(itoa(i)), str_j(itoa(j));
	program.emplace_back("0 < path");
	program.emplace_back(str_j);
	program.emplace_back("[");
	program.emplace_back(str_i);
	program.emplace_back("] && path");
	program.emplace_back(str_j);
	program.emplace_back("[");
	program.emplace_back(str_i);
	program.emplace_back("] < 100");
	if (i + j != max_size + 1)
	  program.emplace_back("\n&& ");
      }
    }
    program.emplace_back(");\n");

    program.emplace_back("\n");

    const std::string argv[] = {
      "path0",
      "path1",
      "path0, path2",
      "path1, path2",
      "path2, path0",
      "path2, path1"
    };
    for (std::size_t i = 0; i != 6; ++i) {
      program.emplace_back("int res");
      program.emplace_back(itoa(i));
      program.emplace_back(" = weight");
      program.emplace_back(itoa(count(argv[i], "path")));
      program.emplace_back("(");
      program.emplace_back(argv[i]);
      program.emplace_back(");\n");
    }

    program.emplace_back("\n");

    left_isotonicity_check("res0", "res1", "res4", "res5");
    // right_isotonicity_check("res0", "res1", "res2", "res3");
    // left_monotonicity_check("res0", "res4");
    // right_monotonicity_check("res0", "res2");
  }
    break;
  case GEO: {
    program.emplace_back("unsigned source[2], target0[2], target1[2], path[");
    program.emplace_back(max_size_str);
    program.emplace_back("][2];\n");

    program.emplace_back("\n");

    program.emplace_back("__ESBMC_assume(");
    for (std::size_t i = 0; i != 2; ++i) {
      std::string str_i(itoa(i));
      program.emplace_back("source[");
      program.emplace_back(str_i);
      program.emplace_back("] < 100\n&& ");
      program.emplace_back("target0[");
      program.emplace_back(str_i);
      program.emplace_back("] < 100\n&& ");
      program.emplace_back("target1[");
      program.emplace_back(str_i);
      program.emplace_back("] < 100");
      if (i != 1)
	program.emplace_back("\n&& ");
    }
    program.emplace_back(");\n");

    program.emplace_back("\n");

    program.emplace_back("for (unsigned i = 0; i != ");
    program.emplace_back(max_size_str);
    program.emplace_back("; ++i)\n");
    program.emplace_back("__ESBMC_assume(");
    program.emplace_back("path[i][0] < 100 && path[i][1] < 100);\n");

    program.emplace_back("\n");

    // odd_symmetry_check();
    // transitivity_check(max_size_str);
    strict_order_check();
  }
    break;
  default:
    break;
  }
}

void
TestProgram::left_isotonicity_check(std::string &&res0, std::string &&res1, std::string &&res4, std::string &&res5) {
  program.emplace_back("__ESBMC_assume(");
  program.emplace_back(res0);
  program.emplace_back(" > ");
  program.emplace_back(res1);
  program.emplace_back(");\n");
  program.emplace_back("__ESBMC_assert(");
  program.emplace_back(res4);
  program.emplace_back(" >= ");
  program.emplace_back(res5);
  program.emplace_back(", \"left-isotonicity violated!\\n\");\n");
  program.emplace_back("\n");
}
  
void
TestProgram::right_isotonicity_check(std::string &&res0, std::string &&res1, std::string &&res2, std::string &&res3) {
  program.emplace_back("__ESBMC_assume(");
  program.emplace_back(res0);
  program.emplace_back(" > ");
  program.emplace_back(res1);
  program.emplace_back(");\n");
  program.emplace_back("__ESBMC_assert(");
  program.emplace_back(res2);
  program.emplace_back(" >= ");
  program.emplace_back(res3);
  program.emplace_back(", \"right-isotonicity violated!\\n\");\n");
  program.emplace_back("\n");
}

void
TestProgram::left_monotonicity_check(std::string &&res0, std::string &&res4) {
  program.emplace_back("__ESBMC_assert(");
  program.emplace_back(res4);
  program.emplace_back(" >= ");
  program.emplace_back(res0);
  program.emplace_back(", \"left-monotonicity violated!\\n\");\n");
  program.emplace_back("\n");
}
  
void
TestProgram::right_monotonicity_check(std::string &&res0, std::string &&res2) {
  program.emplace_back("__ESBMC_assert(");
  program.emplace_back(res2);
  program.emplace_back(" >= ");
  program.emplace_back(res0);
  program.emplace_back(", \"right-monotonicity violated!\\n\");\n");
  program.emplace_back("\n");
}

void
TestProgram::odd_symmetry_check() {
  program.emplace_back("__ESBMC_assume(weight(source, target1) <= PHI);\n");
  program.emplace_back("__ESBMC_assert(weight(target1, source) >= PHI, \"Odd symmetry violated!\\n\");\n");
  program.emplace_back("\n");
}

void
TestProgram::transitivity_check(const std::string &max_size_str) {
  program.emplace_back("for (unsigned i = 0; i != ");
  program.emplace_back(max_size_str);
  program.emplace_back(" - 1; ++i)\n");
  program.emplace_back("__ESBMC_assume(weight(path[i], path[i + 1]) <= PHI);\n");
  program.emplace_back("__ESBMC_assert(weight(path[0], path[");
  program.emplace_back(max_size_str);
  program.emplace_back(" - 1]) <= PHI, \"Transitivity violated!\\n\");\n");
  program.emplace_back("\n");
}

void
TestProgram::strict_order_check() {
  program.emplace_back("__ESBMC_assume(source[0] != target1[0] || source[1] != target1[1]);\n");
  program.emplace_back("__ESBMC_assume(target0[0] != target1[0] || target0[1] != target1[1]);\n");
  program.emplace_back("__ESBMC_assert(weight(source, target0) != weight(source, target1), \"Strict order violated!\\n\");\n");
  program.emplace_back("\n");
}

void
TestProgram::print() {
  for (auto &&stmt : program)
    std::cout << stmt;
}

void
TestProgram::writeIntoFile() {
  for (auto &&stmt : program)
    fout << stmt;
}
