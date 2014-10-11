#ifndef KD_FINDER_H
#define KD_FINDER_H

#include <limits>
#include <stdexcept>
#include <vector>

namespace KD
{
	template <class CORE> class Finder
	{
	public:
		typedef typename CORE::DataType  DataType;
		typedef typename CORE::PointType PointType;
		typedef typename CORE::CoordType CoordType;
	
	protected:
		struct Stat
		{
			const DataType* item;
			CoordType       stat;
			
			Stat() {}
			Stat(const DataType* data, CoordType stat): item(data), stat(stat) {}

			bool operator < (const Stat& other) const {return stat < other.stat;}
			bool operator > (const Stat& other) const {return stat > other.stat;}
		};
	
	protected:
		int       mCount;
		int       mCapacity;
		int       mMaxCount;
		Stat*     mQueue;
		CoordType mRange;
		CoordType mMaxRange;
		CoordType mPoint[CORE::DIMENSIONS];
	
	protected:
		void enlarge()
		{
			mCapacity = std::min(mCapacity + 100, mMaxCount + 1);
			Stat* temp = new Stat[mCapacity];
			for(int i = 0; i < mCount; ++i)
			{
				temp[i] = mQueue[i];
			}
			
			delete [] mQueue;
			mQueue = temp;
		}
		
		void pop()
		{
			int p;
			int c = 0;
			mCount -= 1;
		
			while(true)
			{
				p = c;
				c = p * 2 + 1;
				if(c + 1 < mCount)
				{
					c += (mQueue[c] < mQueue[c+1]);
					if(mQueue[c] > mQueue[mCount])
					{
						mQueue[p] = mQueue[c];
						continue;
					}
				}
				else if(c < mCount)
				{
					if(mQueue[c] > mQueue[mCount])
					{
						mQueue[p] = mQueue[c];
						continue;
					}
				}
				break;
			}
		
			mQueue[p] = mQueue[mCount];
		}
	
		void push(const Stat& stat)
		{
			if(mCount >= mCapacity)
			{
				enlarge();
			}
			
			int c = mCount;
			while(c > 0)
			{
				int p = (c - 1) / 2;
				if(mQueue[p] > stat) break;
				mQueue[c] = mQueue[p];
				c = p;
			}
		
			mQueue[c] = stat;
			mCount += 1;
		}
	
	public:
		Finder(const PointType& point, int count, CoordType range)
		{
			mCount    = 0;
			mMaxCount = count;
			mCapacity = std::min(99, mMaxCount) + 1;
			mQueue    = new Stat[mCapacity];
			mRange    = range * range;
			mMaxRange = mRange;
		
			for(int i = 0; i < CORE::DIMENSIONS; ++i)
			{
				mPoint[i] = CORE::coordinate(point, i);
			}
		}
	
		~Finder()
		{
			delete [] mQueue;
		}
	
		DataType best() const
		{
			if(mCount < 1)
			{
				throw new std::underflow_error("No data available.");
			}
		
			int leaves = (mCount + 1) / 2;
			DataType* best_item = 0;
			CoordType best_stat = std::numeric_limits<CoordType>::max();
			for(int i = mCount - leaves; i < mCount; ++i)
			{
				if(mQueue[i].stat < best_stat)
				{
					best_stat = mQueue[i].stat;
					best_item = mQueue[i].item;
				}
			}
		
			return *best_item;
		}
	
		void check(const DataType* data)
		{
			CoordType dist = 0;
			for(int i = 0; i < CORE::DIMENSIONS; ++i)
			{
				CoordType d = mPoint[i] - CORE::coordinate(CORE::point(*data), i);
				dist += d * d;
			}
		
			if(dist < mRange)
			{
				push(Stat(data, dist));
				if(mCount > mMaxCount)
				{
					pop();
					mRange = mQueue[0].stat;
				}
			}
		}
	
		void clear()
		{
			mCount = 0;
			mRange = mMaxRange;
		}
	
		CoordType coordinate(int axis) const
		{
			return mPoint[axis];
		}
	
		CoordType rangeSquared() const
		{
			return mRange;
		}
		
		DataType top() const
		{
			if(mCount < 1)
			{
				throw new std::underflow_error("No data available.");
			}
		
			return *mQueue[0].item;
		}
	
		std::vector<DataType> vector()
		{
			std::vector<DataType> results(mCount);
		
			for(int i = mCount - 1; i >= 0; --i)
			{
				results[i] = *mQueue[0].item;
				pop();
			}
		
			clear();
			return results;
		}
	
	public:
		static Finder byCount(const PointType& point, int count)
		{
			return Finder(point, count, std::numeric_limits<CoordType>::max());
		}
	
		static Finder byRange(const PointType& point, CoordType range)
		{
			return Finder(point, std::numeric_limits<int>::max(), range);
		}
	};
}

#endif

