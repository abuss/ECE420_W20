/*
// Copyright (c) 2000-2009, Texas Engineering Experiment Station (TEES), a
// component of the Texas A&M University System.

// All rights reserved.

// The information and source code contained herein is the exclusive
// property of TEES and may not be disclosed, examined or reproduced
// in whole or in part without explicit written authorization from TEES.
*/

#include <time.h>

class clock_gettime_timer
{
private:
  struct timespec m_ts;

public:
  /*!
    \brief Starts the counter.
   */
  void start(void)
  {
    if (clock_gettime(CLOCK_REALTIME, &m_ts) != 0)
      printf("clock_gettime_timer failed\n");
  }

  double stop(void) const
  {
    struct timespec nt;
    if (clock_gettime(CLOCK_REALTIME, &nt) != 0)
      printf("clock_gettime_timer failedi\n");
    return (double(nt.tv_sec-m_ts.tv_sec) +
            double(nt.tv_nsec-m_ts.tv_nsec)/1.0E9);
  }

};
