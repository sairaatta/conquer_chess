#include "lobby_view.h"

#ifndef LOGIC_ONLY

#include "render_window.h"
#include "about_view.h"
#include "screen_coordinate.h"
#include "game_view.h"
#include "played_game_view.h"
#include "options_view.h"
#include "render_window.h"
#include "sfml_helper.h"
#include <cassert>
#include <cmath>
#include <iostream>

lobby_view::lobby_view()
  : m_lhs_cursor{lobby_view_item::color},
    m_lhs_start{false},
    m_lobby_options{create_default_lobby_options()},
    m_rhs_cursor{lobby_view_item::color},
    m_rhs_start{false}
{
  game_resources::get().get_songs().get_soothing().setVolume(
    get_music_volume_as_percentage(game_options::get())
  );
  game_resources::get().get_sound_effects().set_master_volume(
    game_options::get().get_sound_effects_volume()
  );
  game_resources::get().get_songs().get_soothing().setLoop(true);
  game_resources::get().get_songs().get_soothing().play();

}

void lobby_view::tick()
{
  if (m_clock)
  {
    if (m_clock.value().getElapsedTime().asSeconds() > m_countdown_secs)
    {
      m_clock = {};
      exec_game();
      m_lhs_start = false;
      m_rhs_start = false;
    }
  }
}

void lobby_view::exec_game()
{
  const auto cur_pos{get_render_window().getPosition()};
  game_resources::get().get_songs().get_soothing().stop();
  get_render_window().setVisible(false);
  game_view view{
    game(m_lobby_options)
  };
  view.tick();
  get_render_window().setVisible(true);
  get_render_window().setPosition(cur_pos);
  game_resources::get().get_songs().get_soothing().play();
}


lobby_view_item lobby_view::get_selected(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_cursor;
  }
  assert(player_side == side::rhs);
  return m_rhs_cursor;
}

bool lobby_view::get_start(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_start;
  }
  assert(player_side == side::rhs);
  return m_rhs_start;
}

bool lobby_view::process_event(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
    get_render_window().close();
    return true; // Close lobby
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      get_render_window().close();
      return true;
    }
    else if (key_pressed == sf::Keyboard::Key::Q)
    {
      switch (m_lhs_cursor)
      {
        case lobby_view_item::color:
          m_lobby_options.set_color(
            get_next(m_lobby_options.get_color(side::lhs)),
            side::lhs
          );
          m_lhs_start = false;
          m_rhs_start = false;
          break;
        case lobby_view_item::race:
          m_lobby_options.set_race(
            get_next(m_lobby_options.get_race(side::lhs)),
            side::lhs
          );
          m_lhs_start = false;
          m_rhs_start = false;
          break;
        default:
        case lobby_view_item::start:
          assert(m_lhs_cursor == lobby_view_item::start);
          m_lhs_start = !m_lhs_start;
          break;
      }
      game_resources::get().get_sound_effects().play_hide();
    }
    else if (key_pressed == sf::Keyboard::Key::W)
    {
      m_lhs_cursor = get_previous(m_lhs_cursor);
      game_resources::get().get_sound_effects().play_hide();
    }
    else if (key_pressed == sf::Keyboard::Key::S)
    {
      m_lhs_cursor = get_next(m_lhs_cursor);
      game_resources::get().get_sound_effects().play_hide();
    }
    else if (key_pressed == sf::Keyboard::Key::O)
    {
      m_rhs_cursor = get_previous(m_rhs_cursor);
      game_resources::get().get_sound_effects().play_hide();
    }
    else if (key_pressed == sf::Keyboard::Key::L)
    {
      m_rhs_cursor = get_next(m_rhs_cursor);
      game_resources::get().get_sound_effects().play_hide();
    }
    else if (key_pressed == sf::Keyboard::Key::I)
    {
      switch (m_rhs_cursor)
      {
        case lobby_view_item::color:
          m_lobby_options.set_color(
            get_next(m_lobby_options.get_color(side::rhs)),
            side::rhs
          );
          m_lhs_start = false;
          m_rhs_start = false;
          break;
        case lobby_view_item::race:
          m_lobby_options.set_race(
            get_next(m_lobby_options.get_race(side::rhs)),
            side::rhs
          );
          m_lhs_start = false;
          m_rhs_start = false;
          break;
        default:
        case lobby_view_item::start:
          assert(m_rhs_cursor == lobby_view_item::start);
          m_rhs_start = !m_rhs_start;
          break;
      }
      game_resources::get().get_sound_effects().play_hide();
    }
    else if (key_pressed == sf::Keyboard::Key::Q)
    {
      get_render_window().close();
      return true;
    }
  }
  if (m_lhs_start && m_rhs_start && !m_clock.has_value())
  {
    m_clock = sf::Clock();
    game_resources::get().get_sound_effects().play_countdown();

  }
  else if (!(m_lhs_start && m_rhs_start) && m_clock.has_value())
  {
    m_clock = {};
  }

  return false; // Do not close the lobby :-)
}

