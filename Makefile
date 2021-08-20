CXX = g++
CXXFLAGS = -std=c++0x -Wall -g -O3 -march=native -mtune=native
FILES = src/main.cpp src/Construtivo.cpp src/MinCut.cpp src/parameters.cpp src/ProblemPath.cpp src/ProblemInstance.cpp src/SampleDecoder.cpp src/Solution.cpp src/BFS.cpp src/Scanner.cpp src/tclp.cpp

build:
	$(CXX) $(CXXFLAGS) -Ofast -o tclp-brkga $(FILES) -lemon -fopenmp

build-nomp:
	$(CXX) $(CXXFLAGS) -Ofast -o tclp-brkga-nomp $(FILES) -lemon

build-cluster:
	$(CXX) $(CXXFLAGS) -Ofast -o tclp-brkga $(FILES) -lemon -fopenmp -I/app/devel/include -L/app/devel/lib

build-cluster-nomp:
	$(CXX) $(CXXFLAGS) -Ofast -o tclp-brkga-nomp $(FILES) -lemon -I/app/devel/include -L/app/devel/lib

build-profiling:
	$(CXX) -pg $(CXXFLAGS) -Ofast -o tclp-brkga-profiling $(FILES) -lemon -fopenmp

build-profiling-nomp:
	$(CXX) -pg $(CXXFLAGS) -Ofast -o tclp-brkga-profiling-nomp $(FILES) -lemon

clean:
	rm -rf *.o tclp-brkga tclp-brkga-nomp

clean-profiling:
	rm -rf tclp-brkga-profiling gmon.out profiling-result.txt