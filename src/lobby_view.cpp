#include "lobby_view.h"

#ifndef LOGIC_ONLY

#include "action_number.h"
#include "render_window.h"
#include "screen_coordinate.h"

#include "draw.h"
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
      m_next_state = program_state::game;
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
        case lobby_view_item::start:
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
    else if (key_pressed == sf::Keyboard::Key::T)
    {
      m_dog_mode = !m_dog_mode;
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
        case lobby_view_item::start:
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
  draw_lobby_title(*this);
  draw_color_panel(*this, side::lhs);
  draw_controls_panel(*this, side::lhs);
  draw_king_portrait(*this, side::lhs);
  draw_race_panel(*this, side::lhs);
  draw_ready_panel(*this, side::lhs);
  draw_color_panel(*this, side::rhs);
  draw_controls_panel(*this, side::rhs);
  draw_king_portrait(*this, side::rhs);
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

  draw_fancy_text(to_human_str(get_color(player_side)), screen_rect, 64, 60);
}

void draw_controls_panel(lobby_view& v, const side player_side)
{
  const auto &layout{v.get_layout()};
  const auto& c{physical_controllers::get().get_controller(player_side)};

  draw_physical_controller_symbol(
    c.get_type(),
    create_centered_rect(get_center(layout.get_control_symbol(player_side)), 64, 64)
  );

  // Up
  draw_text(
    sf::String("Up"),
    layout.get_control_up_label(player_side),
    get_normal_character_size(),
    sf::Color::White
  );
  draw_input_prompt_symbol(
    c.get_key_bindings().get_key_for_move_up(),
    create_centered_rect(get_center(layout.get_control_up_symbol(player_side)), 64, 64)
  );

  // Down
  draw_text(
    sf::String("Down"),
    layout.get_control_down_label(player_side),
    get_normal_character_size(),
    sf::Color::White
  );
  draw_input_prompt_symbol(
    c.get_key_bindings().get_key_for_move_down(),
    create_centered_rect(get_center(layout.get_control_down_symbol(player_side)), 64, 64)
  );


  // Select
  draw_text(
    sf::String("Select"),
    layout.get_control_select_label(player_side),
    get_normal_character_size(),
    sf::Color::White
  );
  draw_input_prompt_symbol(
    c.get_key_bindings().get_key_for_action(action_number(1)),
    create_centered_rect(get_center(layout.get_control_select_symbol(player_side)), 64, 64)
  );

  screen_rect panel{
    screen_coordinate(layout.get_control_symbol(player_side).get_tl()),
    screen_coordinate(layout.get_control_select(player_side).get_br())
  };
  sf::Color outline_color;
  switch (player_side)
  {
    case side::lhs: outline_color = sf::Color::Red; break;
    case side::rhs: outline_color = sf::Color::Blue; break;
  }
  draw_outline(panel, outline_color);

}

void draw_countdown(lobby_view& v, const int n_left_secs)
{
  const screen_rect window_rect{
    screen_coordinate(0, 0),
    v.get_layout().get_window_size()
  };
  draw_huge_fancy_text(std::to_string(n_left_secs), window_rect);
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
  const std::string s{to_human_str(get_race_of_side(player_side))};
  draw_fancy_text(s, text_rect, 64, 60);
}

void draw_background(lobby_view& v)
{
  const auto screen_rect{v.get_layout().get_background()};
  draw_texture(
    game_resources::get().get_map_textures().get_map(race::classic),
    //game_resources::get().get_textures().get_all_races_1(),
    screen_rect
  );
}

void draw_king_portrait(lobby_view& v, const side player_side)
{
  const auto player_color{get_color(player_side)};
  if (!v.get_dog_mode())
  {
    const auto player_race{get_race_of_side(player_side)};
    draw_texture(
      game_resources::get().get_piece_portrait_textures().get_portrait(
        player_race,
        player_color,
        piece_type::king
      ),
      v.get_layout().get_king_portrait(player_side)
    );
  }
  else
  {
    const int index{player_color == chess_color::white ? 1 : 2};
    draw_texture(
      game_resources::get().get_themba_textures().get_texture(index),
      v.get_layout().get_king_portrait(player_side)
    );
  }
  sf::Color outline_color;
  switch (player_side)
  {
    case side::lhs: outline_color = sf::Color::Red; break;
    case side::rhs: outline_color = sf::Color::Blue; break;
  }
  draw_outline(
    v.get_layout().get_king_portrait(player_side),
    outline_color
  );
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
  sf::Color c;
  switch (player_side)
  {
    case side::lhs: c = sf::Color::Red; break;
    case side::rhs: c = sf::Color::Blue; break;
  }
  draw_outline(select_rect, c);
}

void draw_ready_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_start(player_side)};
  draw_texture(
    game_resources::get().get_lobby_menu_textures().get_ready(v.get_start(player_side)),
    screen_rect
  );

  // Text
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  const bool is_ready{v.get_start(player_side)};
  const std::string s{is_ready ? "Ready" : "Not ready"};
  draw_fancy_text(s, text_rect, 64, 60);
}

void draw_lobby_title(lobby_view& v)
{
  const auto screen_rect{v.get_layout().get_title()};
  draw_squares(screen_rect, true);
  //draw_texture(
  //  game_resources::get().get_map_textures().get_map(race::classic),
  //  screen_rect
  //);
  draw_fancy_text("Lobby", screen_rect, 120, 116);

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
  if (m_clock.has_value())
  {
    m_clock.value().restart();
  }
  m_clock = {};
  m_lhs_cursor = lobby_view_item::color;
  m_rhs_cursor = lobby_view_item::color;
  m_lhs_start = false;
  m_rhs_start = false;
}

void lobby_view::stop()
{
  game_resources::get().get_songs().get_soothing().stop();
  if (m_clock.has_value())
  {
    m_clock.value().restart();
  }
  m_clock = {};
  m_lhs_start = false;
  m_rhs_start = false;
}

#endif // LOGIC_ONLY
