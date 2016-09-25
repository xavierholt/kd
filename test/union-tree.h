#ifndef KD_UNION_TREE_H
#define KD_UNION_TREE_H

#include "../src/finder.h"

namespace KD
{
  namespace Union
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

      union
      {
        Item  mItems[CORE::STORAGE];
        Tree* mNodes[2];
      };

    protected:
      Tree(const Tree* parent, const Item& item): mDepth(parent->mDepth + 1), mAxis(mDepth % CORE::DIMENSIONS) {
        mCount    = 1;
        mItems[0] = item;

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

      void split() {
        Tree* buffer[2] = {0};

        for(int i = mCount-1; i >= 0; --i) {
          Item& item = mItems[i];
          Tree*& n = buffer[child(CORE::point(item))];
          if(n == 0) n = new Tree(this, item);
          else n->insert(item);
        }

        mNodes[0] = buffer[0];
        mNodes[1] = buffer[1];
        mCount = -1;
      }

    public:
      Tree(const Point& min, const Point& max): mDepth(0), mAxis(0) {
        mCount    = -1;
        mNodes[0] = 0;
        mNodes[1] = 0;

        for(int i = 0; i < CORE::DIMENSIONS; ++i) {
          mMinima[i] = CORE::coordinate(min, i);
          mMaxima[i] = CORE::coordinate(max, i);
        }

        mMidpoint = (mMinima[mAxis] + mMaxima[mAxis]) / 2;
      }

      ~Tree() {
        if(mCount < 0) {
          delete mNodes[0];
          delete mNodes[1];
        }
      }

      void insert(const Item& item) {
        if(mCount >= CORE::STORAGE) {
          this->split();
        }

        if(mCount >= 0) {
          mItems[mCount] = item;
          mCount += 1;
        }
        else {
          Tree*& n = mNodes[child(CORE::point(item))];
          if(n == 0) n = new Tree(this, item);
          else n->insert(item);
        }
      }

      Tree* remove(const Item& item) {
        if(mCount >= 0) {
          for(int i = 0; i < mCount; ++i) {
            if(mItems[i] == item) {
              mCount = mCount - 1;
              if(i < mCount) {
                mItems[i] = mItems[mCount];
              }
              break;
            }
          }

          if(mCount == 0 && mDepth != 0) {
            delete this;
            return 0;
          }
        }
        else {
          Tree*& n = mNodes[child(CORE::point(item))];
          if(n) n = n->remove(item);

          if(mNodes[0] == 0 && mNodes[1] == 0 &&mDepth != 0) {
            delete this;
            return 0;
          }
        }

        return this;
      }

      void search(const Point& point, Finder<Item, Coord>& finder) const {
        if(mCount >= 0) {
          for(int i = 0; i < mCount; ++i) {
            Coord score(0);
            for(int d = 0; d < CORE::DIMENSIONS; ++d) {
              Coord pc = CORE::coordinate(point, d);
              Coord ic = CORE::coordinate(CORE::point(mItems[i]), d);
              score += (pc - ic) * (pc - ic);
            }

            finder.check(mItems[i], score);
          }
        }
        else {
          Coord diff = CORE::coordinate(point, mAxis) - mMidpoint;
          int idx = (diff > 0);

          if(mNodes[idx]) {
            mNodes[idx]->search(point, finder);
          }

          if(diff * diff < finder.score() && mNodes[idx ^ 1]) {
            mNodes[idx ^ 1]->search(point, finder);
          }
        }
      }
    };
  }
}

#endif

