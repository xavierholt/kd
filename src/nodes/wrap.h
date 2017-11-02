template <class CORE> class Wrap : public Node<CORE>
{
  typedef typename CORE::Item  Item;
  typedef typename CORE::Point Point;
  typedef typename CORE::Coord Coord;
  typedef Chaff::MinFinder<Item,Coord> Finder;

protected:
  Node<CORE>* mChild;

protected:
  Wrap(const Point& min, const Point& max, int wraps, int depth): Node<CORE>(min, max, depth) {
    if(wraps > 1) {
      mChild = new Wrap<CORE>(min, max, wraps - 1, depth + 1);
    }
    else {
      mChild = new Tree<CORE>(min, max);
    }
  }

  static Coord mod(Coord num, Coord dnm) {
    int n = num / dnm - (num < 0);
    return num - n * dnm;
  }

public:
  Wrap(const Point& min, const Point& max, int wraps = CORE::DIMENSIONS): Node<CORE>(min, max) {
    if(wraps > 1) {
      mChild = new Wrap<CORE>(min, max, wraps - 1, 1);
    }
    else {
      mChild = new Tree<CORE>(min, max);
    }
  }

  ~Wrap() {
    delete mChild;
  }

  Node<CORE>* insert(const Item& item) {
    mChild->insert(item);
    return this;
  }

  Node<CORE>* remove(const Item& item) {
    mChild->remove(item);
    return this;
  }

  void search(const Point& point, Finder& finder) const {
    const Coord min = this->mMinima[this->mAxis];
    const Coord max = this->mMaxima[this->mAxis];
    const Coord rng = max - min;
    const Coord val = mod(point[this->mAxis] - min, rng) + min;

    Point scratch(point);
    scratch[this->mAxis] = val;
    mChild->search(scratch, finder);

    if(finder.score() > val - min) {
      scratch[this->mAxis] = val + rng;
      mChild->search(scratch, finder);
    }

    if(finder.score() > max - val) {
      scratch[this->mAxis] = val - rng;
      mChild->search(scratch, finder);
    }
  }
};
