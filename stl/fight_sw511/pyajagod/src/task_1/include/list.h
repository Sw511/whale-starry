#include <assert.h>
#include "xmemory.h"
template <class _Ty, class _A = myallocator<_Ty>>
class list {
 public:
  struct _Node;
  typedef _Node* _Nodeptr;
  typedef size_t size_type;
  typedef _Ty value_type;
  typedef _Ty* pointer;
  typedef _Ty& reference;
  typedef const _Ty* const_pointer;
  typedef const _Ty& const_reference;
  typedef int difference_type;
  typedef list<_Ty, _A> _Myt;

  class iterator;
  class const_iterator;
  typedef const_iterator _It;

  // 常迭代器
  class const_iterator {
    // 比较是否相同，如何++，如何--，如何取值

   public:
    const_iterator() {}

    const_iterator(_Nodeptr _P) : _Ptr(_P) {}

    const_iterator(const_iterator& _X) : _Ptr(_X._Ptr) {}

    // 比较是否相同与不同
    bool operator==(const const_iterator& _X) const { return _Ptr == _X._Ptr; }

    bool operator!=(const const_iterator& _X) const {
      return !(*this == _X);  // 直接使用构造过的==
    }

    // 重写*
    const_reference operator*() const { return _Acc::_Value(_Ptr); }

    reference operator*() { return _Acc::_Value(_Ptr); }

    // 取_Ptr
    _Nodeptr _Mynode() const { return _Ptr; }

    // 重写++
    // 前++
    const_iterator operator++() {
      _Ptr = _Acc::_Next(_Ptr);
      return (*this);
    }
    // 后++
    const_iterator operator++(int) {
      const_iterator _Tmp = *this;
      ++*this;
      return (_Tmp);
    }
    // 重写--
    // 前--
    const_iterator operator--() {
      _Ptr = _Acc::_Prev(_Ptr);
      return (*this);
    }
    // 后--
    const_iterator operator--(int) {
      const_iterator _Tmp = *this;
      --*this;
      return (_Tmp);
    }

   protected:
    _Nodeptr _Ptr;
  };

  struct _Node {
    _Ty _Value;
    _Nodeptr _Next;
    _Nodeptr _Prev;
  };

  struct _Acc {
    typedef _Nodeptr& _Nodepref;
    typedef _Ty& _Vref;

    static _Nodepref _Next(_Nodeptr _P) {
      return ((_Nodepref)(*_P)._Next);  // _p->_next
    }
    static _Nodepref _Prev(_Nodeptr _P) {
      return ((_Nodepref)(*_P)._Prev);  // _p->_pref
    }
    static _Vref _Value(_Nodeptr _P) {
      return ((_Vref)(*_P)._Value);  // _p->_value
    }
  };

  // 迭代器
  class iterator : public const_iterator {
   public:
    iterator() {}
    iterator(_Nodeptr _P) : _Ptr(_P) {}
    // iterator(const_iterator& _X) : const_iterator(_X), _Ptr(_X._Mynode()) {}
    iterator(const iterator& _X) : _Ptr(_X._Ptr) {}

    _Nodeptr _Mynode() const { return _Ptr; }

    // 比较是否相同与不同
    bool operator==(const iterator& _X) const { return _Ptr == _X._Ptr; }
    bool operator!=(const iterator& _X) const {
      return !(*this == _X);  // 直接使用构造过的==
    }

    // 重写*
    const_reference operator*() const {
      // return _Ptr->_Value;
      return _Acc::_Value(_Ptr);
    }

    reference operator*() { return _Acc::_Value(_Ptr); }

    // 重写++
    // 前++
    iterator operator++() {
      this->_Ptr = _Acc::_Next(_Ptr);
      return (*this);
    }

    // 后++
    iterator operator++(int) {
      iterator _Tmp = *this;
      ++*this;
      return (_Tmp);
    }

    // 重写--
    // 前--
    iterator operator--() {
      this->_Ptr = _Acc::_Prev(_Ptr);
      return (*this);
    }

    // 后--
    iterator operator--(int) {
      iterator _Tmp = *this;
      --*this;
      return (_Tmp);

    }

   protected:
    _Nodeptr _Ptr;
  };

