#include "string.hh"

bool startswith(const std::string &str, const std::string &prefix) {
  if (str.empty())
    return false;
  // assert(str.size() > prefix.size()
// && "str.size() should be greater than prefix.size()");
  return (0 == str.find(prefix)) ? true : false;
}

bool endswith(const std::string &str, const std::string &suffix) {
  if (str.empty())
    return false;
  // assert(str.size() > suffix.size()
// && "str.size() should be greater than suffix.size()");
  return ((str.size() - suffix.size()) == str.rfind(suffix)) ? true : false;
}

std::string createNewFileName(const std::string &filename, const std::string &str) {
  std::string ret(filename);
  std::size_t pos = ret.rfind('.');
  ret.insert(pos, str);

  return ret;
}

void strip(std::string &str, const std::string &target) {
  std::size_t start(str.find_first_not_of(target)),
    end(str.find_last_not_of(target));
  if (start == std::string::npos
      || end == std::string::npos)
    return;
  str = str.substr(start, end - start + 1);
}

void lstrip(std::string &str, const std::string &target) {
  std::size_t start(str.find_first_not_of(target)),
    end(str.size());
  if (start == std::string::npos)
    return;
  str = str.substr(start, end - start + 1);
}

void rstrip(std::string &str, const std::string &target) {
  std::size_t start(0),
    end(str.find_last_not_of(target));
  if (end == std::string::npos)
    return;
  str = str.substr(start, end - start + 1);
}

std::string itoa(const unsigned &n) {
  std::stringstream ss;
  ss << n;
  std::string ret(ss.str());
  return ret;
}
