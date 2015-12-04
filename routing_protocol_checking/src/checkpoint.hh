#ifndef __HH_CHECKPOINT_
#define __HH_CHECKPOINT_

#include "path.h"
#include "TraceInfo.h"
#include "ProgramState.hh"
#include "ProgramStates.hh"

#include <pthread.h>
#include <typeinfo>
#include <cassert>
#include <memory>

void checkpoint(const unsigned &label,
		const Path<int> &p, 
		Graph *graph,
		const unsigned &n,
		const unsigned &v,
		int &res, 
		TraceInfo &this_traceInfo,
		std::size_t &i,
		std::size_t &row,
		std::size_t &col,
		std::vector<std::vector<Path<int> > > &path,
		Path<int> &tmp) {
  switch (this_traceInfo.getJob()) {
  case RECORD: {
    switch (label) {
    case 4: {
      std::shared_ptr<ProgramState1> ps(new ProgramState1(p, res, i, row, col, path, tmp));
      // ProgramState1 *ps = new ProgramState1(p, res, i, row, col, path, tmp);
      this_traceInfo.setPSPtr(ps);
      this_traceInfo.setLabel(label);
      if ((0 <= graph->matrix[p.head()][p.rear()]
	   && graph->matrix[p.head()][p.rear()] < inf)
	  || p.is_checked())
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
      if ((0 <= graph->matrix[p.head()][p.rear()]
	   && graph->matrix[p.head()][p.rear()] < inf) 
	  || p.is_checked())
	return;
      p.check();
      // ProgramState1 *ps = new ProgramState1(p, res, i, row, col, path, tmp);
      std::shared_ptr<ProgramState1> ps(new ProgramState1(p, res, i, row, col, path, tmp));
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
    } // end_of_switch
  }
    break;

  case RESUME: {
    this_traceInfo.setJob(RECORD);
    // ProgramState1 *ps = NULL;
    std::shared_ptr<ProgramState1> ps;
    try {
      ps = dynamic_pointer_cast<ProgramState1>(this_traceInfo.getPSPtr());
    } catch (const std::bad_cast &e) {
      std::cout << e.what() << std::endl << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << endl;
    }
    // Recover each variable
    if (this_traceInfo.getFirst() != v)
      this_traceInfo.setFirst(v);
    if (this_traceInfo.getLast() != n)
      this_traceInfo.setLast(n);
    assert(NULL != ps && "NULL Pointer!\n");
    res = ps->res;
    i = ps->i;
    row = ps->row;
    col = ps->col;
    {
      for (std::size_t row = 0; row != ps->path.size(); ++row) {
	// s[row] = ps->s[row];
	for (std::size_t col = 0; col != ps->path[row].size(); ++col)
	  path[row][col] = ps->path[row][col];
      }
    }
    // std::memcpy(path, ps->path, sizeof(path[0][0]) * maxnum * maxnum);
    tmp = ps->tmp;
    // std::memcpy(s, ps->s, sizeof(s[0]) * maxnum);

    // // Release useless memory allocated for inner-execution
    // if (!(ps->p.is_checked())) {
    //   // Namely, ps in this traceInfo will not be used in the future
    //   this_traceInfo.collectGarbage();
    // }
    ps = NULL;
  }
    break;

  default:
    break;
  } // end_of_switch
}

void checkpoint(const unsigned &label,
		const Path<int> &p, 
		Graph *graph,
		const unsigned &n,
		const unsigned &v,
		int &res, 
		TraceInfo &this_traceInfo,
		unsigned &i,
		unsigned &j,
		std::vector<std::vector<Path<int> > > &path,
		std::bitset<maxnum> &s,
		unsigned &u,
		unsigned &x,
		unsigned &y,
		const Path<int> &tmp) {
  switch (this_traceInfo.getJob()) {
  case RECORD: {
    switch (label) {
    case 5:
    case 7: {
      std::shared_ptr<ProgramState2> ps(new ProgramState2(p, res, i, j, path, s, u, x, y, tmp));
      this_traceInfo.setPSPtr(ps);
      this_traceInfo.setLabel(label);
      if ((0 <= graph->matrix[p.head()][p.rear()]
	   && graph->matrix[p.head()][p.rear()] < inf) 
	  || p.is_checked())
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
      if ((0 <= graph->matrix[p.head()][p.rear()]
	   && graph->matrix[p.head()][p.rear()] < inf) 
	  || p.is_checked())
	return;
      p.check();
      std::shared_ptr<ProgramState2> ps(new ProgramState2(p, res, i, j, path, s, u, x, y, tmp));
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
    } // end_of_switch
  }
    break;

  case RESUME: {
    this_traceInfo.setJob(RECORD);
    std::shared_ptr<ProgramState2> ps;// = NULL;
    try {
      ps = dynamic_pointer_cast<ProgramState2>(this_traceInfo.getPSPtr());
    } catch (const std::bad_cast &e) {
      std::cout << e.what() << std::endl << __FILE__ << ": " << __FUNCTION__ << ": " << __LINE__ << endl;
    }

    // Recover each variable
    if (this_traceInfo.getFirst() != v)
      this_traceInfo.setFirst(v);
    if (this_traceInfo.getLast() != n)
      this_traceInfo.setLast(n);
    assert(NULL != ps && "NULL Pointer!\n");
    res = ps->res;
    i = ps->i;
    j = ps->j;
    // path = ps->path;
    s = ps->s;
    for (std::size_t row = 0; row != ps->path.size(); ++row) {
      // s[row] = ps->s[row];
      for (std::size_t col = 0; col != ps->path[row].size(); ++col)
    	path[row][col] = ps->path[row][col];
    }
    // std::memcpy(path, ps->path, sizeof(path[0][0]) * maxnum * maxnum);
    // std::memcpy(s, ps->s, sizeof(s[0]) * maxnum);
    u = ps->u;
    x = ps->x;
    y = ps->y;

    // // Release useless memory allocated for inner-execution
    // if (!(ps->p.is_checked())) {
    //   // Namely, ps in this traceInfo will not be used in the future
    //   this_traceInfo.collectGarbage();
    // }
    ps = NULL;
  }
    break;

  default:
    break;
  } // end_of_switch
}

#endif	// __HH_CHECKPOINT_
