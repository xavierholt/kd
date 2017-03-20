template <class CORE> class Node
{
protected:
  typedef typename CORE::Item  Item;
  typedef typename CORE::Point Point;
  typedef typename CORE::Coord Coord;
  typedef Chaff::MinFinder<Item,Coord> Finder;

protected:
  const int mDepth;
  const int mAxis;
  Coord     mMidpoint;
  Coord     mMinima[CORE::DIMENSIONS];
  Coord     mMaxima[CORE::DIMENSIONS];

protected:
  Node(const Twig<CORE>* tofork): mDepth(tofork->mDepth), mAxis(tofork->mAxis) {
    mMidpoint = tofork->mMidpoint;

    for(int i = 0; i < CORE::DIMENSIONS; ++i) {
      mMinima[i] = tofork->mMinima[i];
      mMaxima[i] = tofork->mMaxima[i];
    }
  }

  Node(const Tree<CORE>* parent, const Item& item): mDepth(parent->mDepth + 1), mAxis(mDepth % CORE::DIMENSIONS) {
    for(int i = 0; i < CORE::DIMENSIONS; ++i) {
      mMinima[i] = parent->mMinima[i];
      mMaxima[i] = parent->mMaxima[i];
    }

    if(CORE::coordinate(CORE::point(item), parent->mAxis) < parent->mMidpoint) {
      mMaxima[parent->mAxis] = parent->mMidpoint;
    }
    else {
      mMinima[parent->mAxis] = parent->mMidpoint;
    }

    mMidpoint = (mMinima[mAxis] + mMaxima[mAxis]) / 2;
  }

  Node(const Point& min, const Point& max): mDepth(0), mAxis(0) {
    for(int i = 0; i < CORE::DIMENSIONS; ++i) {
      mMinima[i] = CORE::coordinate(min, i);
      mMaxima[i] = CORE::coordinate(max, i);
    }

    mMidpoint = (mMinima[mAxis] + mMaxima[mAxis]) / 2;
  }

  static Coord score(const Point& point, Item item) {
    Coord result(0);
    for(int i = 0; i < CORE::DIMENSIONS; ++i) {
      Coord pc = CORE::coordinate(CORE::point(item), i);
      Coord ic = CORE::coordinate(point, i);
      result  += (pc - ic) * (pc - ic);
    }

    return result;
  }

public:
  virtual ~Node() {
    // Nothing to do.
  }

  virtual Node* insert(const Item& item) = 0;
  virtual Node* remove(const Item& item) = 0;

  virtual void  search(const Point& point, Finder& finder) const = 0;

  std::vector<Item> find(const Point& point, Finder& finder) const {
    search(point, finder);
    return finder.reap();
  }

  std::vector<Item> find(const Point& point, int count, Coord radius) const {
    Finder finder(count, radius * radius);
    return find(point, finder);
  }

  Item nearest(const Point& point) const {
    Finder finder = Finder::byCount(1);
    search(point, finder);
    return finder.thing();
  }

  std::vector<Item> nearest(const Point& point, int count) const {
    Finder finder = Finder::byCount(count);
    return find(point, finder);
  }

  std::vector<Item> within(const Point& point, Coord radius) const {
    Finder finder = Finder::byScore(radius * radius);
    return find(point, finder);
  }
};
