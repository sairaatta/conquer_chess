#include "played_game_view.h"

#ifndef LOGIC_ONLY

#include "screen_coordinate.h"
#include "render_window.h"
#include "game_resources.h"
#include "sfml_helper.h"
//#include "draw.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

played_game_view::played_game_view(const game& g)
  : m_game{g}
{
  m_controls_bar.set_draw_player_controls(false);
}

void played_game_view::exec()
{

}

bool played_game_view::process_event(sf::Event& event)
{
  if (event.type == sf::Event::Resized)
  {
    // From https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized
    const sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
    get_render_window().setView(sf::View(visible_area));

    m_layout = played_game_view_layout(
      screen_coordinate(event.size.width, event.size.height),
      get_default_margin_width()
    );
  }
  else if (event.type == sf::Event::Closed)
  {
    m_next_state = program_state::main_menu;
    return false;
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      m_next_state = program_state::main_menu;
      return false;
    }
    else if (key_pressed == sf::Keyboard::Key::F2)
    {
      std::ofstream file("replay.pgn");
      file << to_pgn(m_game) << '\n';
      play_sound_effect(message(message_type::done, chess_color::white, piece_type::king));
    }
  }
  return false; // Do not close the window :-)
}

void played_game_view::set_text_style(sf::Text& text)
{
  text.setFont(get_code_squared_font());
  //text.setStyle(sf::Text::Bold);
  text.setCharacterSize(m_layout.get_font_size());
  text.setFillColor(sf::Color::White);
}

void played_game_view::draw()
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

void show_text_panel(played_game_view& v)
{
  const auto& g{v.get_game()};
  const auto screen_rect{v.get_layout().get_text()};
  std::stringstream s;
  s << to_pgn(g);
  if (s.str().empty()) s << "[none]";
  sf::Text text;
  text.setString(s.str().c_str());
  v.set_text_style(text);
  set_text_position(text, screen_rect);
  get_render_window().draw(text);
}

#endif // LOGIC_ONLY