 public:
  explicit list() : _Head(_Buynode()), _Size() {}  // 对私有属性的赋值尽量写在外面
  explicit list(size_type _N, const _Ty& _V = _Ty()) : _Head(_Buynode()), _Size(0) {
    insert(begin(), _N, _V);
  }
  explicit list(const _Ty* _F, const _Ty* _L) : _Head(_Buynode()), _Size(0) {
    insert(begin(), _F, _L);
  }
  ~list() {
    erase(begin(), end());
    _FreeNode(_Head);
    _Head = 0;
    _Size = 0;
  }

 public:
  // 返回大小
  size_type size() const { return _Size; }

  // 判空
  bool empty() const { return size() == 0; }

  // 返回头
  // const_reference front() const { return (*begin()); }
  reference front() { return *begin(); }

  // 返回尾
  // const_reference_type back() const { return (*(--end())); }
  reference back() { return *--end(); }
  void clear() { erase(begin(), end()); }

 public:
  void splice(iterator _P, _Myt& _X) {
    if (!_X.empty()) {
      _Splice(_P, _X, _X.begin(), _X.end());
      _Size += _X._Size;
      _X._Size = 0;
    }
  }
  void remove(const _Ty& _V) {
    iterator _L = end();
    for (iterator _F = begin(); _F != _L;) {
      if (*_F == _V) {
        erase(_F++);
      } else {
        ++_F;
      }
    }
  }
  void unique() {
    iterator _F = begin(), _L = end();
    if (_F != _L) {
      for (iterator _M = _F; ++_M != _L; _M = _F) {
        if (*_F == *_M)
          erase(_M);
        else
          _F = _M;
      }
    }
  }
  void reverse() {
    if (size >= 2) {
      iterator _L = end();
      for (iterator _F = ++begin(); _F != _L;) {
        iterator _M = _F;
        _Splice(begin(), *this, _M, ++_F);
      }
    }
  }

  void merge(_Myt& _X);
  void sort();

 public:
  void assign(iterator _F, _It _L) {
    erase(begin(), end());
    insert(_F, _L);
  }
  void assign(size_type _N, const _Ty& _X = _Ty()) {
    erase(begin(), end());
    insert(begin(), _N, _X);
  }
  const_iterator begin() const { return const_iterator(_Acc::_Next(_Head)); }
  iterator begin() { return iterator(_Acc::_Next(_Head)); }
  iterator end() { return iterator(_Head); }
  const_iterator end() const { return const_iterator(_Head); }

  // 尾插
  // void push_back(const_reference_type v) {
  //   // _s->_Value = v;
  //   // _s->_Prev = _Head->_Prev;
  //   // _Head->_Prev->_Next = _s;
  //   // _s->_Next = _Head;
  //   // _Head->_Prev = _s;
  //   _Nodeptr _S = _Buynode(_Head, _Acc::_Prev(_Head));
  //   _S->_Value = v;
  //   _Acc::_Next(_Acc::_Prev(_S)) = _S;
  //   _Acc::_Prev(_Head) = _S;
  //   _Size++;
  // }
  void push_back(const _Ty& _X) { insert(end(), _X); }
  // 头插
  void push_front(const _Ty& _X) { insert(begin(), _X); }
  void pop_back() {
    // std::cout << "pop_back" << (end())._Mynode()->_Value << std::endl;
    erase(--end());
  }
  void pop_front() { erase(begin()); }

  void show() {
    _Nodeptr _P = _Head->_Next;
    while (_P != _Head) {
      std::cout << _P->_Value << "-->";
      _P = _P->_Next;
    }
    std::cout << "Over" << std::endl;
  }

  void show_iterator() {
    iterator _F = ++begin(), _L = end();
    std::cout << *_F << " == " << *_L << std::endl;
    int cc = 0;
    for (; _F != _L; ++_F) {
      if (cc > 10) {
        break;
      }
      std::cout << *_F << " ";
      cc++;
    }
  }

