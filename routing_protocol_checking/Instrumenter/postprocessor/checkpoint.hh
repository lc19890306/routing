#ifndef CHECKPOINT_HH
#define CHECKPOINT_HH

#include "path.h"
#include "TraceInfo.h"
#include "ProgramState.hh"
#include "ProgramStates.hh"

#include <pthread.h>
#include <typeinfo>
#include <cassert>

void checkpoint(const Path<int> &p, 
Graph* graph, 
unsigned &i, 
unsigned &j, 
const unsigned &n, 
Path<int> path[21][21], 
int &res, 
bool s[21], 
TraceInfo &this_traceInfo, 
Path<int> &tmp, 
unsigned &u, 
const unsigned &v, 
unsigned &x, 
unsigned &y, 
const unsigned &label
) {
switch (this_traceInfo.getJob()) {
case RECORD: {
switch (label) {
case 6:
case 13:
{
ProgramState0 *ps = new ProgramState0(i, j, p, path, res, s, tmp, u, x, y);
this_traceInfo.setPSPtr(ps);
this_traceInfo.setLabel(label);
if ((0 <= graph->matrix[p.head()][p.rear()] && graph->matrix[p.head()][p.rear()] < inf) || p.is_checked())
return;
ps->p.check();
p.check();
TraceInfo traceInfo(this_traceInfo);
traceInfo.setJob(RESUME);
pthread_mutex_lock(&graph->v_psLock[p.head()][p.rear()]);
graph->v_tracePoints[p.head()][p.rear()].push_back(traceInfo);
pthread_mutex_unlock(&graph->v_psLock[p.head()][p.rear()]);
}
break;
default: {
if ((0 <= graph->matrix[p.head()][p.rear()] && graph->matrix[p.head()][p.rear()] < inf) || p.is_checked())
return;
p.check();
ProgramState0 *ps = new ProgramState0(i, j, p, path, res, s, tmp, u, x, y);
ps->setGCFlag();
TraceInfo traceInfo(this_traceInfo);
traceInfo.setPSPtr(ps);
traceInfo.setLabel(label);
traceInfo.setJob(RESUME);
pthread_mutex_lock(&graph->v_psLock[p.head()][p.rear()]);
graph->v_tracePoints[p.head()][p.rear()].push_back(traceInfo);
pthread_mutex_unlock(&graph->v_psLock[p.head()][p.rear()]);
}
break;
}
}
break;

case RESUME: {
this_traceInfo.setJob(RECORD);
ProgramState0 *ps = NULL;
try {
ps = static_cast<ProgramState0 *ps *>(this_traceInfo.getPSPtr());
} catch (const std::bad_cast &e) {
std::cout << e.what() << std::endl << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << std::endl;
}

assert(NULL != ps && "NULL Pointer!\n");
i = ps->i;
j = ps->j;
p = ps->p;
for (std::size_t idx0 = 0; idx0 != 21; ++idx0) {
for (std::size_t idx1 = 0; idx1 != 21; ++idx1) {
path[idx0][idx1] = ps->path[idx0][idx1];
}
}
res = ps->res;
for (std::size_t idx0 = 0; idx0 != 21; ++idx0) {
s[idx0] = ps->s[idx0];
}
tmp = ps->tmp;
u = ps->u;
x = ps->x;
y = ps->y;

ps = NULL;
}
break;

default:
break;
}
}

#endif
