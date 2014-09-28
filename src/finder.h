#ifndef KD_FINDER_H
#define KD_FINDER_H

#include <limits>
#include <queue>
#include <vector>

template <typename CORE> class TFinder
{
public:
	typedef typename CORE::DataType  DataType;
	typedef typename CORE::PointType PointType;
	typedef typename CORE::UnitType  UnitType;
	
protected:
	class Stat
	{
	protected:
		DataType* mItem;
		UnitType  mStat;

	public:
		Stat(DataType* d, UnitType u): mItem(d), mStat(u) {}

		const DataType* item() const {return mItem;}
		const UnitType& stat() const {return mStat;}

		bool operator < (const Stat& other) const {return mStat < other.mStat;}
		bool operator > (const Stat& other) const {return mStat > other.mStat;}
	};
	
	typedef std::priority_queue<Stat> Queue;
	
protected:
	Queue            mQueue;
	const PointType& mPoint;
	UnitType         mRange;
	int              mMaxCount;
	UnitType         mMaxRange;
	
public:
	TFinder(const PointType& point, int count, UnitType range): mPoint(point)
	{
		mRange    = range * range;
		mMaxCount = count;
		mMaxRange = mRange;
	}
	
	void clear()
	{
		mQueue.clear();
		mRange = mMaxRange;
	}
	
	UnitType coordinate(int axis) const
	{
		return CORE::coordinate(mPoint, axis);
	}
	
	void push(DataType* d)
	{
		UnitType u = 0;
		for(int i = 0; i < CORE::DIMENSIONS; ++i)
		{
			UnitType c1 = CORE::coordinate(mPoint, i);
			UnitType c2 = CORE::coordinate(CORE::point(*d), i);
			UnitType di = c1 - c2;
			u += di * di;
		}
		
		if(u < this->rangeSquared())
		{
			mQueue.push(Stat(d, u));
			
			if(mQueue.size() > mMaxCount)
			{
				mQueue.pop();
				mRange = mQueue.top().stat();
			}
		}
	}
	
	UnitType rangeSquared() const
	{
		return mRange;
	}
	
	std::vector<DataType> vector() const
	{
		Queue temp(mQueue);
		std::vector<DataType> results(mQueue.size());
		
		for(int i = mQueue.size() - 1; i >= 0; --i)
		{
			results[i] = *const_cast<DataType*>(temp.top().item());
			temp.pop();
		}
		
		return results;
	}
	
public:
	static TFinder byCount(const PointType& point, int count)
	{
		return TFinder(point, count, std::numeric_limits<UnitType>::max());
	}
	
	static TFinder byRange(const PointType& point, UnitType range)
	{
		return TFinder(point, std::numeric_limits<int>::max(), range);
	}
};

#endif

