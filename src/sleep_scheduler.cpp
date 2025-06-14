#include "sleep_scheduler.h"

#include <cassert>

void sleep_scheduler::tick()
{
  sf::sleep(sf::microseconds(m_sleep_time_microseconds));

  const int fps{m_fps_clock.get_fps()};
  if (fps > 120)
  {
    m_sleep_time_microseconds *= 2;
  }
  else if (fps > 60)
  {
    ++m_sleep_time_microseconds;
  }
  else if (fps < 50 && m_sleep_time_microseconds > 1)
  {
    --m_sleep_time_microseconds;
  }

  m_fps_clock.tick();
}

void test_sleeper()
{
  sleep_scheduler s;
  const int fps_0{s.get_fps()};
  assert(fps_0 == 0);
  sf::sleep(sf::microseconds(100));
  s.tick();
  for (int i = 0; i < 100; ++i)
  {
    const int fps_1{s.get_fps()};
    assert(fps_1 > 0);
    sf::sleep(sf::microseconds(100));
    s.tick();
  }
  const int fps_2{s.get_fps()};
  sf::sleep(sf::microseconds(100));
  s.tick();
  const int fps_3{s.get_fps()};
  assert(fps_2 > 0);
  assert(fps_3 > 0);
}

