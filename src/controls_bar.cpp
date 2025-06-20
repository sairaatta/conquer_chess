#include "controls_bar.h"

#ifndef LOGIC_ONLY

#include "physical_controllers.h"
#include "draw.h"
#include "action_number.h"

#include <SFML/Graphics/Color.hpp>

#include <cassert>

controls_bar::controls_bar() : m_window_size{get_default_screen_size()}
{

}

void controls_bar::draw()
{
  const auto controls_bar_rect{create_controls_bar_area(m_window_size)};
  assert(get_height(controls_bar_rect) == 64);
  const int y1{controls_bar_rect.get_tl().get_y()};
  const int y2{controls_bar_rect.get_br().get_y()};
  const int font_size{26};
  const int label_up_width{50};
  const int label_down_width{90};
  const int label_select_width{100};
  const int label_increase_width{120};
  const int label_decrease_width{120};
  const int label_quit_width{80};
  const sf::Color semi_transparent(128, 128, 128, 128);

  int x{controls_bar_rect.get_tl().get_x()};
  int w = 0;
  screen_rect r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));

  // Up
  if (m_draw_up_down_select)
  {
    x += w;
    w = 64;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(sf::Keyboard::Key::Up, r);

    x += w;
    w = label_up_width;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_text("Up", r, font_size);

    x += w;
    w = 16;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
  }

  // Down
  if (m_draw_up_down_select)
  {
    x += w;
    w = 64;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(sf::Keyboard::Key::Down, r);

    x += w;
    w = label_down_width;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_text("Down", r, font_size);

    x += w;
    w = 16;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
  }

  // Select
  if (m_draw_up_down_select)
  {
    x += w;
    w = 64;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(sf::Keyboard::Key::Space, r);

    x += w;
    w = 64;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(sf::Keyboard::Key::Enter, r);

    x += w;
    w = 64;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(sf::Mouse::Button::Left, r);


    x += w;
    w = label_select_width;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_text("Select", r, font_size);

    x += w;
    w = 16;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
  }
  if (m_draw_left_right_increase_descrease)
  {
    x += w;
    w = 64;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(sf::Keyboard::Key::Left, r);

    x += w;
    w = label_decrease_width;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_text("Decrease", r, font_size);

    x += w;
    w = 16;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);

    x += w;
    w = 64;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(sf::Keyboard::Key::Right, r);

    x += w;
    w = label_increase_width;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_text("Increase", r, font_size);

    x += w;
    w = 16;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);

  }

  // Invert
  if (m_draw_invert)
  {
    x += w;
    w = 64;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(sf::Keyboard::Key::LShift, r);

    // Left and right shift are the same symbol
    //x += w;
    //w = 64;
    //r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    //draw_rectangle(r, semi_transparent);
    //draw_input_prompt_symbol(sf::Keyboard::Key::RShift, r);

    x += w;
    w = 64;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(sf::Mouse::Button::Right, r);

    x += w;
    w = label_select_width;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_text("Invert", r, font_size);

    x += w;
    w = 16;
    r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
  }

  if (m_draw_player_controls)
  {
    for (const auto player_side: { side::lhs, side::rhs })
    {
      const auto& c{physical_controllers::get().get_controller(player_side)};
      if (player_side == side::rhs) x = m_window_size.get_x() / 2;

      // Symbol
      x += w;
      w = 64;
      r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
      draw_rectangle(r, semi_transparent);
      draw_physical_controller_symbol(c.get_type(), r);

      // Up

      x += w;
      w = 64;
      r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
      draw_rectangle(r, semi_transparent);
      draw_input_prompt_symbol(c.get_key_bindings().get_key_for_move_up(), r);

      x += w;
      w = label_up_width;
      r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
      draw_rectangle(r, semi_transparent);
      draw_text("Up", r, font_size);

      x += w;
      w = 16;
      r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
      draw_rectangle(r, semi_transparent);

      // Down
      x += w;
      w = 64;
      r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
      draw_rectangle(r, semi_transparent);
      draw_input_prompt_symbol(c.get_key_bindings().get_key_for_move_down(), r);

      x += w;
      w = label_down_width;
      r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
      draw_rectangle(r, semi_transparent);
      draw_text("Down", r, font_size);

      x += w;
      w = 16;
      r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
      draw_rectangle(r, semi_transparent);

      // Select
      x += w;
      w = 64;
      r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
      draw_rectangle(r, semi_transparent);
      draw_input_prompt_symbol(c.get_key_bindings().get_key_for_action(action_number(1)), r);

      x += w;
      w = label_select_width;
      r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
      draw_rectangle(r, semi_transparent);
      draw_text("Select", r, font_size);

      x += w;
      w = 16;
      r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
      draw_rectangle(r, semi_transparent);
    }
  }

  // Quit
  x += w;
  w = 64;
  r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
  draw_rectangle(r, semi_transparent);
  draw_input_prompt_symbol(sf::Keyboard::Key::Escape, r);

  x += w;
  w = 64;
  r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
  draw_rectangle(r, semi_transparent);
  draw_input_prompt_symbol(sf::Keyboard::Key::F4, r);

  x += w;
  w = label_quit_width;
  r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
  draw_rectangle(r, semi_transparent);
  draw_text("Quit", r, font_size);

  x += w;
  w = 16;
  r = screen_rect(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
  draw_rectangle(r, semi_transparent);
}

screen_rect create_controls_bar_area(const screen_coordinate& window_size) noexcept
{

  const int controls_bar_height{64};
  const int x1{0 + 16};
  const int x2{window_size.get_x() - 16};
  const int y2{window_size.get_y() - 16};
  const int y1{y2 - controls_bar_height};
  return screen_rect(screen_coordinate(x1, y1), screen_coordinate(x2, y2));
}

#endif // LOGIC_ONLY
