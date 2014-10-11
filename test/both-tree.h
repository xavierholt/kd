#ifndef KD_BOTH_TREE_H
#define KD_BOTH_TREE_H

#include "../src/finder.h"

namespace KD
{
	namespace Both
	{
		template <typename CORE> class Tree
		{
			typedef typename CORE::DataType  DataType;
			typedef typename CORE::PointType PointType;
			typedef typename CORE::CoordType CoordType;
			
		protected:
			const int mDepth;
			const int mAxis;
			int       mCount;
			CoordType mMidpoint;
			CoordType mMinima[CORE::DIMENSIONS];
			CoordType mMaxima[CORE::DIMENSIONS];
	
			Tree*     mNodes[2];
			DataType  mItems[CORE::STORAGE];
	
		protected:
			Tree(const Tree* parent, const DataType& data):
				mDepth(parent->mDepth + 1),
				mAxis(mDepth % CORE::DIMENSIONS)
			{
				mCount    = 1;
				mItems[0] = data;
				mNodes[0] = 0;
				mNodes[1] = 0;
		
				for(int i = 0; i < CORE::DIMENSIONS; ++i)
				{
					mMinima[i] = parent->mMinima[i];
					mMaxima[i] = parent->mMaxima[i];
				}
		
				int pax = parent->mAxis;
				if(CORE::coordinate(CORE::point(data), pax) > parent->mMidpoint)
				{
					mMinima[pax] = parent->mMidpoint;
				}
				else
				{
					mMaxima[pax] = parent->mMidpoint;
				}
		
				mMidpoint = (mMinima[mAxis] + mMaxima[mAxis]) / 2;
			}
			
			int child(const PointType& point) const
			{
				return (CORE::coordinate(point, mAxis) > mMidpoint);
			}
	
		public:
			Tree(const PointType& min, const PointType& max): mDepth(0), mAxis(0)
			{
				mCount    = 0;
				mNodes[0] = 0;
				mNodes[1] = 0;
		
				for(int i = 0; i < CORE::DIMENSIONS; ++i)
				{
					mMinima[i] = CORE::coordinate(min, i);
					mMaxima[i] = CORE::coordinate(max, i);
				}
		
				mMidpoint = (mMinima[mAxis] + mMaxima[mAxis]) / 2;
			}
	
			~Tree()
			{
				delete mNodes[0];
				delete mNodes[1];
			}
	
			std::vector<DataType> find(Finder<CORE>& finder) const
			{
				search(finder);
				return finder.vector();
			}
	
			std::vector<DataType> find(const PointType& point, int count, CoordType range) const
			{
				Finder<CORE> finder(point, count, range);
				return find(finder);
			}
	
			void insert(const DataType& data)
			{
				if(mCount >= CORE::STORAGE)
				{
					int c = child(CORE::point(data));
					if(mNodes[c]) mNodes[c]->insert(data);
					else mNodes[c] = new Tree(this, data);
				}
				else
				{
					mItems[mCount] = data;
					mCount += 1;
				}
			}
	
			DataType nearest(const PointType& point) const
			{
				Finder<CORE> finder = Finder<CORE>::byCount(point, 1);
				find(finder);
				return finder.top();
			}
	
			std::vector<DataType> nearest(const PointType& point, int count) const
			{
				Finder<CORE> finder = Finder<CORE>::byCount(point, count);
				return find(finder);
			}
			
			Tree* remove(const DataType& data)
			{
				int i;
				for(i = 0; i < mCount; ++i)
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
		
				if(i == mCount)
				{
					Tree*& n = mNodes[child(CORE::point(data))];
					if(n) n = n->remove(data);
				}
		
				if(mCount == 0 && mNodes[0] == 0 && mNodes[1] == 0 && mDepth != 0)
				{
					delete this;
					return 0;
				}
		
				return this;
			}
			
			void search(Finder<CORE>& finder) const
			{
				for(int i = 0; i < mCount; ++i)
				{
					finder.check(&mItems[i]);
				}
		
				CoordType diff = finder.coordinate(mAxis) - mMidpoint;
				int idx = (diff > 0);
		
				if(mNodes[idx])
				{
					mNodes[idx]->search(finder);
				}
		
				if(diff * diff < finder.rangeSquared() && mNodes[idx ^ 1])
				{
					mNodes[idx ^ 1]->search(finder);
				}
			}
			
			std::vector<DataType> within(const PointType& point, CoordType range) const
			{
				Finder<CORE> finder = Finder<CORE>::byRange(point, range);
				return find(finder);
			}
		};
	}
}

#endif

