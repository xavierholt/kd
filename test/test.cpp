#include "../src/tree.h"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <time.h>

inline long long int nanosec()
{
	timespec time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
	return ((long long int) 1000000000) * time.tv_sec + time.tv_nsec;
}

union Point
{
	float data[3];
	struct
	{
		float x;
		float y;
		float z;
	};
};

struct Thing
{
public:
	static int next;
	
public:
	int id;
	Point point;
	
	Thing()
	{
		id = ++next;
		point.x = ((float) rand()) / RAND_MAX;
		point.y = ((float) rand()) / RAND_MAX;
		point.z = ((float) rand()) / RAND_MAX;
	}
	
	bool operator < (const Thing& other) const
	{
		return id < other.id;
	}
};

std::ostream& operator << (std::ostream& stream, const Point& point)
{
	return stream << '(' << point.x << ", " << point.y << ", " << point.z << ')';
}

std::ostream& operator << (std::ostream& stream, const Thing& thing)
{
	return stream << "Thing " << thing.id << ' ' << thing.point;
}

int Thing::next = 0;

struct CORE
{
	typedef Thing* DataType;
	typedef Point  PointType;
	typedef float  UnitType;
	
	const static int DIMENSIONS;
	const static int STORAGE;
	
	static UnitType coordinate(const PointType& p, int axis)
	{
		return p.data[axis];
	}
	
	static const PointType& point(const DataType& d)
	{
		return d->point;
	}
};

std::ostream& operator << (std::ostream& stream, const std::vector<Thing*>& vec)
{
	stream << "Vector: ";
	std::vector<Thing*>::const_iterator i = vec.begin();
	while(i != vec.end())
	{
		stream << " " << *i;
		++i;
	}
	return stream;
}
const int CORE::DIMENSIONS = 3;
const int CORE::STORAGE    = 8;

int main()
{
	int seed = time(0);
	std::cout << "Seed: " << seed << '\n';
	srand(seed);
	
	const int N = 100000;
	Thing  things[N];
	Thing* tptrs[N];
	
	Point min;
	min.x = 0;
	min.y = 0;
	min.z = 0;
	
	Point max;
	max.x = 1;
	max.y = 1;
	max.z = 1;
	
	KDTree<CORE> kdtree(min, max);
	for(int i = 0; i < N; ++i)
	{
		kdtree.insert(&things[i]);
		tptrs[i] = &things[i];
	}
	
	int trials  = 0;
	int matches = 0;
	
	long long int kdt = 0;
	long long int rat = 0;
	
	while(trials < 10000)
	{
		TFinder<CORE> kdf(things[trials].point, 20, 0.15);
		TFinder<CORE> raf(things[trials].point, 20, 0.15);
		
		long long int ta = nanosec();
		kdtree.find(kdf);
		long long int tb = nanosec();
		for(int i = N-1; i >= 0; --i)
		{
			raf.push(&tptrs[i]);
		}
		long long int tc = nanosec();
		
		kdt += (tb - ta);
		rat += (tc - tb);
		
		std::vector<Thing*> kd_results(kdf.vector());
		std::vector<Thing*> ra_results(raf.vector());
		if(kd_results == ra_results)
		{
			++matches;
		}
		++trials;
	}
	
	std::cout << matches << " of " << trials << " succeeded.\n";
	std::cout << "KD-Tree: ";
	std::cout.width(12);
	std::cout << kdt << " ns\n";
	std::cout << "Array:   ";
	std::cout.width(12);
	std::cout << rat << " ns\n";
	
	for(int i = 0; i < N; ++i)
	{
		kdtree.remove(&things[i]);
	}
	
	return 0;
}

