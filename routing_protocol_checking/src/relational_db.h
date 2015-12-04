#ifndef __H_RELATIONAL_DB_
#define __H_RELATIONAL_DB_

#include <vector>
#include <set>
#include <cstring>
#include <string>
/* #include <string.h> */
#include <map>
/* #include <unordered_map> */
#include <iostream>
#include <algorithm>
#include <pthread.h>

/* #include <unordered_set> */
#include "strop.h"
#include "gripp.hh"

#include <functional>

using namespace std;


/* #define LEFT_MONOTONICITY 0 */
/* #define RIGHT_MONOTONICITY 1 */
/* #define LEFT_ISOTONICITY 0 */
/* #define RIGHT_ISOTONICITY 1 */
/* #define ODD_SYMMETRY 0 */
/* #define TRANSITIVITY 0 */
/* #define STRICT_ORDER 0 */
/* #define SOURCE_INDEPENDENCE 0 */
/* #define LOCAL_MINIMUM_FREE 0 */

class rSets {
 public:
 rSets() : zero_indegree(true) {}
 /* rSets() : zero_indegree(true), pre(0), post(0) {} */

 rSets(const rSets &rsets) : greaterThanMe(rsets.greaterThanMe), zero_indegree(rsets.zero_indegree) {}
 /* rSets(const rSets &rsets) : greaterThanMe(rsets.greaterThanMe), zero_indegree(rsets.zero_indegree), pre(rsets.pre), post(rsets.post) {} */

  virtual ~rSets() noexcept {
    clear();
  }

  void
    clear() {
    std::vector<String>().swap(greaterThanMe);
    zero_indegree = true;
    /* pre = 0; */
    /* post = 0; */
  }

  rSets & 
    operator = (rSets rhs) {
    swap(rhs);
    
    return *this;
  }

 rSets(rSets &&rsets) : greaterThanMe(rsets.greaterThanMe), zero_indegree(rsets.zero_indegree) {}
 /* rSets(rSets &&rsets) : greaterThanMe(rsets.greaterThanMe), zero_indegree(rsets.zero_indegree), pre(rsets.pre), post(rsets.post) {} */

  const std::vector<String> & 
    getGreaterThanMe() const {
    return greaterThanMe;
  }

  std::vector<String> & 
    getGreaterThanMe() {
    return greaterThanMe;
  }

  void 
    setGreaterThanMe(const std::vector<String> &gtm) {
    greaterThanMe = gtm;
  }

  void 
    setGreaterThanMe(std::vector<String> &&gtm) {
    greaterThanMe.swap(gtm);
  }

  void 
    printGreaterThanMe() const {
    /* pthread_mutex_lock(&lock); */
    for (auto &&str : greaterThanMe)
      std::cout << str << " ";
    //    std::cout << greaterThanMe.size() << std::endl;
    /* pthread_mutex_unlock(&lock); */
  }

  void setZeroIndegreeFalse() {
    zero_indegree = false;
  }

  bool getZeroIndegree() const {
    return zero_indegree;
  }

  /* void setPre(const std::size_t &pre) { */
  /*   this->pre = pre; */
  /* } */

  /* std::size_t getPre() const { */
  /*   return pre; */
  /* } */

  /* void setPost(const std::size_t &post) { */
  /*   this->post = post; */
  /* } */

  /* std::size_t getPost() const { */
  /*   return post; */
  /* } */

 private:
  void 
    swap(rSets &other) {
    other.greaterThanMe.swap(greaterThanMe);
    /* other.keyInfo.swap(keyInfo); */
  }

  std::vector<String> greaterThanMe;
  /* vector<string> keyInfo; */
  bool zero_indegree;
  /* std::size_t pre; */
  /* std::size_t post; */
  /* pthread_mutex_t lock; */
};

class rDB {
 public:
 rDB() : size(0), MFF(false) {}

  void 
    swap(rDB &other) {
    other.db.swap(db);
    /* other.index_table.swap(index_table); */
    other.root_children.swap(root_children);
    other.valid.swap(valid);
    size = move(other.size);
    MFF = other.MFF;
    visited = other.visited;
  }

