#ifndef HEAP_H
#define HEAP_H

#include <stdexcept>

template <class Item, class COMP> class Heap
{
protected:
	int   mCount;
	int   mCapacity;
	Item* mItems;
	COMP  C;
	
protected:
	void reallocate(int capacity)
	{
		mCapacity = capacity;
		Item* temp = new Item[mCapacity];
		for(int i = 0; i < mCount; ++i)
		{
			temp[i] = mItems[i];
		}
		
		delete [] mItems;
		mItems = temp;
	}
	
public:
	Heap(int capacity)
	{
		mCount = 0;
		mCapacity = capacity;
		mItems = new Item[capacity];
	}
	
	~Heap()
	{
		delete [] mItems;
	}
	
	int capacity() const
	{
		return mCapacity;
	}
	
	void clear()
	{
		mCount = 0;
	}
	
	int count() const
	{
		return mCount;
	}
	
	void pop()
	{
		if(mCount < 1)
		{
			throw std::underflow_error("Underflow in Heap::pop()");
		}
		
		int p;
		int c = 0;
		mCount -= 1;
	
		while(true)
		{
			p = c;
			c = p * 2 + 1;
			if(c + 1 < mCount)
			{
				c += C(mItems[c+1], mItems[c]);
			}
			if(c < mCount && C(mItems[c], mItems[mCount]))
			{
				mItems[p] = mItems[c];
			}
			else
			{
				break;
			}
		}
	
		mItems[p] = mItems[mCount];
	}
	
	void push(const Item& item)
	{
		if(mCount >= mCapacity)
		{
			reallocate(mCount + 100);
		}
		
		int c = mCount;
		while(c > 0)
		{
			int p = (c - 1) / 2;
			if(C(mItems[p], item)) break;
			mItems[c] = mItems[p];
			c = p;
		}
	
		mItems[c] = item;
		mCount += 1;
	}
	
	void reserve(int capacity)
	{
		if(capacity > mCapacity)
		{
			reallocate(capacity);
		}
	}
	
	void resize(int capacity)
	{
		if(capacity < mCount)
		{
			throw std::overflow_error("Overflow in Heap::resize()");
		}
		
		if(capacity != mCapacity)
		{
			reallocate(capacity);
		}
	}
	
	const Item& top() const
	{
		return mItems[0];
	}
};

#endif
