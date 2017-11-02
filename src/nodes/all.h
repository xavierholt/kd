template <typename CORE> class Node;
template <typename CORE> class Leaf;
template <typename CORE> class Tree;
template <typename CORE> class Twig;

#include "node.h"
#include "leaf.h"
#include "tree.h"
#include "twig.h"
#include "wrap.h"

template <typename CORE>
Node<CORE>* Tree<CORE>::insert(const typename CORE::Item& item) {
  const Coord c = CORE::coordinate(CORE::point(item), this->mAxis);
  const int   i = (c > this->mMidpoint);

  if(mNodes[i]) {
    mNodes[i] = mNodes[i]->insert(item);
  }
  else if(this->mDepth + 1 < CORE::MAX_DEPTH) {
    mNodes[i] = new Twig<CORE>(this, item);
  }
  else {
    mNodes[i] = new Leaf<CORE>(this, item);
  }

  return this;
}
