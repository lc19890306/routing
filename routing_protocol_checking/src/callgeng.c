/* This is a sample of how to call geng as a procedure rather than
 * running it as a separate process.  The basic idea is to construct
 * an argument list for geng's main() function.  At compile time,
 * assign a name to the macros OUTPROC and GENG_MAIN.  A typical
 * Unix-style compilation command would be:
     gcc -o callgeng -O3 -DMAXN=32 -DOUTPROC=myoutproc -DGENG_MAIN=geng_main \
       callgeng.c geng.c gtools.o nauty1.o nautil1.o naugraph1.o schreier.o naurng.o
 */

/* #ifndef GENG_MAIN */
/* #define GENG_MAIN */

#include "gtools.h"

static unsigned long counter;

void
OUTPROC(FILE *outfile, graph *g, int n)
{
 /* This will be called for each graph. */

    ++counter;
}


int
main(int argc, char *argv[])
{
    int geng_argc;
    char *geng_argv[6];

  /* Set up geng argument list.  The 0-th argument is the command name.
   * There must be a NULL at the end.  This example is for connected
   * bipartite graphs of order 10 and maximum degree at most 4. */

    geng_argv[0] = "geng";
    geng_argv[1] = "-lv";
    geng_argv[2] = "-c";
    geng_argv[3] = "-D4";
    geng_argv[4] = "4";
    geng_argv[5] = NULL;
    geng_argc = 5;

    counter = 0;
    GENG_MAIN(geng_argc,geng_argv);
    /* geng_main(geng_argc,geng_argv); */

    printf("Number of graphs = %lu.\n",counter);

    return 0;
}

/* #endif	/\* GENG_MAIN *\/ */