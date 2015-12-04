#ifndef __H_WEIGHT_
#define __H_WEIGHT_

#include "headers.h"
#include "path.h"
#include "Graph.h"

unsigned weight(const Path<int> &path);
unsigned weight(std::vector<int> path[maxnum][maxnum], unsigned s, unsigned t1, unsigned t2 = 0);

/*   if (prev[t] == 0)		/\* if node t has no previous node *\/ */
/*     w = c[s][t];		/\* return weight from s to t *\/ */
/*   else { */
/*     if (u == 0) {		/\* if no intermediate node u *\/ */
/*       int pre = prev[t];	/\* calculate path weight reversely from *\/ */
/*       do {			/\* t to s *\/ */
/* 	w += c[pre][t]; */
/* 	pre = prev[pre]; */
/*       } while (pre != s); */
/*     } */
/*     else {			/\* if intermediate node u exists *\/ */
/*       if (prev[t] == 0)		/\* if node t is node u's neighbor *\/ */
/* 	w += c[u][t];		/\* directly add weight from u to t *\/ */
/*       else {			/\* if not *\/ */
/* 	int pre = prev[t];	/\* calculate path weight reversely from *\/ */
/* 	do {			/\* t to u first *\/ */
/* 	  w += c[pre][t]; */
/* 	  pre = prev[pre]; */
/* 	} while (pre != u); */
/*       } */
/*       int pre = prev[u];	/\* calculate path weight reversely from *\/ */
/*       do {			/\* node u to node s *\/ */
/* 	w += c[pre][u]; */
/* 	pre = prev[pre]; */
/*       } while (pre != s); */
/*     } */
/*   } */
/*      std::cout << ", prev[" << t << "]: " << prev[t] << ", w: " << w << std::endl;  */
/*     return w; */

/* } */

#endif	/* __H_WEIGHT_ */
