template <class CORE> class Node
{
	typedef typename CORE::DataType  DataType;
	typedef typename CORE::PointType PointType;
	typedef typename CORE::CoordType CoordType;
	
protected:
	const int mDepth;
	const int mAxis;
	CoordType mMidpoint;
	CoordType mMinima[CORE::DIMENSIONS];
	CoordType mMaxima[CORE::DIMENSIONS];
	
protected:
	Node(const Twig<CORE>* tofork):
		mDepth(tofork->mDepth),
		mAxis(tofork->mAxis)
	{
		mMidpoint = tofork->mMidpoint;
		
		for(int i = 0; i < CORE::DIMENSIONS; ++i)
		{
			mMinima[i] = tofork->mMinima[i];
			mMaxima[i] = tofork->mMaxima[i];
		}
	}
	
	Node(const Tree<CORE>* parent, const DataType& data):
		mDepth(parent->mDepth + 1),
		mAxis(mDepth % CORE::DIMENSIONS)
	{
		for(int i = 0; i < CORE::DIMENSIONS; ++i)
		{
			mMinima[i] = parent->mMinima[i];
			mMaxima[i] = parent->mMaxima[i];
		}
		
		if(CORE::coordinate(CORE::point(data), parent->mAxis) < parent->mMidpoint)
		{
			mMaxima[parent->mAxis] = parent->mMidpoint;
		}
		else
		{
			mMinima[parent->mAxis] = parent->mMidpoint;
		}
		
		mMidpoint = (mMinima[mAxis] + mMaxima[mAxis]) / 2;
	}
	
	Node(const PointType& min, const PointType& max): mDepth(0), mAxis(0)
	{
		for(int i = 0; i < CORE::DIMENSIONS; ++i)
		{
			mMinima[i] = CORE::coordinate(min, i);
			mMaxima[i] = CORE::coordinate(max, i);
		}
		
		mMidpoint = (mMinima[mAxis] + mMaxima[mAxis]) / 2;
	}
	
public:
	virtual ~Node()
	{
		// Nothing to do.
	}
	
	virtual Node* insert(const DataType& data)       = 0;
	virtual Node* remove(const DataType& data)       = 0;
	virtual void  search(Finder<CORE>& finder) const = 0;
	
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
	
	std::vector<DataType> within(const PointType& point, CoordType range) const
	{
		Finder<CORE> finder = Finder<CORE>::byRange(point, range);
		return find(finder);
	}
};

