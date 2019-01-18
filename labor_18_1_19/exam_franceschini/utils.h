#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <time.h>
#include <cstdint>

namespace timeutils {

  int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p);
  double getTimeS();

}

#endif