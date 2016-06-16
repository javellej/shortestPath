CXX=g++
CPPFLAGS=-std=c++0x -O2
SRC=src/main.cpp src/Graph.cpp src/Tools.cpp
INC=-Iinc/
SRC_TST=src/Graph.cpp src/Tools.cpp test/test.cpp
INC_TST=$(INC) -Itest/gtest/include
LIB_TST=-lgtest

all: main tests

main: $(SRC)
	$(CXX) $(CPPFLAGS) -o shortestPath $(SRC) $(INC)

tests: $(SRC_TST)
	$(CXX) $(CPPFLAGS) -o testShortestPath $(SRC_TST) $(INC_TST) $(LIB_TST)

clean:
	rm -f shortestPath testShortestPath
