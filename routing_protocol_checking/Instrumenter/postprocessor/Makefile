# CXX = clang++-3.3
CXX = clang++
CFLAGS = -Wall -O0 -g -ggdb -std=c++11
CFLAGS = -O3 -std=c++11
INCLUDE = -I .
LIBS = 
TARGET = ./postprocessor
SOURCES = string.cc program.cc chkptprogram.cc psprogram.cc postprocprogram.cc postprocessor.cc main.cc

make:
	$(CXX) $(CFLAGS) $(SOURCES) -o $(TARGET) $(INCLUDE) $(LIBS)
clean:
	rm -rf $(TARGET)
