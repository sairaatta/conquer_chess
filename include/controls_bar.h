#ifndef CONTROLS_BAR_H
#define CONTROLS_BAR_H

#ifndef LOGIC_ONLY

#include "screen_rect.h"

// The bar at the bottom of the screen
class controls_bar
{
public:

  controls_bar();

  void draw();

  /// Show that there is a way to invert a direction, by pressing shift
  void set_draw_invert(const bool b) noexcept { m_draw_invert = b; }
  void set_draw_player_controls(const bool b) noexcept { m_draw_player_controls = b; }

  /// Draw the arrows to left and right, with the texts 'decrease' and 'increase'
  void set_draw_left_right_increase_descrease(const bool b) noexcept { m_draw_left_right_increase_descrease = b; }
  void set_draw_up_down(const bool b) noexcept { m_draw_up_down = b; }
  void set_draw_select(const bool b) noexcept { m_draw_select = b; }

  void set_screen_rect(const screen_rect& r) noexcept { m_background = r; }

private:

  screen_rect m_background;

  bool m_draw_invert{false};
  bool m_draw_left_right_increase_descrease{false};
  bool m_draw_player_controls{true};
  bool m_draw_up_down{true};
  bool m_draw_select{true};
};

/// At the bottom of each window, there is a bar for the controls.
/// Create it in a uniform way
screen_rect create_controls_bar_area(const screen_coordinate& window_size) noexcept;

#endif // LOGIC_ONLY

#endif // CONTROLS_BAR_H
