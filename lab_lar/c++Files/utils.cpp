#include "utils.h"

namespace timeutils {

  int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
  {
    return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
            ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
  }

  double getTimeS()
  {
    static struct timespec t0;
    static int init = 0;
    if (!init)
    {
      clock_gettime(CLOCK_REALTIME, &t0);
      init = 1;
    }
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    int64_t timediff = timespecDiff(&spec, &t0);
    return timediff/1e9;
//    return ((spec.tv_sec * 1000000000) + spec.tv_nsec)/1e9;
  }

}
