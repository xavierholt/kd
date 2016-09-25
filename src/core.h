#ifndef KD_CORE_H
#define KD_CORE_H

namespace KD
{
  template <int DIM, class DT, class PT, class CT, int STO, int DEP>
  struct CoreBase
  {
    typedef DT Item;
    typedef PT Point;
    typedef CT Coord;

    static const int DIMENSIONS = DIM;
    static const int MAX_DEPTH  = DEP;
    static const int STORAGE    = STO;

    static CT coordinate(const PT& point, int axis) {
      return point[axis];
    }
  };

  template <int DIM, class DT, class PT = DT, class CT = double, int STO = 8, int DEP = 32>
  struct Core : public CoreBase<DIM, DT, PT, CT, STO, DEP>
  {
    static const PT& point(const DT& data) {
      return data.point();
    }
  };

  template <int DIM, class DT, class PT, class CT, int STO, int DEP>
  struct Core<DIM, DT*, PT, CT, STO, DEP> : public CoreBase<DIM, DT*, PT, CT, STO, DEP>
  {
    static const PT& point(const DT* data) {
      return data->point();
    }
  };

  template <int DIM, class T, class CT, int STO, int DEP>
  struct Core<DIM, T, T, CT, STO, DEP> : public CoreBase<DIM, T, T, CT, STO, DEP>
  {
    static const T& point(const T& data) {
      return data;
    }
  };

  template <int DIM, class T, class CT, int STO, int DEP>
  struct Core<DIM, T*, T, CT, STO, DEP> : public CoreBase<DIM, T*, T, CT, STO, DEP>
  {
    static const T& point(const T* data) {
      return *data;
    }
  };
}

#endif
