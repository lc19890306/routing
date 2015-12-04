CXX = clang++-3.3
CFLAGS = -Wall -O0 -g -ggdb -std=c++11
# CFLAGS = -O3 -std=c++11
INCLUDE = -I .
LIBS = 
TARGET = ./preprocessor
SOURCES = string.cc program.cc preprocprogram.cc chkptprogram.cc preprocessor.cc main.cc

make:
	$(CXX) $(CFLAGS) $(SOURCES) -o $(TARGET) $(INCLUDE) $(LIBS)
clean:
	rm -rf $(TARGET)
