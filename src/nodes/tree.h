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

  int child(const Point& point) const {
    return (CORE::coordinate(point, this->mAxis) > this->mMidpoint);
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
    int c = child(CORE::point(item));
    if(mNodes[c]) {
      mNodes[c] = mNodes[c]->remove(item);
      if(mNodes[0] == 0 && mNodes[1] == 0 && this->mDepth != 0) {
        delete this;
        return 0;
      }
    }

    return this;
  }

  void search(const Point& point, Finder& finder) const {
    Coord diff = CORE::coordinate(point, this->mAxis) - this->mMidpoint;
    int idx = (diff > 0);

    if(mNodes[idx]) {
      mNodes[idx]->search(point, finder);
    }

    if(diff * diff < finder.score() && mNodes[idx ^ 1]) {
      mNodes[idx ^ 1]->search(point, finder);
    }
  }
};
