#ifndef KD_UNION_TREE_H
#define KD_UNION_TREE_H

#include "../src/finder.h"

namespace KD
{
	namespace Union
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
	
			union
			{
				DataType mItems[CORE::STORAGE];
				Tree*    mNodes[2];
			};
	
		protected:
			Tree(const Tree* parent, const DataType& data):
				mDepth(parent->mDepth + 1),
				mAxis(mDepth % CORE::DIMENSIONS)
			{
				mCount    = 1;
				mItems[0] = data;
		
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
			
			void split()
			{
				Tree* buffer[2] = {0};
		
				for(int i = mCount-1; i >= 0; --i)
				{
					DataType& data = mItems[i];
					Tree*& n = buffer[child(CORE::point(data))];
					if(n == 0) n = new Tree(this, data);
					else n->insert(data);
				}
		
				mNodes[0] = buffer[0];
				mNodes[1] = buffer[1];
				mCount = -1;
			}
	
		public:
			Tree(const PointType& min, const PointType& max): mDepth(0), mAxis(0)
			{
				mCount    = -1;
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
				if(mCount < 0)
				{
					delete mNodes[0];
					delete mNodes[1];
				}
			}
	
			std::vector<DataType> find(Finder<CORE>& finder) const
			{
				search(finder);
				return finder.vector();
			}
	
			std::vector<DataType> find(const PointType& point, int count, CoordType range) const
			{
				return find(Finder<CORE>(point, count, range));
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
					Tree*& n = mNodes[child(CORE::point(data))];
					if(n == 0) n = new Tree(this, data);
					else n->insert(data);
				}
			}
			
			DataType nearest(const PointType& point) const
			{
				Finder<CORE> finder = Finder<CORE>::byCount(point, 1);
				search(finder);
				return finder.top();
			}
			
			std::vector<DataType> nearest(const PointType& point, int count) const
			{
				return find(Finder<CORE>::byCount(point, count));
			}
	
			Tree* remove(const DataType& data)
			{
				if(mCount >= 0)
				{
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
			
					if(mCount == 0 && mDepth != 0)
					{
						delete this;
						return 0;
					}
				}
				else
				{
					Tree*& n = mNodes[child(CORE::point(data))];
					if(n) n = n->remove(data);
			
					if(mNodes[0] == 0 && mNodes[1] == 0 &&mDepth != 0)
					{
						delete this;
						return 0;
					}
				}
		
				return this;
			}
			
			void search(Finder<CORE>& finder) const
			{
				if(mCount >= 0)
				{
					for(int i = 0; i < mCount; ++i)
					{
						finder.check(&mItems[i]);
					}
				}
				else
				{
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
			}
	
			std::vector<DataType> within(const PointType& point, CoordType range) const
			{
				return find(Finder<CORE>::byRange(point, range));
			}
		};
	}
}

#endif

