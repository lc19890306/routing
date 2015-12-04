#ifndef __HH_ADDRESS_BOOK_
#define __HH_ADDRESS_BOOK_

#include <set>
#include <pthread.h>
#include <memory>

class AddressBook {
public:
  // static std::set<void *> addresses;
  static std::set<std::shared_ptr<void> > addresses;
  static pthread_mutex_t lock;
};

#endif	// __HH_ADDRESS_BOOK_
