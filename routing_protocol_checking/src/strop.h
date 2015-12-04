#ifndef __H_STROP_
#define __H_STROP_

#include <vector>
#include <string>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <algorithm>

using namespace std;

typedef unsigned short value_type;
typedef std::vector<value_type> String;

ostream &
operator<< (ostream &os, const String &str);

void
concat(String &lhs, const String &rhs);

bool 
endsWith(const String &str, const String &suffix);

bool 
endsWith(const string &str, const string &suffix);
/* vector<string> split(const string &str, const char *c = "-"); */
String
split(const string &str, const string &delim = "-");

#endif	/* __H_STROP_ */
