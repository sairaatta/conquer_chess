#include "about_view.h"

#ifndef LOGIC_ONLY

#include "screen_coordinate.h"
#include "game_resources.h"
#include "sfml_helper.h"
#include "render_window.h"
#include <cassert>
#include <cmath>
#include <sstream>

about_view::about_view()
{

}

void about_view::tick()
{
  m_rotation = 5.0 * std::sin(m_clock.getElapsedTime().asMilliseconds() / 1000.0);
}

bool about_view::process_event(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
    m_next_state = program_state::main_menu;
    return false;
  }
  if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      m_next_state = program_state::main_menu;
      return false;
    }
  }

  return false;
}

void about_view::process_resize_event(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  m_layout = about_view_layout(
    screen_coordinate(event.size.width, event.size.height),
    get_default_margin_width()
  );
}

void about_view::set_text_style(sf::Text& text)
{
  text.setFont(get_arial_font());
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(m_layout.get_font_size());
  text.setFillColor(sf::Color::Black);
}

void about_view::draw()
{
  show_layout_panels(*this);
  show_title_panel(*this);
  show_subtitle_panel(*this);
  show_text_panel(*this);
}

void show_layout_panels(about_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    get_render_window().draw(rectangle);
  }
}

void show_subtitle_panel(about_view& v)
{
  const auto screen_rect{v.get_layout().get_subtitle()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_subtitle()
  );
  get_render_window().draw(rectangle);
}

void show_text_panel(about_view& v)
{
  const auto screen_rect{v.get_layout().get_text()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_game_option_icon(
      options_view_item::starting_position
    )
  );
  rectangle.setFillColor(sf::Color(255, 255, 255, 128));
  get_render_window().draw(rectangle);
  std::stringstream s;
  s
    << "Conquer Chess\n"
    << "\n"
    << "(C) 2022-2025 Richel Bilderbeek\n"
    << "\n"
    << "https://github.com/\n"
    << "  richelbilderbeek/\n"
    << "  conquer_chess\n"
    << "\n"
    << "Contributors:\n"
    << "\n"
    << " * Johanna Soederstroem\n"
    << " * Leonid Rutkowski\n"
  ;
  sf::Text text;
  text.setString(s.str().c_str());
  v.set_text_style(text);
  set_text_position(text, screen_rect);
  text.setFillColor(sf::Color::White);
  get_render_window().draw(text);
}

void show_title_panel(about_view& v)
{
  const auto screen_rect{v.get_layout().get_title()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_title()
  );
  get_render_window().draw(rectangle);
}

void about_view::start()
{
  m_rotation = 0.0;
  m_next_state.reset();
}

void about_view::stop()
{
  m_next_state.reset();
}

#endif // LOGIC_ONLY
