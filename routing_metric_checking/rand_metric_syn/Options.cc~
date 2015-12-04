#include "Options.hh"

Options::Options(int argc, char **argv) : depth(5), max_size(3), type(MULTIHOP) {
  for (std::size_t i = 1; i != argc; ++i) {
    std::string arg(argv[i]);

    if (startswith(arg, "--depth=")) {
      arg = arg.substr(arg.find("=") + 1);
      try {
	depth = atoi(arg.c_str());
      } catch (std::exception &e) {
	std::cerr << e.what() << std::endl << "Error: faulty depth provided!\n";
	exit(1);
      }
      if (depth == 0 || depth >= UINT_MAX) {
	std::cerr << "Error: faulty depth provided!\n";
	exit(1);
      }
      continue;
    }

    if (startswith(arg, "--max-size=")) {
      arg = arg.substr(arg.find("=") + 1);
      try {
	max_size = atoi(arg.c_str());
      } catch (std::exception &e) {
	std::cerr << e.what() << std::endl << "Error: faulty max size provided!\n";
	exit(1);
      }
      if (max_size == 0 || max_size >= UINT_MAX) {
	std::cerr << "Error: faulty max size provided!\n";
	exit(1);
      }
      continue;
    }

    if (startswith(arg, "--metric-type=")) {
      arg = arg.substr(arg.find("=") + 1);
      try {
	type = static_cast<tMetric>(atoi(arg.c_str()));
      } catch (std::exception &e) {
	std::cerr << e.what() << std::endl << "Error: faulty metric type provided!\n";
	exit(1);
      }
      if (type < 0 || type >= tMetric::SIZE) {
	std::cerr << "Error: faulty metric type provided!\n";
	exit(1);
      }
      continue;
    }

    std::cerr << "Error: faulty arguments provided!\n";
    exit(1);
  }
}

std::size_t
Options::getDepth() const {
  return depth;
}

std::size_t
Options::getMaxSize() const {
  return max_size;
}

tMetric
Options::getMetricType() const {
  return type;
}