  /* rDB(const rDB &rdb) : db(rdb.db) {} */
  rDB(const rDB &rdb) {
    db = rdb.db;
    /* index_table = rdb.index_table; */
    root_children = rdb.root_children;
    size = rdb.size;
    MFF = rdb.MFF;
    visited = rdb.visited;
  }

  rDB & 
    operator = (rDB rdb) {
    swap(rdb);

    return *this;
  }

 rDB(rDB &&rdb) : db(rdb.db), root_children(rdb.root_children), size(rdb.size), MFF(rdb.MFF), visited(rdb.visited) {}

  virtual ~rDB() noexcept {
    clear();
    /* pthread_mutex_destroy(&lock); */
  }

  void 
    clear() {
    map<String, rSets>().swap(db);
    /* clear_index_info(); */
    std::set<String>().swap(root_children);
    vector<vector<int> >().swap(valid);
    size = 0;
    MFF = false;
    visited.reset();
  }

  int 
    insert(const String &str1, const String &str2, const int &relation) {
    if (2 == simple_insert(str1, str2, relation))
      return 2;
    if (str1.size() > 2)
      if (2 == adjust(str1))
	return 2;
    if (str2.size() > 2)
      if (2 == adjust(str2))
	return 2;
    return 0;
  }

  int 
    insert(String &&str1, String &&str2, int &&relation) {
    return insert(static_cast<const String &>(str1),
		  static_cast<const String &>(str2),
		  static_cast<const int &>(relation));
  }

  int query(const String &str1, const String &str2) {
    //    pthread_mutex_lock(&lock);
    /* cout << str1 << " - " << str2 << endl; */
    if (db.find(str1) == db.end()) {
      /* keyRegister(str1); */
      /* cout << keyInfo[str1].size() << endl; */
      if (str1.size() > 2) {
	if (2 == adjust(str1))
	  return 2;
      }
    }
    if (db.find(str2) == db.end()) {
      /* keyRegister(str2); */
      /* cout << keyInfo[str2].size() << endl; */
      if (str2.size() > 2) {
	if (2 == adjust(str2))
	  return 2;
      }
    }

    /* if (db[str1].getKeyInfo().size() > 2 */
    /* 	&& db[str2].getKeyInfo().size() > 2) */
    /*   adjust(str1, str2); */

    /* int res = simple_query(str1, str2); */
    return simple_query(str1, str2);
    //    pthread_mutex_unlock(&lock);

    /* return res; */
  }

  int 
    query(String &&str1, String &&str2) {
    return query(static_cast<const String &>(str1),
		 static_cast<const String &>(str2));
  }

  int 
    merge(const rDB &other) {
    rDB temp(*this);		/* roll back in case any exception is thrown */
    map<String, rSets>::const_iterator db_iter;
    for (db_iter = other.db.begin(); db_iter != other.db.end(); ++db_iter) {
      for (auto &&item : db_iter->second.getGreaterThanMe()) {
	auto res(temp.query(item, db_iter->first));
	if (0 == res)
	  if (2 == temp.insert(item, db_iter->first, 1))
	    return 2;
	  else
	    continue;
	else if (-1 == res) {
	  return -1;
	}
	else if (2 == res)
	  return 2;
      }
    }
    swap(temp);
    return 0;
  }

  int 
    merge(rDB &&other) {
    return merge(static_cast<const rDB &>(other));
  }

  int 
    force_merge(const rDB &other) {
    rDB temp(*this);
    int retVal(0);
    map<String, rSets>::const_iterator db_iter;
    for (db_iter = other.db.begin(); db_iter != other.db.end(); ++db_iter) {
      for (auto &&item : db_iter->second.getGreaterThanMe()) {
	auto res(temp.query(item, db_iter->first));
	if (0 == res)
	  if (2 == temp.insert(item, db_iter->first, 1))
	    return 2;
	  else
	    continue;
	else if (-1 == res) {//cout << *set_iter << " " << str << endl;
	  /* cout << "conflict!\n"; */
	  retVal = -1;
	}
	else if (2 == res)
	  return 2;
      }
    }
    swap(temp);
    return retVal;
  }

