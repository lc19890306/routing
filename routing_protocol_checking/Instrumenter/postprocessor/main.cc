#include "postprocessor.hh"

int main(int argc, char **argv) {
  if (argc > 2)
    std::cout << "Error: too many arguments provided!\n";

  std::string filename(argv[1]);
  if (!endswith(filename, "prep.c")
      && !endswith(filename, "prep.cc")
      && !endswith(filename, "prep.cpp")
      && !endswith(filename, "prep.cxx")) {
    std::cout << "Error: wrong file type!\n";
    return 1;
  }
  postprocess(filename, "Dijkstra");

  return 0;
}
