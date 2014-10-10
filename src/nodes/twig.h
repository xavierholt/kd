template <class CORE> class Twig : public Node<CORE>
{
	typedef typename CORE::DataType  DataType;
	typedef typename CORE::PointType PointType;
	typedef typename CORE::CoordType CoordType;
	
	friend class Leaf<CORE>;
	friend class Tree<CORE>;
	
protected:
	int      mCount;
	DataType mItems[CORE::STORAGE];
	
protected:
	Twig(const Tree<CORE>* parent, const DataType& data): Node<CORE>(parent, data)
	{
		mCount    = 1;
		mItems[0] = data;
	}
	
public:
	Node<CORE>* insert(const DataType& data)
	{
		if(mCount >= CORE::STORAGE)
		{
			Tree<CORE>* ret = new Tree<CORE>(this);
			for(int i = 0; i < mCount; ++i)
			{
				ret->insert(mItems[i]);
			}
			
			ret->insert(data);
			delete this;
			return ret;
		}
		else
		{
			mItems[mCount] = data;
			mCount += 1;
			return this;
		}
	}
	
	Node<CORE>* remove(const DataType& data)
	{
		for(int i = 0; i < mCount; ++i)
		{
			if(mItems[i] == data)
			{
				mCount -= 1;
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

