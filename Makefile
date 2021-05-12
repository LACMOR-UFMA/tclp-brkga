CXX = g++
CXXFLAGS = -std=c++0x -Wall -g -O3 -march=native -mtune=native
FILES = src/main.cpp src/Construtivo.cpp src/FWChrono.cpp src/MinCut.cpp src/Path.cpp src/ProblemInstance.cpp src/SampleDecoder.cpp src/Solution.cpp src/BFS.cpp src/Scanner.cpp

tclp-brkga:
	$(CXX) $(CXXFLAGS) -o tclp-brkga $(FILES) -lemon -fopenmp

fast-tclp-brkga:
	$(CXX) $(CXXFLAGS) -Ofast -o fast-tclp-brkga $(FILES) -lemon -fopenmp

cluster-run:
	$(CXX) $(CXXFLAGS) -Ofast -o fast-tclp-brkga $(FILES) -lemon -fopenmp -I/app/devel/include -L/app/devel/lib

clean:
	rm -rf *.o tclp-brkga fast-tclp-brkga
