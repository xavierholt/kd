#include "array-tree.h"
#include "both-tree.h"
#include "union-tree.h"
#include "virtual-tree.h"

#include <cstdlib>
#include <cstdio>
#include <time.h>

inline long long int nanosec() {
  timespec time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
  return 1000000000LL * time.tv_sec + time.tv_nsec;
}

template <int DIM, class T>
struct TPoint
{
public:
  typedef T Coord;
  const static int N = DIM;

  static TPoint random() {
    TPoint result;
    for(int i = 0; i < N; ++i) {
      result.data[i] = (T) rand() / RAND_MAX;
    }

    return result;
  }

public:
  T data[DIM];

public:
  TPoint() {}

  TPoint(const TPoint& other) {
    for(int i = 0; i < N; ++i) {
      data[i] = other[i];
    }
  }

  TPoint(T value) {
    for(int i = 0; i < N; ++i) {
      data[i] = value;
    }
  }

  T operator [] (int index) const {
    return data[index];
  }
};

typedef TPoint<3, float> Point;

struct Thing
{
public:
  int id;
  Point mPoint;

  Thing(): mPoint(Point::random()) {
    static int next = 0;
    id = ++next;
  }

  const Point& point() const {
    return mPoint;
  }

  bool operator == (const Thing& other) const {
    return id == other.id;
  }

  bool operator < (const Thing& other) const {
    return id < other.id;
  }
};

typedef Finder<Thing*, Point::Coord> ThingFinder;

void printline(const char* label, int matches, int queries, long long int insert, long long int search, long long int remove) {
  double ims = double(insert) / 1000000;
  double sms = double(search) / 1000000;
  double rms = double(remove) / 1000000;
  printf(" %-10s %7i / %-7i %14.6f %14.6f %14.6f\n", label, matches, queries, ims, sms, rms);
}

typedef KD::Core<Point::N, Thing*, Point, Point::Coord, 32, 64> CORE;

int main(int argc, char* argv[]) {
  int COUNT   = 100000;
  int QUERIES = 10000;
  int SEED    = time(0);

  if(argc == 2) {
    SEED = atoi(argv[1]);
  }

  printf("Seed:       %i\n", SEED);
  printf("Objects:    %i\n", COUNT);
  printf("Queries:    %i\n", QUERIES);
  printf("Dimensions: %i\n", CORE::DIMENSIONS);
  printf("Storage:    %i\n", CORE::STORAGE);
  printf("Max Depth:  %i\n", CORE::MAX_DEPTH);
  printf("\n");
  srand(SEED);

  Thing*  things = new Thing[COUNT];

  Point min(0);
  Point max(1);
  KD::Array::Tree<CORE>   atree(min, max);
  KD::Both::Tree<CORE>    btree(min, max);
  KD::Union::Tree<CORE>   utree(min, max);
  KD::Virtual::Tree<CORE> vtree(min, max);

  long long int ta = nanosec();
  for(int i = 0; i < COUNT; ++i)
    atree.insert(&things[i]);
  long long int tb = nanosec();
  for(int i = 0; i < COUNT; ++i)
    btree.insert(&things[i]);
  long long int tc = nanosec();
  for(int i = 0; i < COUNT; ++i)
    utree.insert(&things[i]);
  long long int td = nanosec();
  for(int i = 0; i < COUNT; ++i)
    vtree.insert(&things[i]);
  long long int te = nanosec();

  long long int ainit = tb - ta;
  long long int binit = tc - tb;
  long long int uinit = td - tc;
  long long int vinit = te - td;

  int bmatch = 0;
  int umatch = 0;
  int vmatch = 0;

  long long int atime = 0;
  long long int btime = 0;
  long long int utime = 0;
  long long int vtime = 0;

  for(int i = 0; i < QUERIES; ++i) {
    Point testpoint(Point::random());
    ThingFinder afind(20, 0.15);
    ThingFinder bfind(20, 0.15);
    ThingFinder ufind(20, 0.15);
    ThingFinder vfind(20, 0.15);

    ta = nanosec();
    atree.search(testpoint, afind);
    tb = nanosec();
    btree.search(testpoint, bfind);
    tc = nanosec();
    utree.search(testpoint, ufind);
    td = nanosec();
    vtree.search(testpoint, vfind);
    te = nanosec();

    atime += (tb - ta);
    btime += (tc - tb);
    utime += (td - tc);
    vtime += (te - td);

    std::vector<Thing*> canon(afind.vector());
    bmatch += (canon == bfind.vector());
    umatch += (canon == ufind.vector());
    vmatch += (canon == vfind.vector());
  }

  ta = nanosec();
  for(int i = 0; i < COUNT; ++i)
    atree.remove(&things[i]);
  tb = nanosec();
  for(int i = 0; i < COUNT; ++i)
    btree.remove(&things[i]);
  tc = nanosec();
  for(int i = 0; i < COUNT; ++i)
    utree.remove(&things[i]);
  td = nanosec();
  for(int i = 0; i < COUNT; ++i)
    vtree.remove(&things[i]);
  te = nanosec();

  long long int adele = tb - ta;
  long long int bdele = tc - tb;
  long long int udele = td - tc;
  long long int vdele = te - td;

  printf("            Matches / Queries      Init (ms)    Search (ms)    Delete (ms)\n");
  printf("----------+-------------------+--------------+--------------+--------------\n");
  printline("Array",   QUERIES, QUERIES, ainit, atime, adele);
  printline("Both",    bmatch,  QUERIES, binit, btime, bdele);
  printline("Union",   umatch,  QUERIES, uinit, utime, udele);
  printline("Virtual", vmatch,  QUERIES, vinit, vtime, vdele);
  return 0;
}

