#ifndef FPS_CLOCK_H
#define FPS_CLOCK_H

#include <SFML/System.hpp>

/// Clock to count the frames per second.
/// @note Inspired by https://gist.github.com/kenpower/7233967
class fps_clock
{
public:
  fps_clock();

  /// Get the frames per seconds
  double get_fps() const noexcept { return m_fps; }

  /// Indicate a frame has processed
  void tick() noexcept;

private:

  sf::Clock m_clock;

  double m_fps;
};

/// Test this class and its free functions
void test_fps_clock();

#endif // FPS_CLOCK_H
