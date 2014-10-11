#ifndef KD_ARRAY_TREE_H
#define KD_ARRAY_TREE_H

#include "../src/finder.h"

namespace KD
{
	namespace Array
	{
		template <typename CORE> class Tree
		{
			typedef typename CORE::DataType  DataType;
			typedef typename CORE::PointType PointType;
			typedef typename CORE::CoordType CoordType;
	
		protected:
			std::vector<DataType> mItems;
	
		public:
			Tree(const PointType&, const PointType&)
			{
				// Nothing to do.
			}
	
			std::vector<DataType> find(Finder<CORE>& finder) const
			{
				search(finder);
				return finder.vector();
			}
	
			std::vector<DataType> find(const PointType& point, int count, CoordType range) const
			{
				Finder<CORE> finder = Finder<CORE>(point, count, range);
				return find(finder);
			}
	
			void insert(const DataType& data)
			{
				mItems.push_back(data);
			}
			
			DataType nearest(const PointType& point) const
			{
				Finder<CORE> finder = Finder<CORE>::byCount(point, 1);
				search(finder);
				return finder.top();
			}
			
			std::vector<DataType> nearest(const PointType& point, int count) const
			{
				Finder<CORE> finder = Finder<CORE>::byCount(point, count);
				return find(finder);
			}
	
			void remove(const DataType& data)
			{
				for(int i = mItems.size() - 1; i >= 0; --i)
				{
					if(mItems[i] == data)
					{
						mItems[i] = mItems.back();
						mItems.pop_back();
						return;
					}
				}
			}
			
			void search(Finder<CORE>& finder) const
			{
				for(int i = mItems.size() - 1; i >= 0; --i)
				{
					finder.check(&mItems[i]);
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

