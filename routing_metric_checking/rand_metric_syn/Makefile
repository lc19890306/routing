CC = clang
CXX = clang++
CFLAGS = -Wall -O0 -g -ggdb -std=c++11 -Wunused-result -fpermissive -Wsign-compare -D_REENTRANT
# CFLAGS = -Wall -O0 -g -ggdb -std=c++11 -fsanitize=thread -fPIE
# CFLAGS = -O3 -std=c++11 -fpermissive
INCLUDE = -I .
TARGET = ./rand_metric_syn
SOURCES = string.cc Options.cc TernaryTree.cc RandMetric.cc TestProgram.cc main.cc
make:
	$(CXX) $(CFLAGS) $(SOURCES) -o $(TARGET) $(INCLUDE) $(LIBS)
clean:
	rm -rf $(TARGET)
