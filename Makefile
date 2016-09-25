FLAGS = -std=c++11 -O3 -Wall -Werror
LIBS  = -lrt

all: example.out
example.out: test/example.cpp
	${CXX} $(FLAGS) -o $@ $^ $(LIBS)
chaff-init:
	git subtree add  --prefix src/chaff https://github.com/xavierholt/chaff.git master --squash 
chaff-pull:
	git subtree pull --prefix src/chaff https://github.com/xavierholt/chaff.git master --squash 
clean:
	rm -f example.out
