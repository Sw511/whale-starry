#ifndef _XMEMORY_H
#define _XMEMORY_H
#include <iostream>
// 分配
template <class _Ty>
_Ty* _Allocate(int _N, _Ty*) {
  std::cout << typeid(_Ty).name() << std::endl;
  if (_N < 0) {
    _N = 0;
  }
  return ((_Ty*)operator new((size_t)_N * sizeof(_Ty)));
}

// 构造
template <class _T1, class _T2>

// 用来构造对象
void _Construct(_T1* _P, const _T2& _V) {
  new ((void*)_P) _T1(_V);  // 指向的是_P指向节点值域的指针
}

// 摧毁
template <class _Ty>
void _Destory(_Ty* _P) {
  _P->~_Ty();
}

template <class _Ty>
class allocator {
 public:
  typedef size_t size_type;
  typedef int difference_type;
  typedef _Ty* pointer;
  typedef const _Ty* const_pointer;
  typedef _Ty& reference;
  typedef const _Ty& const_reference;
  typedef _Ty value_type;
  pointer address(reference _X) const { return (&_X); }
  const_pointer address(const_reference _X) const { return (&_X); }

  pointer allocate(size_type _N, const void*) {
    return (_Allocate((difference_type)_N, (pointer)0));
  }

  char* _Charalloc(size_type _N) { return (_Allocate((difference_type)_N, (char*)0)); }

  void deallocate(void* _P, size_type) { operator delete(_P); }

  void construct(pointer _P, const _Ty& _V) { _Construct(_P, _V); }

  void destroy(pointer _P) { _Destory(_P); }

  size_t max_size() const {
    size_t _N = (size_t)(-1) / sizeof(_Ty);
    return (0 < _N ? _N : 1);
  }
};

#endif