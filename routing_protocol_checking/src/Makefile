CC = mpicc

# CXX = mpic++
#CXX = clang++
CXX = g++-4.9

DBGCFLAGS = -Wall -O0 -g -ggdb -std=c++11 -Wunused-result -fpermissive -Wsign-compare -D_REENTRANT
# DBGCFLAGS += -DLEFT_MONOTONICITY
DBGCFLAGS += -DRIGHT_MONOTONICITY
# DBGCFLAGS += -DLEFT_ISOTONICITY
DBGCFLAGS += -DRIGHT_ISOTONICITY
DBGCFLAGS += -DODD_SYMMETRY
DBGCFLAGS += -DTRANSITIVITY
DBGCFLAGS += -DSTRICT_ORDER
DBGCFLAGS += -DSOURCE_INDEPENDENCE
DBGCFLAGS += -DLOCAL_MINIMUM_FREE
DBGCFLAGS += -DCGF

# CFLAGS = -Wall -O0 -g -ggdb -std=c++11 -fsanitize=thread -fPIE
CFLAGS = -std=c++11 -fpermissive -DMAXN=32 -DOUTPROC=myoutproc -DGENG_MAIN=geng_main
# CFLAGS += -DLEFT_MONOTONICITY
CFLAGS += -DRIGHT_MONOTONICITY
# CFLAGS += -DLEFT_ISOTONICITY
CFLAGS += -DRIGHT_ISOTONICITY
CFLAGS += -DODD_SYMMETRY
CFLAGS += -DTRANSITIVITY
CFLAGS += -DSTRICT_ORDER
CFLAGS += -DSOURCE_INDEPENDENCE
CFLAGS += -DLOCAL_MINIMUM_FREE
CFLAGS += -DCGF

INCLUDE = -I .

LIBS = -pthread #-lck -ltbbmalloc -ltbb
# LIBS = -pthread -pie

# TARGET = ./bellman_ford
TARGET = ./dijkstra

# SOURCES = compare.cpp propertycheck.cc bellman_ford.cc AlgoJob.cc CheckConsistencyJob.cc functions.cpp strop.cpp TThread.cc TThreadPool.cc dft.cc ucg_generator.cc Graph.cc check.cc main.cc
SOURCES = compare.cpp propertycheck.cc dijkstra.cpp AlgoJob.cc CheckConsistencyJob.cc functions.cpp strop.cpp TThread.cc TThreadPool.cc dft.cc ucg_generator.cc Graph.cc check.cc main.cc geng.c nauty.a

all:
	$(CXX) $(CFLAGS) $(SOURCES) -o $(TARGET) $(INCLUDE) $(LIBS)

debug:
	$(CXX) $(DBGCFLAGS) $(SOURCES) -o $(TARGET) $(INCLUDE) $(LIBS)

clean:
	rm -rf $(TARGET)