void lobby_view::process_resize_event(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  // From https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized
  const sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
  get_render_window().setView(sf::View(visible_area));

  m_layout = lobby_view_layout(
    screen_coordinate(event.size.width, event.size.height),
    get_default_margin_width()
  );
}


void lobby_view::set_text_style(sf::Text& text)
{
  text.setFont(get_arial_font());
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(m_layout.get_font_size());
  text.setFillColor(sf::Color::Black);
}

void lobby_view::draw()
{
  show_layout_panels(*this);
  show_image_panel(*this);
  show_color_panel(*this, side::lhs);
  show_race_panel(*this, side::lhs);
  show_start_panel(*this, side::lhs);
  show_color_panel(*this, side::rhs);
  show_race_panel(*this, side::rhs);
  show_start_panel(*this, side::rhs);
  show_selected_panel(*this, side::lhs);
  show_selected_panel(*this, side::rhs);
  if (m_clock)
  {
    const int n_left{
      m_countdown_secs
      - static_cast<int>(
        m_clock.value().getElapsedTime().asSeconds()
      )
    };
    show_countdown(*this, n_left);
  }

}

void show_color_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_color(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_lobby_menu_textures().get_color(
      v.get_options().get_color(player_side)
    )
  );
  get_render_window().draw(rectangle);

  // Text
  sf::Text text;
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  std::string s{to_str(v.get_options().get_color(player_side))};
  s[0] = std::toupper(s[0]);
  text.setString(s);
  v.set_text_style(text);
  set_text_position(text, text_rect);
  get_render_window().draw(text);

  // Smaller
  text.setCharacterSize(text.getCharacterSize() - 2);
  set_text_position(text, text_rect);
  text.setFillColor(sf::Color::White);
  get_render_window().draw(text);
}

void show_countdown(lobby_view& v, const int n_left_secs)
{
  // Text
  const screen_rect window_rect{
    screen_coordinate(0, 0),
    v.get_layout().get_window_size()
  };
  sf::Text text;
  std::string s{std::to_string(n_left_secs)};
  text.setString(s);
  v.set_text_style(text);
  text.setCharacterSize(512);
  set_text_position(text, window_rect);
  get_render_window().draw(text);

  // Smaller
  text.setCharacterSize(512 - 2);
  set_text_position(text, window_rect);
  text.setFillColor(sf::Color::White);
  get_render_window().draw(text);

}

void show_race_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_race(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_lobby_menu_textures().get_head(
      v.get_options().get_race(player_side)
    )
  );
  get_render_window().draw(rectangle);

  // Text
  sf::Text text;
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  std::string s{to_str(v.get_options().get_race(player_side))};
  s[0] = std::toupper(s[0]);
  text.setString(s);
  v.set_text_style(text);
  set_text_position(text, text_rect);
  get_render_window().draw(text);

  // Smaller
  text.setCharacterSize(text.getCharacterSize() - 2);
  set_text_position(text, text_rect);
  text.setFillColor(sf::Color::White);
  get_render_window().draw(text);
}

void show_image_panel(lobby_view& v)
{
  const auto screen_rect{v.get_layout().get_image()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_textures().get_all_races_1()
  );
  get_render_window().draw(rectangle);
}

void show_layout_panels(lobby_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    get_render_window().draw(rectangle);
  }
}

void show_selected_panel(lobby_view& v, const side player_side)
{
  const auto select_rect{
    get_cursor_rect(
      v.get_layout(),
      v.get_selected(player_side),
      player_side
    )
  };
  sf::RectangleShape rectangle;
  set_rect(rectangle, select_rect);
  rectangle.setOrigin(
    get_width(select_rect) / 2,
    get_height(select_rect) / 2
  );
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(5);
  get_render_window().draw(rectangle);
}

void show_start_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_start(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_lobby_menu_textures().get_ready(
      v.get_start(player_side)
    )
  );
  get_render_window().draw(rectangle);

  // Text
  sf::Text text;
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  if (v.get_start(player_side))
  {
    text.setString("Ready");
  }
  else
  {
    text.setString("Not ready");
  }
  v.set_text_style(text);
  set_text_position(text, text_rect);
  get_render_window().draw(text);

  // Smaller
  text.setCharacterSize(text.getCharacterSize() - 2);
  set_text_position(text, text_rect);
  text.setFillColor(sf::Color::White);
  get_render_window().draw(text);
}

void lobby_view::start()
{

}

void lobby_view::stop()
{

}

#endif // LOGIC_ONLY
