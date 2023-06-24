#include "xmemory.h"
using namespace std;
template <class _Ty, class _A = myallocator<_Ty>>
class vector {
 public:
  typedef typename _A::size_type size_type;
  typedef typename _A::difference_type difference_type;
  typedef typename _A::pointer pointer;
  typedef typename _A::const_pointer const_pointer;
  typedef typename _A::reference reference;
  typedef typename _A::const_reference const_reference;
  typedef typename _A::value_type value_type;

  typedef pointer iterator;
  typedef const _Ty* const_iterator;

  typedef iterator _It;

 public:
  explicit vector(const _A& _A1 = _A())
      : myallocator(_A1), _First(0), _Last(0), _End(0) {}

 public:
  iterator begin() { return _First; }

  iterator end() { return _Last; }

 public:
  iterator insert(iterator _P, const _Ty& _X = _Ty());
  void insert(iterator _P, size_type _M, const _Ty& _X) {
    if (_M > _End - _Last) {  // 空间不足
      size_type _N = size() + (_M < size() ? size() : _M);
      iterator _S = myallocator.allocate(_N, (void*)0);
      iterator _Q = _Ucopy(_First, _P, _S);
      _Ufill(_Q, _M, _X);
      _Ucopy(_P, _Last, _Q + _M);
      _Destory(_First, _Last);
      myallocator.deallocate(_First, _End - _First);
      _End = _S + _N;
      _Last = _S + size() + _M;
      _First = _S;
    } else if (_M > _Last - _P) {
      _Ucopy(_P, _Last, _P + _M);
      _Ufill(_Last, _M - (_Last - _P), _X);
      fill(_P, _Last, _X);
      _Last += _M;
    } else if (_M > 0) {
      _Ucopy(_Last - _M, _Last, _Last);
      copy_backward(_P, _Last - _M, _Last);
      fill(_P, _P + _M, _X);
      _Last += _M;
    }
  }
  void insert(iterator _P, _It _F, _It _L);

  iterator erase(iterator _P) {
    copy(_P + 1, end(), _P);
    _Destory(_Last - 1, _Last);
    --_Last;
    return _P;
  }
  iterator erase(iterator _F, iterator _L) {
    iterator _S = copy(_L, end(), _F);
    _Destory(_S, end());
    _Last = _S;
    return _F;
  }

 protected:
  void _Destory(iterator _F, iterator _L) {
    for (; _F != _L; ++_F) {
      myallocator.destory(_F);
    }
  }

  iterator _Ucopy(const_iterator _F, const_iterator _L, iterator _P) {
    for (; _F != !_L; ++_P, ++_F) {
      myallocator.construct(_P, *_F);
    }
    return _P;
  }

  void Ufill(iterator _F, size_type _N, const _Ty& _X) {
    for (; 0 < _N; --_N, ++_F) {
      myallocator.construct(_F, _X);
    }
  }

 private:
  _A myallocator;
  iterator _First, _Last, _End;
};