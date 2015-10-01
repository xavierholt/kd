FLAGS = -std=c++11 -O3 -Wall -Werror
LIBS  = -lrt

all: example.out benchmark.out
example.out: test/example.cpp Chaff.h
	${CXX} $(FLAGS) -o $@ $< $(LIBS)
benchmark.out: test/benchmark.cpp Chaff.h
	${CXX} $(FLAGS) -o $@ $< $(LIBS)
test: all
	./example.out
	./benchmark.out 1000000      1 10
	./benchmark.out 1000000     10 10
	./benchmark.out 1000000    100 10
	./benchmark.out 1000000   1000 10
	./benchmark.out 1000000  10000 10
	./benchmark.out 1000000 100000 10
clean:
	rm -f example.out benchmark.out
