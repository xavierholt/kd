#ifndef FINDER_H
#define FINDER_H

#include <limits>
#include <vector>

#include "heap.h"

template <class Item, class Score> class Finder
{
protected:
	struct Stat
	{
		Item  item;
		Score score;
		
		Stat() {}
		Stat(const Item& item, Score score): item(item), score(score) {}

		bool operator < (const Stat& other) const
		{
			return score < other.score;
		}
		
		bool operator > (const Stat& other) const
		{
			return score > other.score;
		}
	};
	
	typedef Heap<Stat, std::greater<Stat> > SHeap;

protected:
	SHeap mHeap;
	Score mScore;
	Score mMaxScore;
	int   mMaxCount;

public:
	Finder(int count, Score score): mHeap(std::min(100, count + 1))
	{
		mScore    = score;
		mMaxScore = score;
		mMaxCount = count;
	}

	void check(const Item& item, Score score)
	{
		if(score < mScore)
		{
			mHeap.push(Stat(item, score));
			if(mHeap.count() > mMaxCount)
			{
				mHeap.pop();
				mScore = mHeap.top().score;
			}
		}
	}

	void clear()
	{
		mHeap.clear();
		mScore = mMaxScore;
	}
	
	Score score() const
	{
		return mScore;
	}
	
	Item top() const
	{
		return mHeap.top().item;
	}

	std::vector<Item> vector()
	{
		std::vector<Item> results(mHeap.count());
	
		for(int i = mHeap.count() - 1; i >= 0; --i)
		{
			results[i] = mHeap.top().item;
			mHeap.pop();
		}
	
		clear();
		return results;
	}

public:
	static Finder byCount(int count)
	{
		return Finder(count, std::numeric_limits<Score>::max());
	}

	static Finder byScore(Score score)
	{
		return Finder(std::numeric_limits<int>::max(), score);
	}
};

#endif
