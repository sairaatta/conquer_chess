#include "sleep_scheduler.h"

#include <cassert>

void sleep_scheduler::set_target_fps(double fps)
{
  assert(fps > 0.0);
  m_target_fps = fps;
}

void sleep_scheduler::tick()
{
  sf::sleep(sf::microseconds(1 + static_cast<int>(m_sleep_time_microseconds)));

  const double fps{m_fps_clock.get_fps()};
  const double relative_frame_rate{fps / m_target_fps};

  // Dampen the change in sleep time
  const double sleep_increase{
    (relative_frame_rate + 1.0) / 2.0
  };
  m_sleep_time_microseconds *= sleep_increase;

  m_fps_clock.tick();
}

void test_sleep_scheduler()
{
  sleep_scheduler s;
  const double target_fps{69.0};
  s.set_target_fps(target_fps);
  const double fps_0{s.get_fps()};
  assert(fps_0 == 0);
  sf::sleep(sf::microseconds(100));
  s.tick();
  for (int i = 0; i < 20; ++i)
  {
    const double fps_1{s.get_fps()};
    assert(fps_1 > 0);
    sf::sleep(sf::microseconds(100));
    s.tick();
  }
  const double fps_2{s.get_fps()};
  assert(fps_2 > target_fps * 0.9);
  assert(fps_2 < target_fps * 1.1);
}

