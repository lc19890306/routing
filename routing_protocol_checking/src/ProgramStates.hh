#ifndef __HH_PROGRAM_STATES_
#define __HH_PROGRAM_STATES_

#include "ProgramState.hh"

#include <cstring>

class ProgramState1 : public ProgramState {
public:
  ProgramState1(const Path<int> &p, 
  		const int &res, 
  		const std::size_t &i,
  		const std::size_t &row,
  		const std::size_t &col,
		const std::vector<std::vector<Path<int> > > &path,
		const Path<int> &tmp) :
    p(p),
    res(res),
    i(i),
    row(row),
    col(col),
    tmp(tmp) {

    this->path.resize(path.size());
    for (std::size_t row = 0; row != path.size(); ++row) {
      for (std::size_t col = 0; col != path[row].size(); ++col)
	this->path[row].push_back(path[row][col]);
    }
    // for (auto &&row : path)
    //   this->path.push_back(row);
    // std::memcpy(this->path, path, sizeof(path[0][0]) * maxnum * maxnum);
    // std::memcpy(this->s, s, sizeof(s[0]) * maxnum);
  }

  ~ProgramState1() {
    Path<int> emptyPath;
    std::swap(emptyPath, this->p);
    for (auto &&row : path)
      for (auto &&col : row) {
	Path<int> emptyPath;
	std::swap(emptyPath, col);
      }
  }

  // ProgramState1(const ProgramState1 &ps) : 
  //   p(ps.p),
  //   res(ps.res),
  //   i(ps.i),
  //   j(ps.j) {
  //   std::memcpy(path, ps.path, sizeof(ps.path[0][0]) * maxnum * maxnum);
  //   std::memcpy(s, ps.s, sizeof(ps.s[0]) * maxnum);
  // }

  Path<int> p;
  int res;
  std::size_t i;
  std::size_t row;
  std::size_t col;
  std::vector<std::vector<Path<int> > > path;
  Path<int> tmp;
};

class ProgramState2 : public ProgramState {
public:
  ProgramState2(const Path<int> &p, 
		const int &res, 
		const unsigned &i,
		const unsigned &j,
		const std::vector<std::vector<Path<int> > > &path,
		const std::bitset<maxnum> &s,
		const unsigned &u,
		const unsigned &x,
		const unsigned &y,
		const Path<int> &tmp) :
    p(p),
    res(res),
    i(i),
    j(j),
    s(s),
    u(u),
    x(x),
    y(y),
    tmp(tmp) {
    // for (int i = 0; i < maxnum; ++i) {
    //   this->s[i] = s[i];
    //   for (int j = 0; j < maxnum; ++j)
    // 	this->path[i][j] = path[i][j];
    // }

    this->path.resize(path.size());
    for (std::size_t row = 0; row != path.size(); ++row) {
      for (std::size_t col = 0; col != path[row].size(); ++col)
	this->path[row].push_back(path[row][col]);
    }

    // std::memcpy(this->path, path, sizeof(path[0][0]) * maxnum * maxnum);
    // std::memcpy(this->s, s, sizeof(s[0]) * maxnum);
  }

  ~ProgramState2() {
    Path<int> emptyPath;
    std::swap(emptyPath, this->p);
    for (auto &&row : path)
      for (auto &&col : row) {
	Path<int> emptyPath;
	std::swap(emptyPath, col);
      }
  }

  // ProgramState2(const ProgramState2 &ps) :
  //   p(ps.p),
  //   res(ps.res),
  //   i(ps.i),
  //   j(ps.j),
  //   u(ps.u),
  //   x(ps.x),
  //   y(ps.y),
  //   tmp(ps.tmp) {
  //   std::memcpy(path, ps.path, sizeof(ps.path[0][0]) * maxnum * maxnum);
  //   std::memcpy(s, ps.s, sizeof(ps.s[0]) * maxnum);
  // }

  Path<int> p;
  int res;
  unsigned i;
  unsigned j;
  std::vector<std::vector<Path<int> > > path;
  std::bitset<maxnum> s;
  unsigned u;
  unsigned x;
  unsigned y;
  Path<int> tmp;
};

#endif	// __HH_PROGRAM_STATES_
