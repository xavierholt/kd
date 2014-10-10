template <typename CORE> class Node;
template <typename CORE> class Leaf;
template <typename CORE> class Tree;
template <typename CORE> class Twig;

#include "node.h"
#include "leaf.h"
#include "tree.h"
#include "twig.h"

template <typename CORE>
Node<CORE>* Tree<CORE>::insert(const typename CORE::DataType& data)
{
	int c = child(CORE::point(data));
	if(mNodes[c])
	{
		mNodes[c] = mNodes[c]->insert(data);
	}
	else if(this->mDepth + 1 < CORE::MAX_DEPTH)
	{
		mNodes[c] = new Twig<CORE>(this, data);
	}
	else
	{
		mNodes[c] = new Leaf<CORE>(this, data);
	}
	
	return this;
}
