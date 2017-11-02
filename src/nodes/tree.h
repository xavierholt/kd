template <class CORE> class Tree : public Node<CORE>
{
  typedef typename CORE::Item  Item;
  typedef typename CORE::Point Point;
  typedef typename CORE::Coord Coord;
  typedef Chaff::MinFinder<Item,Coord> Finder;

  friend class Leaf<CORE>;
  friend class Twig<CORE>;

protected:
  Node<CORE>* mNodes[2];

protected:
  Tree(const Twig<CORE>* tofork): Node<CORE>(tofork) {
    mNodes[0] = 0;
    mNodes[1] = 0;
  }

public:
  Tree(const Point& min, const Point& max): Node<CORE>(min, max) {
    mNodes[0] = 0;
    mNodes[1] = 0;
  }

  ~Tree() {
    delete mNodes[0];
    delete mNodes[1];
  }

  // This creates the other types, so it
  // has to be defined out-of-line:
  Node<CORE>* insert(const Item& item);

  Node<CORE>* remove(const Item& item) {
    const Coord c = CORE::coordinate(CORE::point(item), this->mAxis);
    const int   i = (c > this->mMidpoint);

    if(mNodes[i]) {
      mNodes[i] = mNodes[i]->remove(item);
      if(mNodes[0] == 0 && mNodes[1] == 0 && this->mDepth != 0) {
        delete this;
        return 0;
      }
    }

    return this;
  }

  void search(const Point& point, Finder& finder) const {
    const Coord d = CORE::coordinate(point, this->mAxis) - this->mMidpoint;
    const int   i = (d > 0);

    if(mNodes[i]) {
      mNodes[i]->search(point, finder);
    }

    if(d * d < finder.score() && mNodes[i ^ 1]) {
      mNodes[i ^ 1]->search(point, finder);
    }
  }
};
