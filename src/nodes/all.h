template <typename CORE> class Node;
template <typename CORE> class Leaf;
template <typename CORE> class Tree;
template <typename CORE> class Twig;

#include "node.h"
#include "leaf.h"
#include "tree.h"
#include "twig.h"

template <typename CORE>
Node<CORE>* Tree<CORE>::insert(const typename CORE::Item& item) {
  int c = child(CORE::point(item));
  if(mNodes[c]) {
    mNodes[c] = mNodes[c]->insert(item);
  }
  else if(this->mDepth + 1 < CORE::MAX_DEPTH) {
    mNodes[c] = new Twig<CORE>(this, item);
  }
  else {
    mNodes[c] = new Leaf<CORE>(this, item);
  }

  return this;
}
