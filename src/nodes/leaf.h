template <class CORE> class Leaf : public Node<CORE>
{
  typedef typename CORE::Item  Item;
  typedef typename CORE::Point Point;
  typedef typename CORE::Coord Coord;
  typedef Chaff::MinFinder<Item,Coord> Finder;

  friend class Tree<CORE>;
  friend class Twig<CORE>;

protected:
  int   mCount;
  int   mCapacity;
  Item* mItems;

protected:
  Leaf(const Tree<CORE>* parent, const Item& item): Node<CORE>(parent, item) {
    mCount    = 1;
    mCapacity = CORE::STORAGE;
    mItems    = new Item[mCapacity];
    mItems[0] = item;
  }

public:
  ~Leaf() {
    delete [] mItems;
  }

  Node<CORE>* insert(const Item& item) {
    if(mCount >= mCapacity) {
      mCapacity += CORE::STORAGE;
      Item* temp = new Item[mCapacity];
      for(int i = 0; i < mCount; ++i) {
        temp[i] = mItems[i];
      }

      delete [] mItems;
      mItems = temp;
    }

    mItems[mCount] = item;
    mCount += 1;
    return this;
  }

  Node<CORE>* remove(const Item& item) {
    for(int i = 0; i < mCount; ++i) {
      if(mItems[i] == item) {
        mCount -= 1;
        if(mCount == 0) {
          delete this;
          return 0;
        }
        else {
          mItems[i] = mItems[mCount];
          return this;
        }
      }
    }

    return this;
  }

  void search(const Point& point, Finder& finder) const {
    for(int i = 0; i < mCount; ++i) {
      Coord score = this->score(point, mItems[i]);
      finder.sow(mItems[i], score);
    }
  }
};
