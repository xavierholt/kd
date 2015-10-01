#include "../Chaff.h"

#include <cstdlib>
#include <iostream>
#include <vector>

int main() {
  std::vector<int> things(1000000);
  for(int& i: things) {
    i = rand();
  }

  auto finder = Chaff::MaxFinder<int,int>::byCount(3);
  for(int i: things) {
    finder.sow(i, i);
  }

  std::cout << "The best three things are:\n";
  for(int i: finder.reap()) {
    std::cout << " - " << i << '\n';
  }

  std::cout << "Aren't they amazing?\n\n";
  return 0;
}
