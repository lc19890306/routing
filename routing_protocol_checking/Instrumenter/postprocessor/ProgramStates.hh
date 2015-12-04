#ifndef PROGRAMSTATES_HH
#define PROGRAMSTATES_HH

#include "ProgramState.hh"

#include <cstring>

class ProgramState0 : public ProgramState {
public:
ProgramState0(const unsigned &i,
const unsigned &j,
const Path<int> &p,
const Path<int> path[21][21],
const int &res,
const bool s[21],
const Path<int> &tmp,
const unsigned &u,
const unsigned &x,
const unsigned &y) {
this->i = i;
this->j = j;
this->p = p;
std::memcpy(this->path, path, sizeof(Path<int>) * 21 * 21);
this->res = res;
std::memcpy(this->s, s, sizeof(bool) * 21);
this->tmp = tmp;
this->u = u;
this->x = x;
this->y = y;
}

unsigned i;
unsigned j;
Path<int> p;
Path<int> path[21][21];
int res;
bool s[21];
Path<int> tmp;
unsigned u;
unsigned x;
unsigned y;
};

#endif
