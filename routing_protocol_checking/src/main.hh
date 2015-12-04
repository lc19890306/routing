#ifndef __H_MAIN_
#define __H_MAIN_

#include <deque>
#include <array>

#include "gtools.h"
#include "geng.h"

#include "headers.h"
#include "dijkstra.h"
#include "TThreadPool.hh"
#include "check.hh"

void checkDerivedGraphs(pAlgoFunc algoFunc, const Graph &inputGraph, const unsigned &maxLen, const bool &val);
void OUTPROC(FILE *outfile, graph *g, int n)
{
 /* This will be called for each graph. */

    // ++counter;
}


#endif	/* __H_MAIN_ */
