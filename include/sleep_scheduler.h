#ifndef SLEEP_SCHEDULER_H
#define SLEEP_SCHEDULER_H

#include "fps_clock.h"

/// Sleeps enough to reach 50-60 frames per second.
class sleep_scheduler
{
public:

  /// Get the number of frames per second
  int get_fps() const noexcept { return m_fps_clock.get_fps(); }

  /// Indicate a frame has passed.
  /// This class will sleep here and will tune itself
  /// to sleep as much to achieve 50 frames per second
  void tick();

private:

  /// The clock that keeps track of the frames per second
  fps_clock m_fps_clock;

  /// The time this class sleeps in a tick
  int m_sleep_time_microseconds{1};
};

/// Test this class
void test_sleep_scheduler();

#endif // SLEEP_SCHEDULER_H
