#ifndef STRING_HH
#define STRING_HH

#include <string>
#include <cassert>
#include <sstream>
#include <cstring>
#include <cstdlib>

// bool startswith(const std::string &str, const std::string &prefix);

// bool endswith(const std::string &str, const std::string &suffix);

// std::string createNewFileName(const std::string &filename, const std::string &str);

// void strip(std::string &str, const std::string &target);

// void lstrip(std::string &str, const std::string &target);

// void rstrip(std::string &str, const std::string &target);

std::string itoa(const unsigned &n) {
  std::stringstream ss;
  ss << n;
  std::string ret(ss.str());
  return ret;
}

#endif	// STRING_HH
