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
	
			std::vector<DataType> find(Finder<CORE>& finder)
			{
				search(finder);
				return finder.vector();
			}
	
			std::vector<DataType> find(const PointType& point, int count, CoordType range)
			{
				return find(Finder<CORE>(point, count, range));
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
				return find(Finder<CORE>::byCount(point, count));
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
			
			void search(Finder<CORE>& finder)
			{
				for(int i = mItems.size() - 1; i >= 0; --i)
				{
					finder.check(&mItems[i]);
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

