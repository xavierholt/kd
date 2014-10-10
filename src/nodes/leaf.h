template <class CORE> class Leaf : public Node<CORE>
{
	typedef typename CORE::DataType  DataType;
	typedef typename CORE::PointType PointType;
	typedef typename CORE::CoordType CoordType;
	
	friend class Tree<CORE>;
	friend class Twig<CORE>;
	
protected:
	int       mCount;
	int       mCapacity;
	DataType* mItems;
	
protected:
	Leaf(const Tree<CORE>* parent, const DataType& data): Node<CORE>(parent, data)
	{
		mCount    = 1;
		mCapacity = CORE::STORAGE;
		mItems    = new DataType[mCapacity];
		mItems[0] = data;
	}
	
public:
	~Leaf()
	{
		delete [] mItems;
	}
	
	Node<CORE>* insert(const DataType& data)
	{
		if(mCount >= CORE::STORAGE)
		{
			mCapacity += CORE::STORAGE;
			DataType* temp = new DataType[mCapacity];
			for(int i = 0; i < mCount; ++i)
			{
				temp[i] = mItems[i];
			}
			
			delete [] mItems;
			mItems = temp;
		}
		
		mItems[mCount] = data;
		mCount += 1;
		return this;
	}
	
	Node<CORE>* remove(const DataType& data)
	{
		for(int i = 0; i < mCount; ++i)
		{
			if(mItems[i] == data)
			{
				mCount = mCount - 1;
				mItems[i] = mItems[mCount];
				break;
			}
		}
		
		if(mCount == 0)
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
	}
};

