#include "preprocessor.hh"
#include "string.hh"

int main(int argc, char **argv) {
  if (argc > 2)
    std::cout << "Error: too many arguments provided!\n";

  std::string filename(argv[1]);
  if (!endswith(filename, ".c")
      && !endswith(filename, ".cc")
      && !endswith(filename, ".cpp")
      && !endswith(filename, ".cxx")) {
    std::cout << "Error: wrong file type!\n";
    return 1;
  }
  preprocess(filename, "Dijkstra");

  return 0;
}