  int 
    force_merge(rDB &&other) {
    return force_merge(static_cast<const rDB &>(other));
  }

  void 
    print() {
    static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&lock);
    map<String, rSets>::iterator iter;
    for (iter = db.begin(); iter != db.end(); ++iter) {
      cout << iter->first << ":\n";
      cout << "greaterThanMe: ";
      for (auto &&str : db[iter->first].getGreaterThanMe())
	cout << str << " ";
      cout << endl;
    }
    pthread_mutex_unlock(&lock);
  }

  bitset<maxnum> is_visited(const int &idx) const {
    return visited[idx];
  }

  bool is_all_visited() const {
    return MFF ? visited.none() : visited.all();
  }

  void visit(const int &idx) {
    visited[idx] = !MFF;
  }

  bool isMFF() const {
    return MFF;
  }

  void flipMFF() {
    MFF = !MFF;
  }

 private:

  /* void */
  /*   clear_index_info() { */
  /*   std::map<String, index_t>().swap(index_table); */
  /* } */

  /* void */
  /*   compute_index() { */
  /*   /\* clear index_table first *\/ */
  /*   std::size_t pre_post(0); */
  /*   std::set<String> seen; */
  /*   for (auto &&next_node : root_children) { */
  /*     auto pre_node(pre_post); */
  /*     ++pre_post; */
  /*     traverse(next_node, pre_post, seen); */
  /*     auto it(db.find(next_node)); */
  /*     /\* if (it != db.end()) { *\/ */
  /* 	it->second.setPre(pre_node); */
  /* 	it->second.setPost(pre_post); */
  /*     /\* } *\/ */
  /*     ++pre_post; */
  /*   } */
  /* } */

  /* void */
  /*   traverse(const String &next_node, */
  /* 	     std::size_t &pre_post, */
  /* 	     std::set<String> &seen) { */
  /*   seen.insert(next_node); */
  /*   for (auto &&child : db[next_node].getGreaterThanMe()) { */
  /*     auto pre_node(pre_post); */
  /*     ++pre_post; */
  /*     if (seen.find(child) == seen.end()) */
  /* 	traverse(child, pre_post, seen); */
  /*     auto it(db.find(child)); */
  /*     /\* if (it != db.end()) { *\/ */
  /* 	it->second.setPre(pre_node); */
  /* 	it->second.setPost(pre_post); */
  /*     /\* } *\/ */
  /*     /\* index_table.emplace(std::make_pair(child, (index_t){pre_node, pre_post})); *\/ */
  /*     ++pre_post; */
  /*   } */
  /* } */

  /* bool */
  /*   reachability(const String &query, const String &target) { */
  /*   auto it_query(db.find(query)); */
  /*   auto it_target(db.find(target)); */
  /*   if (it_query != db.end() */
  /* 	&& it_target != db.end() */
  /* 	&& it_query->second.getPre() <= it_target->second.getPre() */
  /* 	&& it_target->second.getPre() <= it_query->second.getPost()) */
  /*     return true; */
  /*   return false; */
  /* } */
  
  bool 
    DFS(const String &src, const String &dest) {
    for (auto &&item : db[src].getGreaterThanMe()) {
      if (dest == item)
	return true;
      else if (DFS(item, dest))
	return true;
    }
    return false;
  }

  void 
    collectLessThanMe(std::set<String> &ltm,
		      const std::set<String> &gtm,
		      const String &target,
		      const String &suffix) {
    for (auto &&pair : db) {
      if (pair.first == target)
    	continue;
      auto tmp(pair.first);
      if (tmp.front() != target.front()
      	  || tmp.back() != target.back())
      	continue;
      if (gtm.find(pair.first) != gtm.end())
    	continue;
      String intersection;
      sort(tmp.begin() + 1, tmp.end() - 1);
      set_intersection(tmp.begin() + 1, tmp.end() - 1,
      		       suffix.begin(), suffix.end(),
      		       inserter(intersection, intersection.begin()));
      if (0 != intersection.size())
    	continue;
      if (-1 == query(pair.first, target))
    	ltm.insert(pair.first);
    }
  }

  void 
    collectGreaterThanMe(std::set<String> &gtm, const String &str) {
    for (auto &&item : db[str].getGreaterThanMe()) {
      gtm.insert(item);
      collectGreaterThanMe(gtm, item);
    }
  }

  /* void  */
  /*   keyRegister(const string &str) { */
  /*   /\* if (keyInfo.find(str) == keyInfo.end()) *\/ */
  /*   rSets &rsets(db[str]); */
  /*   if (rsets.getKeyInfo().empty()) */
  /*     rsets.setKeyInfo(str);//[str] = split(str); */
  /* } */

  int
    simple_insert(const String &str1, const String &str2, const int &relation) {
    if (str1 == str2)
      return 2;
    /* keyRegister(str1); */
    /* keyRegister(str2); */
    auto res(simple_query(str1, str2));

    if (1 == relation) {
      if (-1 == res) {
	cout << "serious conflict!\n";
	cout << str1 << " " << str2 << " " << res << endl;
	return 2;
      }
      else if (std::find(db[str2].getGreaterThanMe().begin(), db[str2].getGreaterThanMe().end(), str1) == db[str2].getGreaterThanMe().end()) {
	db[str2].getGreaterThanMe().push_back(str1);
	db[str1].setZeroIndegreeFalse();
	root_children.erase(str1);
	if (db[str2].getZeroIndegree())
	  root_children.insert(str2);
	valid[str2.front()][str2.back()] = -1;
      }
    }
    if (-1 == relation) {
      if (1 == res) {
	cout << "serious conflict!\n";
	cout << str1 << " " << str2 << " " << res << endl;
	return 2;
      }
      else if (std::find(db[str1].getGreaterThanMe().begin(), db[str1].getGreaterThanMe().end(), str2) == db[str1].getGreaterThanMe().end()) {
	db[str1].getGreaterThanMe().push_back(str2);
	db[str2].setZeroIndegreeFalse();
	root_children.erase(str2);
	if (db[str1].getZeroIndegree())
	  root_children.insert(str1);
	valid[str1.front()][str1.back()] = -1;
      }
    }
    /* if (db.find(str1) == db.end() */
    /* 	|| db.find(str2) == db.end()) { */
    /*   /\* std::map<String, index_t>().swap(index_table); *\/ */
    /*   compute_index(); */
    /* } */
    return 0;
  }

  int 
    adjust(const String &str) {//, const String &other = string()) {
    /* vector<string> &v_str(db[str].getKeyInfo()); */
    int i, j, size = str.size();
    String str1, str2;
    int res;

#ifdef LEFT_MONOTONICITY
/* #if (LEFT_MONOTONICITY==1) */
    for (i = 0; i < size - 2; ++i)
      for (j = i + 1; j < size - 1; ++j) {
	str1 = assemble(str, i, size);
	str2 = assemble(str, j, size);
	res = query(str1, str2);
	if (0 == res)
	  if (2 == simple_insert(str1, str2, 1))
	    return 2;
	if (2 == res)
	  return 2;
      }
#endif	/* left-monotonicity adjustment */

#ifdef RIGHT_MONOTONICITY
/* #if (RIGHT_MONOTONICITY==1) */
    for (i = 2; i < size; ++i)
      for (j = i + 1; j <= size; ++j) {
	str1 = assemble(str, 0, i);
	str2 = assemble(str, 0, j);
	res = simple_query(str1, str2);
	if (0 == res)
	  if (2 == simple_insert(str1, str2, -1))
	    return 2;
	if (2 == res)
	  return 2;
      }
#endif	/* right-monotonicity adjustment */

#ifdef LEFT_ISOTONICITY
/* #if (LEFT_ISOTONICITY==1) */
    if (size < 3)
      return 0;

    for (i = 1; i < size - 1; ++i) {
      auto target(assign(str, i, size));
      auto prefix(assign(str, 0, i));
      auto &gtm(db[target].getGreaterThanMe());
      set<String>::iterator it;
      String tmp;
      for (it = gtm.begin(); it != gtm.end(); ++it) {
	tmp = *it;
	/* vector<string> v_tmp(split(tmp)); */
	  
	if ((tmp.front() == target.front()) 
	    && (tmp.back() == v_target.back())) {
	  String intersection;
	  sort(tmp.begin() + 1, tmp.end() - 1);
	  sort(prefix.begin(), prefix.end());
	  set_intersection(tmp.begin() + 1, tmp.end() - 1,
			   prefix.begin(), prefix.end(),
			   inserter(intersection, intersection.begin()));

	  if (0 == intersection.size()) {
	    String str1(prefix);
	    concat(str1, tmp);
	    /* str1 += tmp; */
	    res = query(str1, str);
	    if (0 == res)
	      if (2 == simple_insert(str1, str, 1))
		return 2;
	    if (2 == res)
	      return 2;
	  }
	}
      }
    }
#endif	/* left-isotonicity adjustment */

#ifdef RIGHT_ISOTONICITY
/* #if (RIGHT_ISOTONICITY==1) */
    if (size < 3)
      return 0;

    for (i = size - 2; i > 0; --i) {
      auto target(assign(str, 0, i + 1));
      auto suffix(assign(str, i + 1, size));

      set<String> ltm;
      set<String> gtm;//(db[target].getGreaterThanMe());
      collectGreaterThanMe(gtm, target);
      collectLessThanMe(ltm, gtm, target, suffix); /* not all but correct */

      for (auto &&item : ltm) {
	String str1(item);
	concat(str1, suffix);
	/* str1 += suffix; */
	res = query(str1, str);
	if (0 == res)
	  if (2 == simple_insert(str1, str, -1))
	    return 2;
	if (2 == res)
	  return 2;
      }

      auto tmp_suffix(suffix);
      sort(tmp_suffix.begin(), tmp_suffix.end());

      /* for (auto &&item : gtm) { */
      /* 	String tmp(item); */
      /* 	if ((tmp.front() == target.front())  */
      /* 	    && (tmp.back() == target.back())) { */
      /* 	  String intersection; */
      /* 	  sort(tmp.begin() + 1, tmp.end() - 1); */
      /* 	  /\* sort(v_suffix.begin(), v_suffix.end()); *\/ */
      /* 	  set_intersection(tmp.begin() + 1, tmp.end() - 1, */
      /* 			   suffix.begin(), suffix.end(), */
      /* 			   inserter(intersection, intersection.begin())); */

      /* 	  if (0 == intersection.size()) { */
      /* 	    String str1(tmp); */
      /* 	    concat(tmp, suffix); */
      /* 	    if (2 == simple_insert(str1, str, 1)) */
      /* 	      return 2; */
      /* 	  } */
      /* 	} */
      /* } */

      for (auto &&item : gtm) {
	String tmp(item);
	if ((tmp.front() == target.front()) 
	    && (tmp.back() == target.back())) {
	  String intersection;
	  sort(tmp.begin() + 1, tmp.end() - 1);
	  /* sort(v_suffix.begin(), v_suffix.end()); */
	  set_intersection(tmp.begin() + 1, tmp.end() - 1,
			   tmp_suffix.begin(), tmp_suffix.end(),
			   inserter(intersection, intersection.begin()));

	  if (0 == intersection.size()) {
	    String str1(tmp);
	    concat(str1, suffix);
	    if (2 == simple_insert(str1, str, 1))
	      return 2;
	  }
	}
      }
    }
    /* } */
#endif	/* right-isotonicity adjustment */

#ifdef ODD_SYMMETRY
    if (valid[str.front()][str.back()] * valid[str.back()][str.front()] > 0)
      return 2;
    valid[str.front()][str.back()] = -1;
    valid[str.back()][str.front()] = 1;
#endif	/* ODD_SYMMETRY */

#ifdef TRANSITIVITY
    if (-1 != valid[str.front()][str.back()])
      return 2;
    for (int i(1); i <= size; ++i) {
      if (-1 == valid[i][str.front()]) {
	if (-1 != valid[i][str.back()])
	  return 2;
	valid[i][str.back()] = -1;
      }
    }
#endif	/* TRANSITIVITY */

#ifdef LOCAL_MINIMUM_FREE
    if (-1 == valid[str.back()][str.front()])
      return 2;
#endif	/* LOCAL_MINIMUM_FREE */
    return 0;
  }

  int 
    simple_query(const String &str1, const String &str2) {
    /* keyRegister(str1); */
    /* keyRegister(str2); */
    /* auto res(reachability(str2, str1)); */
    /* if (res) */
    /*   return 1; */
    /* res = reachability(str1, str2); */
    /* if (res) */
    /*   return -1; */
    auto res1 = DFS(str2, str1);
    auto res2 = DFS(str1, str2);

#ifdef STRICT_ORDER
    if (st1.front() == str2.front() && res1 * res2 > 0) {
      cout << "Strict order property is violated!" << endl;
      rdb.print();
      return 2;
    }
#endif	/* STRICT_ORDER */


#ifdef SOURCE_INDEPENDENCE
    if (str1.front() == str1.front() && str1.back() != str2.back()) {
      cout << "Source independence property is violated!" << endl;
      rdb.print();
      return 2;
    }
#endif	/* SOURCE_INDEPENDENCE */

    if (res1)
      return 1;
    if (res2)
      return -1;
    return 0;
  }

  std::tuple<std::size_t, std::size_t, int>
    find_backward(const String &str1, const String &str2) {
    auto i1(str1.size() - 1), i2(str2.size() - 1);
    if (str1[i1] != str2[i2])
      return std::make_tuple(0, 0, 0);
    if (str1[i1 - 1] != str2[i2 - 1])
      return std::make_tuple(0, 0, 0);
    for (; i1 > 0 && i2 > 0; --i1, --i2) {
      auto res(simple_query(assemble(str1, 0, i1), assemble(str2, 0, i2)));
      switch (res) {
      case 0:
	continue;
      case 2:
	return std::make_tuple(0, 0, 2);
      default:
	return std::make_tuple(i1, i2, res);
      }
    }
    return std::make_tuple(0, 0, 0);
  }

  std::tuple<std::size_t, std::size_t, int>
    find_forward(const String &str1, const String &str2) {
    ;
  }

  String 
    assemble(const String &str, const int &head, const int &rear) {
    /* static pthread_mutex_t lock = PTHREAD_MUTEX_INITIAIZER; */
    /* pthread_mutex_lock(&lock); */

    String ret(str.begin() + head, str.begin() + rear);
    /* string res(""); */
    /* int i(0); */
    /* for (i = head; i < rear - 1; ++i) { */
    /*   res.append(v_str[i]); */
    /*   res.append(separator); */
    /* } */
    /* res.append(v_str[i]); */

    /* pthread_mutex_unlock(&lock); */

    return ret;
  }

  String 
    assign(const String &str, const int &head, const int &rear) {
    String ret;
    for (int i = head; i != /* < */ rear; ++i)
      ret.push_back(str[i]);
    return ret;
  }

  /* bool  */
  /*   loop_free(const String &str) { */
  /*   set<String> unique_set;// = set<string>(); */
  /*   for (unsigned i = 0; i < str.size(); ++i) { */
  /*     if (unique_set.count(str[i]) == 0) */
  /* 	unique_set.insert(str[i]); */
  /*     else { */
  /* 	cout << str << endl; */
  /* 	return false; */
  /*     } */
  /*   } */
  /*   return true; */
  /* } */

  std::map<String, rSets> db;
  /* std::map<String, index_t> index_table; */
  std::set<String> root_children;//, bool(*)(const std::string &, const std::string &)> root_children(ris_greater_size);
  bool MFF;
  bitset<maxnum> visited;
  /* pthread_mutex_t lock; */
};

#endif	/* __H_RELATIONAL_DB_ */
