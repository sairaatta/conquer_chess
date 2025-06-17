#include "about_view.h"

#ifndef LOGIC_ONLY

#include "about.h"
#include "draw.h"
#include "screen_coordinate.h"
#include "game_resources.h"
#include "sfml_helper.h"
#include "render_window.h"
#include <cassert>
#include <cmath>

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

void about_view::draw()
{
  draw_background(*this);
  draw_panel(*this);
  draw_layout_panels(*this);
  draw_title(*this);
  draw_subtitle(*this);
  draw_copyright(*this);
  draw_url(*this);
  draw_contributors(*this);
}

void draw_background(about_view& v)
{
  draw_texture(
    get_game_option_icon(options_view_item::starting_position),
    v.get_layout().get_background()
  );

}


void draw_contributors(about_view& v)
{
  const auto screen_rect{v.get_layout().get_contributors()};
  draw_texture(
    get_strip_texture(chess_color::white),
    screen_rect
  );

  std::vector<sf::String> texts;
  texts.push_back(sf::String("Contributors:"));
  const auto names = get_contributor_names();
  std::transform(
    std::begin(names),
    std::end(names),
    std::back_inserter(texts),
    [](const auto& n) { return sf::String("- ") + n; }
  );
  draw_normal_texts(texts, screen_rect);

}

void draw_copyright(about_view& v)
{
  draw_texture(
    get_strip_texture(chess_color::white),
    v.get_layout().get_copyright()
  );
  draw_normal_text(
    sf::String("(C) 2022-2025 ") + sf::String(get_author_name()),
    v.get_layout().get_copyright()
  );
}

void draw_url(about_view& v)
{
  draw_texture(
    get_strip_texture(chess_color::black),
    v.get_layout().get_url()
  );

  draw_normal_text(
    sf::String(get_homepage_url()),
    v.get_layout().get_url()
  );
}

void draw_layout_panels(about_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    get_render_window().draw(rectangle);
  }
}

void draw_panel(about_view& v)
{
  draw_outline(
    v.get_layout().get_panel_outline()
  );

}

void draw_subtitle(about_view& v)
{
  draw_texture(
    get_subtitle_texture(),
    v.get_layout().get_subtitle()
  );
}

void draw_title(about_view& v)
{
  draw_texture(
    get_title_texture(),
    v.get_layout().get_title()
  );
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
