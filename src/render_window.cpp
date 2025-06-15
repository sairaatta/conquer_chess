#include "render_window.h"

#include "screen_coordinate.h"

sf::RenderWindow& get_render_window() noexcept {
  static sf::RenderWindow window{
    sf::VideoMode(
      get_default_screen_size().get_x(),
      get_default_screen_size().get_y()
    ),
    "Conquer Chess",
    sf::Style::Fullscreen
  };

  return window;
}
