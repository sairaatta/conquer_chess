#include "played_game_view.h"

#ifndef LOGIC_ONLY

#include "render_window.h"
#include "game_resources.h"
#include "sfml_helper.h"
#include "draw.h"
//#include "draw.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

played_game_view::played_game_view()
  : m_game{create_game_with_standard_starting_position()}
{
  m_controls_bar.set_draw_player_controls(false);
}

bool played_game_view::process_event_impl(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
    set_next_state(program_state::main_menu);
    return false;
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::F2)
    {
      std::ofstream file("replay.pgn");
      file << to_pgn(m_game) << '\n';
      play_sound_effect(message(message_type::done, chess_color::white, piece_type::king));
    }
  }
  return false; // Do not close the window :-)
}

void played_game_view::process_resize_event_impl(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  const screen_coordinate br(event.size.width, event.size.height);

  const screen_rect window_rect(screen_coordinate(0,0), br);
  m_layout = played_game_view_layout(
    window_rect,
    get_default_margin_width()
  );
  m_controls_bar.set_screen_rect(create_controls_bar_area(br));
}


void played_game_view::draw_impl()
{
  draw_layout_panels(*this);
  show_text_panel(*this);
  m_controls_bar.draw();
}

void draw_layout_panels(played_game_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    get_render_window().draw(rectangle);
  }
}

void played_game_view::start_impl()
{
  assert(!is_active());
  set_is_active(true);
}

void played_game_view::stop_impl()
{
  assert(is_active());
  clear_next_state();
  set_is_active(false);
}

void played_game_view::tick_impl(const delta_t dt)
{
  assert(dt.get() > 0.0);
}



void show_text_panel(played_game_view& v)
{
  const auto& g{v.get_game()};
  const auto screen_rect{v.get_layout().get_text()};
  std::stringstream s;
  s << to_pgn(g);
  if (s.str().empty()) s << "[none]";
  draw_text(s.str(), screen_rect, 32);
}

#endif // LOGIC_ONLY
