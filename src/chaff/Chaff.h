#ifndef CHAFF_H
#define CHAFF_H

#include <limits>
#include <queue>

namespace Chaff
{
  template <class T, class S, class C = std::less<S> >
  class Finder {
  protected:
    struct Ranking {
    protected:
      T mThing;
      S mScore;
      C mCompare;

    public:
      Ranking() {}
      Ranking(const T& t, const S& s): mThing(t), mScore(s) {}

      const T& thing() const {return mThing;}
      const S& score() const {return mScore;}

      bool operator < (const Ranking& other) const {
        return mCompare(score(), other.score());
      }
    };

  public:
    typedef typename std::priority_queue<Ranking> Heap;
    typedef typename Heap::size_type Size;

  protected:
    Heap mHeap;
    Size mMaxCount;
    S    mTopScore;
    S    mMaxScore;
    C    mCompare;

  public:
    Finder(Size count, const S& score): mHeap() {
      mMaxCount = count;
      mTopScore = score;
      mMaxScore = score;
    }

    void clear() {
      mHeap     = Heap();
      mTopScore = mMaxScore;
    }

    Size count() const {
      return mHeap.size();
    }

    std::vector<T> reap() {
      std::vector<T> results(mHeap.size());

      Size i = mHeap.size();
      while(i > 0) {
        results[--i] = mHeap.top().thing();
        mHeap.pop();
      }

      clear();
      return results;
    }

    const S& score() const {
      return mTopScore;
    }

    void sow(const T& thing, const S& score) {
      if(mCompare(score, mTopScore)) {
        mHeap.push(Ranking(thing, score));
        if(mHeap.size() > mMaxCount) {
          mHeap.pop();
          mTopScore = mHeap.top().score();
        }
      }
    }

    const T& thing() const {
      return mHeap.top().thing();
    }
  };

  template <class T, class S>
  class MaxFinder: public Finder<T, S, std::greater<S> > {
  public:
    typedef typename Finder<T, S, std::greater<S> >::Size Size;
  public:
    static MaxFinder byCount(Size count)     {return MaxFinder(count, std::numeric_limits<S>::min());}
    static MaxFinder byScore(const S& score) {return MaxFinder(std::numeric_limits<Size>::max(), score);}
  public:
    MaxFinder(int count, const S& score): Finder<T, S, std::greater<S> >(count, score) {}
  };

  template <class T, class S>
  class MinFinder: public Finder<T, S, std::less<S> > {
  public:
    typedef typename Finder<T, S, std::greater<S> >::Size Size;
  public:
    static MinFinder byCount(Size count)     {return MinFinder(count, std::numeric_limits<S>::max());}
    static MinFinder byScore(const S& score) {return MinFinder(std::numeric_limits<Size>::max(), score);}
  public:
    MinFinder(int count, const S& score): Finder<T, S, std::less<S> >(count, score) {}
  };
}

#endif
