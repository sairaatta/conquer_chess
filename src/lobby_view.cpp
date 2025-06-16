#include "lobby_view.h"

#ifndef LOGIC_ONLY

#include "action_number.h"
#include "render_window.h"
#include "screen_coordinate.h"
#include "helper.h"
#include "lobby_options.h"
#include "game_options.h"
#include "game_resources.h"
#include "render_window.h"
#include "sfml_helper.h"
#include "physical_controllers.h"

#include <cassert>
#include <cmath>

lobby_view::lobby_view()
  : m_lhs_cursor{lobby_view_item::color},
    m_lhs_start{false},
    m_rhs_cursor{lobby_view_item::color},
    m_rhs_start{false}
{
}

void lobby_view::tick()
{
  if (m_clock)
  {
    if (m_clock.value().getElapsedTime().asSeconds() > m_countdown_secs)
    {
      assert(!"TODO");
      //m_next_state = program_state::game;
    }
  }
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
    else if (key_pressed == sf::Keyboard::Key::Q)
    {
      auto& lo{lobby_options::get()};
      switch (m_lhs_cursor)
      {
        case lobby_view_item::color:
          lo.set_color(
            get_next(lo.get_color(side::lhs)),
            side::lhs
          );
          m_lhs_start = false;
          m_rhs_start = false;
          break;
        case lobby_view_item::race:
          lo.set_race(
            get_next(lo.get_race(side::lhs)),
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
          lobby_options::get().set_color(
            get_next(lobby_options::get().get_color(side::rhs)),
            side::rhs
          );
          m_lhs_start = false;
          m_rhs_start = false;
          break;
        case lobby_view_item::race:
          lobby_options::get().set_race(
            get_next(lobby_options::get().get_race(side::rhs)),
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
      m_next_state = program_state::main_menu;
      return false;
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
  draw_background(*this);
  draw_layout_panels(*this);
  draw_title(*this);
  draw_color_panel(*this, side::lhs);
  draw_controls_panel(*this, side::lhs);
  draw_race_panel(*this, side::lhs);
  draw_ready_panel(*this, side::lhs);
  draw_color_panel(*this, side::rhs);
  draw_controls_panel(*this, side::rhs);
  draw_race_panel(*this, side::rhs);
  draw_ready_panel(*this, side::rhs);
  draw_selected_panel(*this, side::lhs);
  draw_selected_panel(*this, side::rhs);
  if (m_clock)
  {
    const int n_left{
      m_countdown_secs
      - static_cast<int>(
        m_clock.value().getElapsedTime().asSeconds()
      )
    };
    draw_countdown(*this, n_left);
  }

}

void draw_color_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_color(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_lobby_menu_textures().get_color(
      get_color(player_side)
    )
  );
  get_render_window().draw(rectangle);

  // Text
  sf::Text text;
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  std::string s{to_str(get_color(player_side))};
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

void draw_controls_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_controls(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  const auto player_color{get_color(player_side)};
  const auto player_race{get_race_of_side(player_side)};
  rectangle.setTexture(
    &game_resources::get().get_piece_portrait_textures().get_portrait(
      player_race,
      player_color,
      piece_type::king
    )
  );
  get_render_window().draw(rectangle);

  // Text
  sf::Text text;
  const auto text_rect{
    get_lower_half(screen_rect)
  };

  const auto& c{physical_controllers::get().get_controller(player_side)};
  const std::string s{
    to_str(c.get_type())
    + std::string("\n")
    + std::string("Up: ")
    + to_str(c.get_key_bindings().get_key_for_move_up())
    + std::string("\n")
    + std::string("Down: ")
    + to_str(c.get_key_bindings().get_key_for_move_down())
    + std::string("\n")
    + std::string("Select: ")
    + to_str(c.get_key_bindings().get_key_for_action(action_number(1)))
    + std::string("\n")
  };
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

void draw_countdown(lobby_view& v, const int n_left_secs)
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

void draw_race_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_race(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_lobby_menu_textures().get_head(
      get_race_of_side(player_side)
    )
  );
  get_render_window().draw(rectangle);

  // Text
  sf::Text text;
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  std::string s{to_str(get_race_of_side(player_side))};
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

void draw_background(lobby_view& v)
{
  const auto screen_rect{v.get_layout().get_background()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_textures().get_all_races_1()
  );
  get_render_window().draw(rectangle);
}

void draw_layout_panels(lobby_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    get_render_window().draw(rectangle);
  }
}

void draw_selected_panel(lobby_view& v, const side player_side)
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

void draw_ready_panel(lobby_view& v, const side player_side)
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

void draw_title(lobby_view& v)
{
  const auto screen_rect{v.get_layout().get_title()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_map_textures().get_map(race::classic)
  );
  get_render_window().draw(rectangle);

  // Text
  sf::Text text;
  text.setString("Lobby");
  v.set_text_style(text);
  text.setCharacterSize(text.getCharacterSize() * 3);

  set_text_position(text, screen_rect);
  get_render_window().draw(text);

  // Smaller
  text.setCharacterSize(text.getCharacterSize() - 4);
  set_text_position(text, screen_rect);
  text.setFillColor(sf::Color::White);
  get_render_window().draw(text);

}

void lobby_view::start()
{
  game_resources::get().get_songs().get_soothing().setVolume(
    get_music_volume_as_percentage()
  );
  game_resources::get().get_sound_effects().set_master_volume(
    game_options::get().get_sound_effects_volume()
  );
  game_resources::get().get_songs().get_soothing().play();
  m_clock = {};
  m_lhs_start = false;
  m_rhs_start = false;
}

void lobby_view::stop()
{
  game_resources::get().get_songs().get_soothing().stop();
  m_clock = {};
  m_lhs_start = false;
  m_rhs_start = false;
}

#endif // LOGIC_ONLY
