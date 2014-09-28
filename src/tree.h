#ifndef KD_TREE_H
#define KD_TREE_H

#include "node.h"

template <typename CORE> class KDTree
{
public:
	typedef typename CORE::DataType  DataType;
	typedef typename CORE::PointType PointType;
	typedef typename CORE::UnitType  UnitType;
	typedef          TFinder<CORE>   Finder;
	typedef          KDNode<CORE>    Node;
	
protected:
	Node* mRoot;
	
public:
	KDTree(const PointType& min, const PointType& max)
	{
		mRoot = new Node(min, max);
	}
	
	void find(Finder& finder) const
	{
		mRoot->find(finder);
	}
	
	void insert(const DataType& data)
	{
		mRoot->insert(data);
	}
	
	DataType nearest(const DataType& data) const
	{
		return nearest(CORE::point(data));
	}
	
	DataType nearest(const PointType& point) const
	{
		Finder finder = Finder::byCount(point, 1);
		mRoot->find(finder);
		if(finder.count() != 1)
		{
			throw "Help!  Help!  I'm being oppressed!";
		}
		
		return finder.top();
	}
	
	std::vector<DataType> nearest(const DataType& data, int count) const
	{
		return nearest(CORE::point(data), count);
	}
	
	std::vector<DataType> nearest(const PointType& point, int count) const
	{
		Finder finder = Finder::byCount(point, count);
		find(finder);
		return finder.vector();
	}
	
	void remove(const DataType& data)
	{
		mRoot->remove(data);
	}
	
	std::vector<DataType> within(const DataType& data, UnitType range) const
	{
		return within(CORE::point(data), range);
	}
	
	std::vector<DataType> within(const PointType& point, UnitType range) const
	{
		Finder finder = Finder::byRange(point, range);
		find(finder);
		return finder.vector();
	}
};

#endif

