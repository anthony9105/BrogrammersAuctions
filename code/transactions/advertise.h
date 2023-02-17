#ifndef ADVERTISE_H
#define ADVERTISE_H

#include <string>
#include "../user/user.h"
#include "transaction.h"

class AdvertiseTransaction : public Transaction {
public:
  void execute();
};

#endif
