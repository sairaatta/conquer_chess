#ifndef SLEEP_SCHEDULER_H
#define SLEEP_SCHEDULER_H

#include "fps_clock.h"

/// Sleeps enough to reach 50-60 frames per second.
class sleep_scheduler
{
public:

  /// Get the number of frames per second
  double get_fps() const noexcept { return m_fps_clock.get_fps(); }

  /// Sets the target frame rate
  void set_target_fps(double fps);

  /// Indicate a frame has passed.
  /// This class will sleep here and will tune itself
  /// to sleep as much to achieve 50 frames per second
  void tick();

private:

  /// The clock that keeps track of the frames per second
  fps_clock m_fps_clock;

  /// The time this class sleeps in a tick
  double m_sleep_time_microseconds{1.0};

  /// The target frame rate
  double m_target_fps{60.0};
};

/// Test this class
void test_sleep_scheduler();

#endif // SLEEP_SCHEDULER_H
