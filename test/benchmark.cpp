#include "../Chaff.h"

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <functional>

typedef long long int NS;
const NS NANO = 1000000000LL;

NS nanoseconds() {
  timespec time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
  return NANO * time.tv_sec + time.tv_nsec;
}

NS bench(const std::function<void()>& callback) {
  NS start = nanoseconds();
  callback();
  return nanoseconds() - start;
}

void print(char id, const std::vector<int>& data) {
  printf("%c: ", id);
  for(int i: data) printf("%i ", i);
  printf("\n");
}

int main(int argc, char* argv[]) {
  if(argc < 4 || argc > 5) {
    printf("USAGE: %s data-count find-count test-count [seed]\n", argv[0]);
    exit(1);
  }

  const int DATA = std::stoi(argv[1]);
  const int FIND = std::stoi(argv[2]);
  const int REPS = std::stoi(argv[3]);
  const int SEED = (argc == 5)? std::stoi(argv[4]): time(nullptr);
  printf("%s %i %i %i %i\n", argv[0], DATA, FIND, REPS, SEED);

  srand(SEED);
  int* data = new int[DATA];
  for(int i = 0; i < DATA ; ++i) {
    data[i] = i;
  }

  NS  cmin_time  = 0;
  NS  cmax_time  = 0;
  NS  sort_time  = 0;

  int cmin_found = 0;
  int cmax_found = 0;

  for(int i = 0; i < REPS; ++i) {
    std::random_shuffle(data, data + DATA);

    std::vector<int> cmin_data;
    std::vector<int> cmax_data;
    std::vector<int> smin_data;
    std::vector<int> smax_data;
    
    cmin_time += bench([&]() {
      auto q = Chaff::MinFinder<int, int>::byCount(FIND);
      for(int i = 0; i < DATA; ++i) q.sow(data[i], data[i]);
      cmin_data = q.reap();
    });

    cmax_time += bench([&]() {
      auto q = Chaff::MaxFinder<int, int>::byCount(FIND);
      for(int i = 0; i < DATA; ++i) q.sow(data[i], data[i]);
      cmax_data = q.reap();
    });

    sort_time += bench([&]() {
      int n = std::min(DATA, FIND);
      std::sort(data, data + DATA);
      smin_data = std::vector<int>(data, data + n);
      std::reverse(data + DATA - n, data + DATA);
      smax_data = std::vector<int>(data + DATA - n, data + DATA);
    });

    cmin_found += (cmin_data == smin_data);
    cmax_found += (cmax_data == smax_data);
  }

  printf("MinFinder %10.6fs (%i / %i)\n", double(cmin_time) / NANO, cmin_found, REPS);
  printf("MaxFinder %10.6fs (%i / %i)\n", double(cmax_time) / NANO, cmax_found, REPS);
  printf("Full Sort %10.6fs (%i / %i)\n", double(sort_time) / NANO, REPS,       REPS);

  if(cmin_found != REPS || cmax_found != REPS) {
    printf("Result mismatch!\n\n");
    exit(2);
  }

  printf("\n");
  return 0;
}
