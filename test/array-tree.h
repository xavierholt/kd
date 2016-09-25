#ifndef KD_ARRAY_TREE_H
#define KD_ARRAY_TREE_H

#include "../src/finder.h"

namespace KD
{
  namespace Array
  {
    template <typename CORE>
    class Tree
    {
      typedef typename CORE::Item  Item;
      typedef typename CORE::Point Point;
      typedef typename CORE::Coord Coord;

    protected:
      std::vector<Item> mItems;

    public:
      Tree(const Point&, const Point&) {
        // Nothing to do.
      }

      void insert(const Item& item) {
        mItems.push_back(item);
      }

      void remove(const Item& item) {
        for(int i = mItems.size() - 1; i >= 0; --i) {
          if(mItems[i] == item) {
            mItems[i] = mItems.back();
            mItems.pop_back();
            return;
          }
        }
      }

      void search(const Point& point, Finder<Item, Coord>& finder) const {
        for(int i = mItems.size() - 1; i >= 0; --i) {
          Coord score(0);
          for(int d = 0; d < CORE::DIMENSIONS; ++d) {
            Coord pc = CORE::coordinate(point, d);
            Coord ic = CORE::coordinate(CORE::point(mItems[i]), d);
            score += (pc - ic) * (pc - ic);
          }

          finder.check(mItems[i], score);
        }
      }
    };
  }
}

#endif