 public:
  // iterator insert(iterator _P, const _Ty& _X = _Ty()) {
  //   _Nodeptr _Ptr = _P._Mynode();
  //   _Nodeptr _S = _Buynode(_Ptr, _Acc::_Prev(_Ptr));
  //   _Acc::_Prev(_Ptr) = _S;
  //   _Acc::_Next(_Acc::_Prev(_S)) = _S;
  //   // _Acc::_Value(_X);
  //   allocator.construct(&_Acc::_Value(_S), _X);
  //   _Size++;
  //   return iterator(_S);
  // }
  iterator insert(iterator _P, const _Ty& _X = _Ty()) {
    _Nodeptr _S = _P._Mynode();
    _Acc::_Prev(_S) = _Buynode(_S, _Acc::_Prev(_S));
    _S = _Acc::_Prev(_S);
    _Acc::_Next(_Acc::_Prev(_S)) = _S;
    ////////////////////////////////////////
    // _S->_Value = _X;
    // _Acc::_Value(_S) = _X;
    // allocator.construct(&_S->_Value, _X);
    myallocator.construct(&_Acc::_Value(_S), _X);
    ///////////////////////////////////////

    _Size++;
    return iterator(_S);
  }
  // 多样性插入
  // 区间插入
  void insert(iterator _P, size_type _M, const _Ty& _X) {
    for (; 0 < _M; --_M) {
      insert(_P, _X);
    }
  }

  void insert(iterator _P, const _Ty* _F, const _Ty* _L) {
    for (; _F != _L; ++_F) {
      insert(_P, *_F);
    }
  }

  // 范围插入
  void insert(iterator _P, _It _F, _It _L) {
    for (; _F != _L; ++_F) {
      insert(_P, *_F);
    }
  }

  iterator erase(iterator _P) {
    _Nodeptr _S = (_P++)._Mynode();
    // std::cout << _S->_Value << " ===== " << std::endl;
    _Acc::_Next(_Acc::_Prev(_S)) = _Acc::_Next(_S);
    _Acc::_Prev(_Acc::_Next(_S)) = _Acc::_Prev(_S);
    ///////////////////////////////////
    // free(_S);  // 只释放节点，未释放空间
    // delete _S;
    // 释放节点前要析构对象
    myallocator.destroy(&_Acc::_Value(_S));
    _FreeNode(_S);
    //////////////////////////////////
    --_Size;
    return _P;
  }

  // 区间删除
  iterator erase(iterator _F, iterator _L) {
    while (_F != _L) {
      erase(_F++);
    }
    return _F;
  }

 protected:
  // _Narg后继参数 _Parg前继参数
  _Nodeptr _Buynode(_Nodeptr _Narg = 0, _Nodeptr _Parg = 0) {
    // 不使用适配器
    // _Nodeptr _S = (_Nodeptr)malloc(sizeof(_Node));
    // 使用适配器
    _Nodeptr _S = (_Nodeptr)myallocator._Charalloc(sizeof(_Node));
    // _Nodeptr _S = (_Nodeptr)allocator.allocate(1, 0);
    assert(_S != NULL);
    _Acc::_Next(_S) = _Narg != 0 ? _Narg : _S;
    _Acc::_Prev(_S) = _Parg != 0 ? _Parg : _S;
    return _S;
  }
  void _FreeNode(_Nodeptr _S) {
    myallocator.deallocate(_S, 1);  // operator delete(_S)
  }

  void _Splice(iterator _P, _Myt& _X, iterator _F, iterator _L) {
    if (myallocator == _X.myallocator) {
      _Acc::_Next(_Acc::_Prev(_L._Mynode())) = _P._Mynode();
      _Acc::_Next(_Acc::_Prev(_F._Mynode())) = _L._Mynode();
      _Acc::_Next(_Acc::_Prev(_P._Mynode())) = _F._Mynode();
      _Nodeptr _S = _Acc::_Prev(_P._Mynode());
      _Acc::_Prev(_P._Mynode()) = _Acc::_Prev(_L._Mynode());
      _Acc::_Prev(_L._Mynode()) = _Acc::_Prev(_F._Mynode());
      _Acc::_Prev(_F._Mynode()) = _S;
    } else {
      insert(_P, _F, _L);
      _X.erase(_F, _L);
    }
  }

 private:
  // 空间适配器
  _A myallocator;
  _Nodeptr _Head;
  size_type _Size;
};
