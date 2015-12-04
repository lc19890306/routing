#include "strop.h"

ostream &
operator<< (ostream &os, const String &str) {
  auto it(str.begin());
  for (; it != str.end() - 1; ++it)
    os << *it << "-";
  os << *it;
  return os;
}

void
concat(String &lhs, const String &rhs) {
  for (auto &&element : rhs)
    lhs.push_back(element);
}

String
operator+ (const String &lhs, const String &rhs) {
  String ret(lhs);
  concat(ret, rhs);
  return ret;
}

bool 
endsWith(const String &str, const String &suffix) {
  if (str.empty())
    return false;
  if (str.size() < suffix.size())
    return false;
  if (std::distance(str.begin(), std::find_end(str.begin(), str.end(), suffix.begin(), suffix.end())) == str.size() - suffix.size())
    return true;
  return false;
}

bool endsWith(const string &str, const string &suffix) {
  if (str.empty())
    return false;
  if (str.size() < suffix.size())
    return false;
  if (str.rfind(suffix) == str.size() - suffix.size())
    return true;
  return false;
}

String split(const string &str, const string &delim) {
  // static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  // pthread_mutex_lock(&lock);

  String ret;
  std::string this_str(str);
  std::size_t begin(0);
  std::size_t end(this_str.find(delim));
  string temp;
  while (end != std::string::npos) {
    temp = string(this_str.begin() + begin, this_str.begin() + end);
    ret.push_back(atoi(temp.c_str()));
    begin = end + 1;
    end = this_str.find(delim, begin);
  }
  temp = string(this_str.begin() + begin, this_str.end());
  ret.push_back(atoi(temp.c_str()));

  return ret;
}

// vector<string> split(const string &str, const char *c) {
//   static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//   pthread_mutex_lock(&lock);
//   char *cstr = NULL, *p = NULL;
//   vector<string> res = vector<string>(0);
//   cstr = new char[str.size()+1];
//   strcpy(cstr, str.c_str());
//   p = strtok(cstr, c);
//   while(p != NULL) {
//     string temp(p);
//     res.push_back(temp);
//     p = strtok(NULL, c);
//   }
//   delete []cstr;
//   pthread_mutex_unlock(&lock);
 
//   return res;
// }
