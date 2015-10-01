KD::Tree
========

A [KD-Tree](https://en.wikipedia.org/wiki/Kd-tree) in C++


## Quickstart

This is a minimal example, missing the necessary `#include`s and the boilerplate
`Point` class.  The full working example can be found at `test/example.cpp`.

```C++
int main()
{
  typedef KD::Core<3, Point> CORE;

  Point min(0, 0, 0);
  Point max(9, 9, 9);
  KD::Tree<CORE> kdtree(min, max);

  kdtree.insert(Point(1, 2, 3));
  kdtree.insert(Point(8, 2, 5));
  kdtree.insert(Point(1, 0, 8));
  kdtree.insert(Point(3, 1, 4));

  std::cout << kdtree.nearest(Point(5, 5, 5)) << std::endl;
  return 0;
}
```


## Methods

The KD-Tree exposes the following methods:

 - `void insert(const Item& item)`  
   Adds an item to the tree.
   
 - `std::vector<Item> find(const Point& point, int count, Coord radius)`  
   Returns the `count` closest items that lie within `radius` units of `point`.
   
 - `Item nearest(const Point& point)`  
   Returns the item closest to `point`.  If no items are found, this method
   throws a `std::underflow_error`.
   
 - `std::vector<Item> nearest(const Point& point, int count)`  
   Returns the nearest `count` items to `point`.
   
 - `std::vector<Item> within(const Point& point, Coord radius)`  
   Returns all items within `radius` units of `point`.
   
 - `void remove(const Item& item)`  
   Removes an item from the tree.


Some notes on the above:

 - If `count` is given as an argument, the result vector may contain fewer items
   if there are not enough that match the search criteria.
   
 - When results are returned as a `std::vector<Item>`, that vector is sorted by
   increasing distance from the search point.
   
 - Items are passed around by value, not by reference.  As such, the KD-Tree is
   best suited for storing pointers, acting as an index onto existing data.


## Configuration

The KD-Tree takes a single template parameter,  refered to as  `CORE` throughout
the internal  implementation.   This should be a  `struct`  with the appropriate
members to fine-tune the tree's behaviour.  An annotated example:

```C++
struct CORE
{
  typedef Thing* Item;  // The type we store in the tree.
  typedef Point  Point; // The type we read coordinates from.
  typedef float  Coord; // The type of individual coordinates.

  static const int DIMENSIONS =  3; // We're in a three-dimensional space.
  static const int MAX_DEPTH  = 10; // The tree will reach at most ten levels.
  static const int STORAGE    =  8; // Leaves can hold eight items before splitting.

  // Get the distance of a point along the given axis.
  static Coord coordinate(const Point& point, int axis)
  {
    return point[axis];
  }

  // Get the location of an item
  static const Point& point(const Item& item)
  {
    return item->point();
  }
};
```


## Configuration Helpers

Most uses don't need so much control.  For these cases, we provide a `KD::Core`
template class equivalent to:

```C++
template <
  int   DIMENSIONS,
  class Item,
  class Point      = Item,
  class Coord      = double,
  int   STORAGE    = 8,
  int   MAX_DEPTH  = 32
> struct KD::Core {...};
```

It can be used as long as the following functions are available:

 - `Coord Point::operator[] (int axis) const`  
   To extract a coordinate from a point.

 - `const Point& Item::point() const`  
   To extract the position of an item.  This function is only required if `Item`
   is neither the same type as `Point` nor a pointer to `Point`.


## Implementation Details

Internally, a KD-Tree node can be one of three subtypes of the polymorphic
`KD::Node`:

 - `KD::Tree`  
   A node that stores no data.  It simply dispatches method calls to the nodes
   beneath it.

 - `KD::Twig`  
   A leaf node that is above `CORE::MAX_DEPTH`.  A twig node can store up to
   `CORE::STORAGE` items, but further insertions will cause it to split,
   becoming a tree node with new storage nodes beneath it.

 - `KD::Leaf`  
   A permanent leaf node - one at `CORE::MAX_DEPTH`.  A leaf node can hold any
   number of items.  It begins with a capacity of `CORE::STORAGE`, and increases
   its capacity by `CORE::STORAGE` whenever it would overflow.


Note that unlike the canonical KD-Tree, this version stores items only in leaf
(and twig) nodes.  This costs a little extra storage space, but allows for some
nice simplifications:

 - At each split, the space is divided exactly in half along the appropriate
   axis, regardless of the positions of any items.  Oddly placed items cannot
   unbalance the tree.

 - Deletion is much simpler (and should be faster) for the same reason: removing
   an item doesn't affect the division of the space, so the tree doesn't need to
   be rebalanced.
