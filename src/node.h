#ifndef KD_NODE_H
#define KD_NODE_H

#include "finder.h"

template <typename CORE> class KDTree;

template <typename CORE> class KDNode
{
	typedef typename CORE::DataType  DataType;
	typedef typename CORE::PointType PointType;
	typedef typename CORE::UnitType  UnitType;
	typedef          TFinder<CORE>   Finder;
	
	friend class KDTree<CORE>;
	
protected:
	int           mAxis;
	int           mCount;
	UnitType      mMinima[CORE::DIMENSIONS];
	UnitType      mMaxima[CORE::DIMENSIONS];
	const KDNode* mParent;
	
	union
	{
		DataType  mItems[CORE::STORAGE];
		KDNode*   mNodes[2];
	};
	
protected:
	KDNode(const KDNode* parent, const DataType& data)
	{
		int pax   = parent->mAxis;
		mAxis     = (pax + 1) % CORE::DIMENSIONS;
		mCount    = 1;
		mItems[0] = data;
		mParent   = parent;
		
		for(int i = 0; i < CORE::DIMENSIONS; ++i)
		{
			mMinima[i] = parent->mMinima[i];
			mMaxima[i] = parent->mMaxima[i];
		}
		
		if(CORE::coordinate(CORE::point(data), pax) > parent->midpoint())
		{
			mMinima[pax] = parent->midpoint();
		}
		else
		{
			mMaxima[pax] = parent->midpoint();
		}
	}
	
	KDNode(const PointType& min, const PointType& max)
	{
		mAxis     = 0;
		mCount    = -1;
		mNodes[0] = 0;
		mNodes[1] = 0;
		mParent   = 0;
		
		for(int i = 0; i < CORE::DIMENSIONS; ++i)
		{
			mMinima[i] = CORE::coordinate(min, i);
			mMaxima[i] = CORE::coordinate(max, i);
		}
	}
	
	~KDNode()
	{
		if(mCount < 0)
		{
			delete mNodes[0];
			delete mNodes[1];
		}
	}
	
	int child(const DataType& point)
	{
		return (CORE::coordinate(CORE::point(point), mAxis) > midpoint());
	}
	
	int child(const PointType& point) const
	{
		return (CORE::coordinate(point, mAxis) > midpoint());
	}
	
	void find(Finder& finder)
	{
		if(mCount >= 0)
		{
			for(int i = mCount-1; i >= 0; --i)
			{
				finder.push(&mItems[i]);
			}
		}
		else
		{
			UnitType diff = finder.coordinate(mAxis) - midpoint();
			int idx = (diff > 0);
			
			if(mNodes[idx])
			{
				mNodes[idx]->find(finder);
			}
			
			if(diff * diff < finder.rangeSquared())
			{
				if(mNodes[idx ^ 1])
				{
					mNodes[idx ^ 1]->find(finder);
				}
			}
		}
	}
	
	void insert(const DataType& data)
	{
		if(mCount >= CORE::STORAGE)
		{
			this->split();
		}
		
		if(mCount >= 0)
		{
			mItems[mCount] = data;
			mCount += 1;
		}
		else
		{
			KDNode*& n = mNodes[child(data)];
			if(n == 0) n = new KDNode(this, data);
			else n->insert(data);
		}
	}
	
	UnitType midpoint() const
	{
		UnitType min = mMinima[mAxis];
		UnitType max = mMaxima[mAxis];
		return (min + max) / 2;
	}
	
	KDNode* remove(const DataType& data)
	{
		if(mCount >= 0)
		{
			int i = 0;
			for(int i = 0; i < mCount; ++i)
			{
				if(mItems[i] == data)
				{
					mCount = mCount - 1;
					if(i < mCount)
					{
						mItems[i] = mItems[mCount];
					}
					break;
				}
			}
			
			if(mCount == 0 && mParent != 0)
			{
				delete this;
				return 0;
			}
		}
		else
		{
			KDNode*& n = mNodes[child(data)];
			if(n) n = n->remove(data);
			
			if(mNodes[0] == 0 && mNodes[1] == 0)
			{
				delete this;
				return 0;
			}
		}
		
		return this;
	}
	
	void split()
	{
		KDNode* buffer[2] = {0};
		
		for(int i = mCount-1; i >= 0; --i)
		{
			DataType& data = mItems[i];
			KDNode*& n = buffer[child(data)];
			if(n == 0) n = new KDNode(this, data);
			else n->insert(data);
		}
		
		mNodes[0] = buffer[0];
		mNodes[1] = buffer[1];
		mCount = -1;
	}
};

#endif

