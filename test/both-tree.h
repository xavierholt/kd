#ifndef KD_BOTH_TREE_H
#define KD_BOTH_TREE_H

#include "../src/finder.h"

namespace KD
{
  namespace Both
  {
    template <typename CORE>
    class Tree
    {
      typedef typename CORE::Item  Item;
      typedef typename CORE::Point Point;
      typedef typename CORE::Coord Coord;

    protected:
      const int mDepth;
      const int mAxis;
      int       mCount;
      Coord     mMidpoint;
      Coord     mMinima[CORE::DIMENSIONS];
      Coord     mMaxima[CORE::DIMENSIONS];

      Tree*     mNodes[2];
      Item      mItems[CORE::STORAGE];

    protected:
      Tree(const Tree* parent, const Item& item): mDepth(parent->mDepth + 1), mAxis(mDepth % CORE::DIMENSIONS) {
        mCount    = 1;
        mItems[0] = item;
        mNodes[0] = 0;
        mNodes[1] = 0;

        for(int i = 0; i < CORE::DIMENSIONS; ++i) {
          mMinima[i] = parent->mMinima[i];
          mMaxima[i] = parent->mMaxima[i];
        }

        int pax = parent->mAxis;
        if(CORE::coordinate(CORE::point(item), pax) > parent->mMidpoint) {
          mMinima[pax] = parent->mMidpoint;
        }
        else {
          mMaxima[pax] = parent->mMidpoint;
        }

        mMidpoint = (mMinima[mAxis] + mMaxima[mAxis]) / 2;
      }

      int child(const Point& point) const {
        return (CORE::coordinate(point, mAxis) > mMidpoint);
      }

    public:
      Tree(const Point& min, const Point& max): mDepth(0), mAxis(0) {
        mCount    = 0;
        mNodes[0] = 0;
        mNodes[1] = 0;

        for(int i = 0; i < CORE::DIMENSIONS; ++i) {
          mMinima[i] = CORE::coordinate(min, i);
          mMaxima[i] = CORE::coordinate(max, i);
        }

        mMidpoint = (mMinima[mAxis] + mMaxima[mAxis]) / 2;
      }

      ~Tree() {
        delete mNodes[0];
        delete mNodes[1];
      }

      void insert(const Item& item) {
        if(mCount >= CORE::STORAGE) {
          int c = child(CORE::point(item));
          if(mNodes[c]) mNodes[c]->insert(item);
          else mNodes[c] = new Tree(this, item);
        }
        else {
          mItems[mCount] = item;
          mCount += 1;
        }
      }

      Tree* remove(const Item& item) {
        int i;
        for(i = 0; i < mCount; ++i) {
          if(mItems[i] == item) {
            mCount = mCount - 1;
            if(i < mCount) {
              mItems[i] = mItems[mCount];
            }
            break;
          }
        }

        if(i == mCount) {
          Tree*& n = mNodes[child(CORE::point(item))];
          if(n) n = n->remove(item);
        }

        if(mCount == 0 && mNodes[0] == 0 && mNodes[1] == 0 && mDepth != 0) {
          delete this;
          return 0;
        }

        return this;
      }

      void search(const Point& point, Finder<Item, Coord>& finder) const {
        for(int i = mCount - 1; i >= 0; --i) {
          Coord score(0);
          for(int d = 0; d < CORE::DIMENSIONS; ++d) {
            Coord pc = CORE::coordinate(point, d);
            Coord ic = CORE::coordinate(CORE::point(mItems[i]), d);
            score += (pc - ic) * (pc - ic);
          }

          finder.check(mItems[i], score);
        }

        Coord diff = CORE::coordinate(point, mAxis) - mMidpoint;
        int idx = (diff > 0);

        if(mNodes[idx]) {
          mNodes[idx]->search(point, finder);
        }

        if(diff * diff < finder.score() && mNodes[idx ^ 1]) {
          mNodes[idx ^ 1]->search(point, finder);
        }
      }
    };
  }
}

#endif

