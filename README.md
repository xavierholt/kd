KD::Tree
========

A Simple KD-Tree in C++


## Quickstart

This is a minimal example, missing the necessary `#include`s and the boilerplate
that defines the `Point` class.  The full working example can be found at
`test/quickstart.cpp`.

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

 - `void insert(const DataType& datum)`  
   Adds a datum to the tree.
   
 - `std::vector<DataType> find(const PointType& point, int count, CoordType radius)`  
   Returns the `count` closest data that lie within `radius` units of `point`.
   
 - `DataType nearest(const PointType& point)`  
   Returns the datum closest to `point`.  If no data are found, this method
   throws a `std::underflow_error`.
   
 - `std::vector<DataType> find(const PointType& point, int count)`  
   Returns the nearest `count` data to `point`.
   
 - `std::vector<DataType> find(const PointType& point, CoordType radius)`
   Returns all data within `radius` units of `point`.
   
 - `void remove(const DataType& datum)`  
   Removes a datum from the tree.

Some notes on the above:

 - If `count` is given as an argument,  the result vector will contain _at most_
   `count` data.   It may contain fewer - or even none - if there are not enough
   data that match the search criteria.
   
 - When results are returned as a `std::vector<DataType>`, that vector is sorted
   by increasing distance from the search point.
   
 - Data are returned _by value_, not by reference.  As such, the KD-Tree is best
   used not as a primary storage mechanism, but as an index onto existing data.


## Configuration

The KD-Tree takes a single template parameter,  refered to as  `CORE` throughout
the internal  implementation.   This should be a  `struct`  with the appropriate
members to fine-tune the tree's behaviour.  An annotated example:

```C++
struct CORE
{
	typedef Thing DataType;  // The type we store in the tree.
	typedef Point PointType; // The type we read coordinates from.
	typedef float CoordType; // The type we store coordinates as internally.
	
	static const int DIMENSIONS =  3; // We're in a three-dimensional space.
	static const int MAX_DEPTH  = 10; // Caps the depth of the tree.
	static const int STORAGE    =  8; // Initial storage space in leaf nodes.
	
	// Get the distance of a point along the given axis.
	static CoordType coordinate(const PointType& point, int axis)
	{
		return point[axis];
	}
	
	// Get the location of a datum.
	static const PointType& point(const DataType& datum)
	{
		return datum->point();
	}
};
```


## Configuration Helpers

Most uses don't need so much control.   For these cases, we provide a `KD::Core`
template class equivalent to:

```C++
template <
    int   DIMENSIONS,
    class DataType,
    class PointType  = DataType,
    class CoordType  = double,
    int   STORAGE    = 32,
    int   MAX_DEPTH  = 32
> class KD::Core {...};
```

This can be used provided that:
 - `PointType` has a valid  `PointType::operator [] (int) const`  that returns a
   value that can be implicily converted to a `CoordType`.
 - If `PointType` differs from `DataType`  (and `DataType` is not a pointer to a
   `PointType`), `DataType` must support the function `DataType:: point() const`
   to retrieve a `const PointType&`.

