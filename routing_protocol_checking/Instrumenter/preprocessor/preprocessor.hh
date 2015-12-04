#ifndef PREPROCESSOR_HH
#define PREPROCESSOR_HH

#include "preprocprogram.hh"
#include "chkptprogram.hh"

#include <iostream>
#include <cctype>
#include <stack>

bool cmpFuncExists(const std::string &stmt, const std::size_t &pos);

std::size_t matchSqrBracket(const std::string &stmt, const std::size_t &pos);

int getArgPath(const std::string &stmt, const std::string &varName, const std::size_t &pos, std::string &var);

void preprocess(const std::string &filename, const std::string &funcName, const std::string &varName = "path");

#endif	// PREPROCESSOR_HH
