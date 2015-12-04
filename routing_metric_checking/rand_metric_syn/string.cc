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
  std::size_t pos = ret.rfind(str);
  ret.erase(pos, str.size());

  return ret;
}

void strip(std::string &str, const std::string &target) {
  std::size_t start(str.find_first_not_of(target)),
    end(str.find_last_not_of(target));
  if (start == std::string::npos
      || end == std::string::npos) {
    str = std::string();
    return;
  }
  str = str.substr(start, end - start + 1);
}

void lstrip(std::string &str, const std::string &target) {
  std::size_t start(str.find_first_not_of(target)),
    end(str.size());
  if (start == std::string::npos) {
    str = std::string();
    return;
  }
  str = str.substr(start, end - start + 1);
}

void rstrip(std::string &str, const std::string &target) {
  std::size_t start(0),
    end(str.find_last_not_of(target));
  if (end == std::string::npos) {
    str = std::string();
    return;
  }
  str = str.substr(start, end - start + 1);
}

std::string itoa(const unsigned &n) {
  std::stringstream ss;
  ss << n;
  std::string ret(ss.str());
  return ret;
}

std::size_t find(const std::string &stmt, const std::string &str) {
  auto pos_lfirst = stmt.find(str);
  auto pos_rfirst = pos_lfirst + str.size();
  if (pos_lfirst != std::string::npos
      && !isalnum(stmt[pos_lfirst - 1])
      && stmt[pos_lfirst - 1] != '_'
      && !isalnum(stmt[pos_rfirst])
      && stmt[pos_rfirst] != '_') {
    if ('(' == stmt[stmt.find_last_not_of(" \t", pos_lfirst - 1)])
      return std::string::npos;
    else {
      std::size_t pos = stmt.find_last_not_of(" \t", pos_lfirst - 1);
      if ('*' == stmt[pos])
	return find(stmt, stmt.substr(pos, pos_rfirst - pos));
    }
    return pos_lfirst;
  }
  else if (pos_lfirst != std::string::npos)
    return find(stmt.substr(pos_lfirst + 1), str);
  else
    return pos_lfirst;
}

std::size_t rfind(const std::string &stmt, const std::string &str, std::size_t pos) {
  if (pos == 0)
    pos = stmt.size() - 1;
  auto pos_lstr = stmt.rfind(str, pos);
  auto pos_rstr = pos_lstr + str.size();
  std::string space(stmt.substr(pos_rstr, pos - pos_rstr + 1));
  strip(space, " \t");
  if (space.empty())
    return pos_lstr;
  else
    return std::string::npos;
}

std::size_t matchSqrBracket(const std::string &stmt, const std::size_t &pos) {
  std::size_t ret(0);
  std::stack<std::size_t> stk_sqrBracket;
  stk_sqrBracket.push(pos);
  std::size_t i(pos + 1);
  while (!stk_sqrBracket.empty()) {
    switch (stmt[i]) {
    case '[':
      stk_sqrBracket.push(i);
      break;
    case ']': {
      stk_sqrBracket.pop();
      ret = i;
    }
      break;
    default:
      break;
    }
    ++i;
  }
  return ret;
}

std::size_t matchAngleBracket(const std::string &stmt, const std::size_t &pos) {
  std::size_t ret(0);
  std::stack<std::size_t> stk_angleBracket;
  stk_angleBracket.push(pos);
  std::size_t i(pos + 1);
  while (!stk_angleBracket.empty()) {
    switch (stmt[i]) {
    case '<':
      stk_angleBracket.push(i);
      break;
    case '>': {
      stk_angleBracket.pop();
      ret = i;
    }
      break;
    default:
      break;
    }
    ++i;
  }
  return ret;
}

std::size_t count(const std::string &str, const std::string &key) {
  std::size_t counter(0), index(str.find(key));
  while (index != std::string::npos) {
    ++counter;
    index = str.find(key, index + key.size());
  }

  return counter;
}
