#ifndef __H_PATH_
#define __H_PATH_

/* #include "headers.h" */
#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>

using namespace std;

#define maxnum 17

/* namespace std { */
/*   template<> */
/*     void swap<bitset<maxnum>::reference>(bitset<maxnum>::reference &left, */
/* 				    bitset<maxnum>::reference &right) noexcept { */
/*     bool temp = std::move((bool)left); */
/*     left = std::move((bool)right); */
/*     right = std::move((bool)temp); */
/*   } */
/* } */

template<class T>
class Path {
 public:

 Path(std::size_t capacity = 512)
   : _capacity(capacity),
    _size(0),
    _head(0),
    _rear(0),
    checked(false),
    keep_lv_checked(false) {
    //        p = (struct Link *) malloc(_capacity * sizeof(struct Link));
    //    p = new Link[3];//cout << "after: " << p << endl;
    //    _size = 0;
  }

  Path(const Path &p) {
    _rear = p._rear;
    _head = p._head;
    _capacity = p._capacity;
    _size = p._size;
    checked = p.checked;
    keep_lv_checked = p.keep_lv_checked;
    /* path = vector<T>(_capacity); */
    for (auto &&node : p.path)
      path.push_back(node);
    /* for (int i = 0; i < _size; ++i) { */
    /*   path.push_back(p[i]); */
    /* } */
    nodes = p.nodes;
  }

  Path & operator = (Path other) {
    swap(other);

    return *this;
  }

  Path(Path &&p) {
    _rear = p._rear;
    _head = p._head;
    _capacity = p._capacity;
    _size = p._size;
    checked = p.checked;
    keep_lv_checked = p.keep_lv_checked;
    /* path = vector<T>(_capacity); */
    for (auto &&node : p.path)
      path.push_back(node);
    /* for (int i = 0; i < _size; ++i) { */
    /*   path.push_back(p[i]); */
    /* } */
    nodes = p.nodes;
    /* p.~Path();//clear(); */
  }

  Path(T *begin, T *end) {
    _capacity = end - begin;
    _size = _capacity;
    for (int i = 0; i < _size; ++i) {
      path.push_back(*(begin + i));
      nodes[*(begin + i)] = 1;
    }
    _head = path.front();
    _rear = path.back();
    checked = false;
    keep_lv_checked = false;
  }

  ~Path() noexcept {
    clear();
    path.~vector<T>();
    //    _capacity = 0;
    //    _size = 0;
    //    free(p);
    //    delete []p;
  }
  
  unsigned size() const {
    return _size;
  }

  unsigned capacity() const {
    return _capacity;
  }

  bool empty() const {
    return (_size == 0);
  }

  void resize(int newCapacity = 0) {
    if (newCapacity == 0) {
      newCapacity = 2 * _capacity;
    }
    assert(newCapacity >= _capacity);
    path.resize(newCapacity);
  }

  T at(const int i) {
    assert(i >= 0 && i < _size);
    return this->path[i];
  }

  inline T &operator [] (const int i) {
    //    static Link<T> link;
    assert(i >= 0 && i < _size);//assert(i < _capacity);
    //    if (i < _size) {
    return this->path[i];
    /*    }
	  else if (i >= _capacity) {
	  //      resize();
	  }
	  return link;*/
  }
  
  const T &operator [] (const int i) const {
    //    static Link<T> link;
    assert(i >= 0 && i < _size);
    //    if (i < _size) {
    return this->path[i];
    /*    }
	  else if (i >= _capacity) {
	  //resize();
	  }
	  return link;*/
  }

  void set_head(const T &head) {
    _head = head;
  }

  T head() const {
    return _head;
  }

  void set_rear(const T &rear) {
    _rear = rear;
  }

  T rear() const {
    return _rear;
  }

  T & front() {
    return this->path.front();
  }

  const T & front() const {
    return this->path.front();
  }

  T & back() {
    return this->path.back();
  }

  const T & back() const {
    return this->path.back();
  }

  void push_back(const T node) {
    assert(_size < _capacity);
    if (_size > _capacity) {
      resize();
    }
    /* int size = path.size(); */
    path.push_back(node);
    nodes[node] = 1;
    /* size = path.size(); */
    ++_size;
    _head = path.front();
    _rear = path.back();
  }

  /* void pop_back() { */
  /*   --_size; */
  /*   assert(_size >= 0); */
  /*   return; */
  /* } */

  void check() const {
    checked = true;
  }

  bool is_checked() const {
    return checked;
  }

  void clear() {
    vector<T>().swap(path);
    _size = 0;
    _capacity = 0;
    checked = false;
    /* keep_lv_checked = false; */
  }

  friend Path<T> operator + (const Path<T> &p1, const Path<T> &p2) {
    assert(p1.back() == p2.front());
    /* unsigned capacity = p1.capacity() + p2.capacity() - 1; */
    Path<T> res(p1);
    /* for (unsigned i = 0; i < p1.size(); ++i) { */
    /*   res.push_back(p1[i]); */
    /* } */
    auto bs = res.nodes & p2.nodes;
    bs[p1.back()] = 0;
    if (bs.any())
      return Path<T>();
    for (unsigned i = 1; i < p2.size(); ++i) {
      res.push_back(p2[i]);
    }
    res.set_head(p1._head);
    res.set_rear(p2._rear);
    res.keep_lv_checked = true;

    return res;
  }

  void swap(Path &other) {
    path.swap(other.path);
    std::swap(nodes, other.nodes);
    std::swap(_capacity, other._capacity);
    std::swap(_size, other._size);
    std::swap(_head, other._head);
    std::swap(_rear, other._rear);
    std::swap(checked, other.checked);
    std::swap(keep_lv_checked, other.keep_lv_checked);
  }

/*   Path operator = (const Path<T> &other) { */
/*     _capacity = other.capacity(); */
/*     _size = other.size(); */
/*     for (int i = 0; i < _size; ++i) { */
/*       path.push_back(other[i]); */
/*     } */
/*     _head = other.front(); */
/*     _rear = other.back(); */

/*     if (!keep_lv_checked) */
/*       checked = other.checked; */
/* keep_lv_checked = other */
/*     return *this; */
/*   } */

  /* void initialize() {//            srand((unsigned)time(NULL)); */
  /*   for (int i = 0; i < _capacity; ++i) { */
  /*     Link<T> link;// = createLink(); */
  /*     __ESBMC_assume(0 < link.ett && link.ett < 100 && link.chnl < 3); */
  /*     //      link.ett = rand()%100;//cout << link.ett << endl; */
  /*     //      link.chnl = rand()%3;//cout << link.chnl << endl;      cout << p+_size << endl; */
  /*     push_back(link); */
  /*     //      cout << _capacity << _size << endl; */
  /*     //cout << p+_size << endl; */
  /*     /\*	       p[i].ett = link.ett; */
  /* 		       p[i].chnl = link.chnl; */
  /* 		       ++_size;*\/ */
  /*   } */
  /* } */

 private:
  vector<T> path;
  std::bitset<maxnum> nodes;
  int _capacity;
  int _size;
  T _head;
  T _rear;
  mutable bool checked;
  bool keep_lv_checked;
};

#endif	/* __H_PATH_ */
