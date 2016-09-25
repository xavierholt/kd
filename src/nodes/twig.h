template <class CORE> class Twig : public Node<CORE>
{
  typedef typename CORE::Item  Item;
  typedef typename CORE::Point Point;
  typedef typename CORE::Coord Coord;
  typedef Chaff::MinFinder<Item,Coord> Finder;

  friend class Leaf<CORE>;
  friend class Tree<CORE>;

protected:
  int  mCount;
  Item mItems[CORE::STORAGE];

protected:
  Twig(const Tree<CORE>* parent, const Item& item): Node<CORE>(parent, item) {
    mCount    = 1;
    mItems[0] = item;
  }

public:
  Node<CORE>* insert(const Item& item) {
    if(mCount >= CORE::STORAGE) {
      Tree<CORE>* ret = new Tree<CORE>(this);
      for(int i = 0; i < mCount; ++i) {
        ret->insert(mItems[i]);
      }

      ret->insert(item);
      delete this;
      return ret;
    }
    else {
      mItems[mCount] = item;
      mCount += 1;
      return this;
    }
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
