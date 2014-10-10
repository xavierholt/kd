template <class CORE> class Tree : public Node<CORE>
{
	typedef typename CORE::DataType  DataType;
	typedef typename CORE::PointType PointType;
	typedef typename CORE::CoordType CoordType;
	
	friend class Leaf<CORE>;
	friend class Twig<CORE>;
	
protected:
	Node<CORE>* mNodes[2];
	
protected:
	Tree(const Twig<CORE>* tofork): Node<CORE>(tofork)
	{
		mNodes[0] = 0;
		mNodes[1] = 0;
	}
	
	int child(const PointType& point) const
	{
		return (CORE::coordinate(point, this->mAxis) > this->mMidpoint);
	}
	
public:
	Tree(const PointType& min, const PointType& max): Node<CORE>(min, max)
	{
		mNodes[0] = 0;
		mNodes[1] = 0;
	}
	
	~Tree()
	{
		delete mNodes[0];
		delete mNodes[1];
	}
	
	Node<CORE>* insert(const DataType& data);
	
	Node<CORE>* remove(const DataType& data)
	{
		int c = child(CORE::point(data));
		if(mNodes[c])
		{
			mNodes[c] = mNodes[c]->remove(data);
			if(mNodes[0] == 0 && mNodes[1] == 0 && this->mDepth != 0)
			{
				delete this;
				return 0;
			}
		}
		
		return this;
	}
	
	void search(Finder<CORE>& finder) const
	{
		CoordType diff = finder.coordinate(this->mAxis) - this->mMidpoint;
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
};

