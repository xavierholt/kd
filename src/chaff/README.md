[![Build Status](https://travis-ci.org/xavierholt/chaff.svg)](https://travis-ci.org/xavierholt/chaff)

# Chaff

So  you have some things.   They're not in any particular order.   You
don't want to sort them.  But you want to find the best three of them.
So what can you do?

```C++
// Full code in test/example.cpp!
std::vector<int> things(1000000);
for(int& i: things) {
  i = rand();
}

auto finder = Chaff::MaxFinder<int,int>::byCount(3);
for(int i: things) {
  finder.sow(i, i);
}

std::cout << "The best three things are:\n";
for(int i: finder.reap()) {
  std::cout << " - " << i << '\n';
}

std::cout << "Aren't they amazing?\n";
```


## Why This is Cool

TODO!  Talk about KD trees and other pruning algorithms.


## The Finder Interface

**Note:** For simplicity, this section assumes that you've `typedef`ed
a `MaxFinder` with the template parameters you need as `MyFinder`. The
obvious reversals apply to `MinFinder`.

```C++
typedef Chaff::MaxFinder<Thing, float> MyFinder;
```

You'll exclusively deal with the specializations of the basic `Finder`
class: `MinFinder` and its obvious counterpart `MaxFinder`.  There are
three ways to create finders:

 - `MyFinder(int max_count, const S& min_score)`  
   This constructor is the most versatile method of creating a Finder.
   The resulting finder will collect at most `max_count`  `T`s  having
   scores strictly greater than `min_score`.

 - `MyFinder::byCount(int max_count)`  
   This creates a Finder that will collect the  (at most)  `max_count`
   `T`s with the highest scores - there is no minimum score.

 - `MyFinder::byScore(const S& min_score)`  
   This creates a  Finder  that will collect all  `T`s  having  scores
   greater than `min_score` regardless of how many there are.

Once you have a Finder, there are a few things you can do with it:

 - `void MyFinder::sow(const T& thing, const S& score)`  
   If `thing`  (with score `score`)  could possibly be a member of the
   result set, add it.

 - `std::vector<T> MyFinder::reap()`  
   Collect the results,  ordered by bestness.   This is a  destructive
   operation; afterward, the Finder is reset to its initial state.

 - `int MyFinder::count() const`  
   See how many results have been collected.

 - `const S& MyFinder::score() const`  
   Get the _worst_ score that could still be added to the result set.

 - `const T& MyFinder::thing() const`  
   Get the _worst_ thing that is part of the result set.  Note that in
   a Finder of size one, this also a way to get the _best_ thing.

 - `void MyFinder::clear()`  
   Reset the Finder to its initial state.
