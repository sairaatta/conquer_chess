#include "game_view.h"

#ifndef LOGIC_ONLY

#include "board_layout.h"
#include "draw.h"
#include "game.h"
#include "game_options.h"
#include "game_rect.h"
#include "game_resources.h"
#include "game_view_layout.h"
#include "game_info_layout.h"
#include "lobby_options.h"
#include "physical_controller.h"
#include "physical_controllers.h"
#include "square_layout.h"
#include "pieces.h"
#include "render_window.h"
#include "physical_controller.h"
#include "screen_coordinate.h"
#include "screen_rect.h"
#include "screen_rect.h"
#include "sfml_helper.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <cmath>
#include <string>
#include <sstream>

game_view::game_view(
) : m_log{game_options::get().get_message_display_time_secs()}
{
  m_controls_bar.set_draw_up_down(false);
  m_controls_bar.set_draw_select(false);
  m_controls_bar.set_draw_player_controls(false);

}


void game_view::tick_impl(delta_t dt)
{
  assert(is_active());

  // Disard old messages
  m_log.tick();

  // Do a tick, so that one delta_t equals one second under normal game speed
  m_game.tick(dt);

  // Read the pieces' messages and play their sounds
  process_piece_messages();

  // Show the new state
  draw_impl();

  game_resources::get().get_songs().get_wonderful_time().stop();
}

const physical_controller& get_physical_controller(const game_view& view, const side player_side)
{
  return get_physical_controller(view.get_game_controller(), player_side);
}

physical_controller_type get_physical_controller_type(const game_view& view, const side player_side)
{
  return get_physical_controller(view, player_side).get_type();
}

std::string get_controls_text(
  const game_view& view,
  const game_controller& c,
  const side player_side,
  const action_number& n
)
{
  return get_text_for_action(view, c, player_side, n);
}

double game_view::get_elapsed_time_secs() const noexcept
{
  return m_clock.getElapsedTime().asSeconds();
}


std::string get_last_log_messages(
  const game_view& view,
  const side player
) noexcept
{
  return get_last_log_messages(
    view.get_log(),
    get_player_color(player)
  );
}

const game_view_layout& get_layout(const game_view& v) noexcept
{
  return v.get_layout();
}

const std::vector<piece>& get_pieces(const game_view& v) noexcept
{
  return get_pieces(v.get_game());
}

void game_view::play_pieces_sound_effects()
{
  for (const auto& sound_effect: collect_messages(m_game))
  {
    game_resources::get().get_sound_effects().play(sound_effect);
  }
}

const game_coordinate& get_cursor_pos(const game_view& view, const side player) noexcept
{
  return get_cursor_pos(view.get_game_controller(), player);
}

std::string get_text_for_action(
  const game_view& view,
  const game_controller& c,
  const side player_side,
  const action_number& n
)
{
  const auto& g{view.get_game()};
  const auto maybe_action{get_piece_action(g, c, n, player_side)};

  if (!maybe_action) return "";
  return to_human_str(maybe_action.value());
}

const in_game_time& get_time(const game_view& v) noexcept
{
  return get_time(v.get_game());
}

bool game_view::process_event_impl(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
      set_next_state(program_state::lobby);
      return false;
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      set_next_state(program_state::lobby);
      return false;
    }
  }
  ::process_event(m_game_controller, event, m_layout);
  m_game_controller.apply_user_inputs_to_game(m_game);
  return false;
}

void game_view::process_resize_event_impl(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  const screen_rect w(
    screen_coordinate(0, 0),
    screen_coordinate(event.size.width, event.size.height)
  );
  m_layout = game_view_layout(
    w,
    get_default_margin_width()
  );
  m_controls_bar.set_screen_rect(w);
}

void process_event(
  game_controller& c,
  const sf::Event& event,
  const game_view_layout& layout
)
{
  for (const auto s: get_all_sides())
  {
    const physical_controller& p{get_physical_controller(c, s)};
    const user_inputs& inputs{p.process_input(event, s, layout)};
    for (const auto& a: inputs.get_user_inputs())
    {
      add_user_input(c, a);
    }
  }
}

void game_view::process_piece_messages()
{
  for (const auto& piece_message: collect_messages(m_game))
  {
    m_log.add_message(piece_message);
  }

  // Play the new sounds to be played
  play_pieces_sound_effects();

  clear_piece_messages(m_game);
}

void game_view::draw_impl()
{
  // Show the layout of the screen: board and sidebars
  draw_background(*this);

  // Show the layout of the screen: board and sidebars
  show_layout(*this);


  // Show the board: squares, unit paths, pieces, health bars
  draw_board(*this);

  // Clock time
  draw_game_info(*this);

  // Show the sidebars: controls (with log), units, debug
  show_sidebar(*this, side::lhs);
  show_sidebar(*this, side::rhs);

  draw_navigation_controls(*this);

  m_controls_bar.draw();
}

void draw_board(game_view& view)
{
  draw_squares(view);
  if (game_options::get().get_show_occupied())
  {
    show_occupied_squares(view);
  }
  draw_square_under_cursor(view, side::lhs);
  draw_square_under_cursor(view, side::rhs);
  draw_possible_moves(view);
  draw_unit_paths(view);
  draw_pieces(view);
  draw_unit_health_bars(view);
}

void draw_controls(
  game_view& view,
  const side player_side
)
{
  // Stub for keyboard only
  const auto& c{physical_controllers::get().get_controller(player_side)};

  const auto& layout{view.get_layout()};

  // Blur the entire background here
  draw_rectangle(layout.get_controls(player_side), sf::Color(128, 128, 128, 128));
  draw_navigation_controls(layout.get_navigation_controls(player_side), player_side);


  for (const auto n: get_all_action_numbers())
  {
    const screen_rect row_rect{
      layout.get_controls_key(player_side, n)
    };
    const screen_rect symbol_rect{
      row_rect.get_tl(),
      row_rect.get_tl() + screen_coordinate(64, 64)
    };
    if (c.get_type() == physical_controller_type::keyboard)
    {
      draw_input_prompt_symbol(
        physical_controllers::get().get_controller(player_side).get_key_bindings().get_key_for_action(n),
        symbol_rect
      );
    }
    else
    {
      /*
      draw_input_prompt_symbol(
        physical_controllers::get().get_controller(player_side).get_mouse_bindings().get_do_it_button()),
        symbol_rect
      );
      */
    }
    const screen_rect text_rect{
      symbol_rect.get_tl() + screen_coordinate(64, 0),
      screen_coordinate(row_rect.get_br().get_x(), symbol_rect.get_br().get_y())
    };
    const std::string text{
      get_controls_text(view, view.get_game_controller(), player_side, n)
    };
    if (!text.empty())
    {
      draw_text(text, text_rect, 26);
    }
  }
}

void draw_game_info(game_view& view)
{
  const game_info_layout layout(view.get_layout().get_game_info());

  // Background
  draw_rectangle(layout.get_background(), sf::Color(128, 128, 128, 128));

  // Draw all symbols
  for (const auto s: get_all_game_info_statistics())
  {
    draw_texture(
      game_resources::get().get_board_game_textures().get_symbol(s),
      layout.get_symbol(s),
      game_resources::get().get_board_game_textures().get_symbol_color(s)
    );
  }

  // Clock time
  {
    const auto& r{layout.get_relative(game_info_statistic::time)};
    draw_rectangle(
      r,
      sf::Color(128, 128, 128, 128)
    );
    const std::string s{to_human_str(view.get_game().get_in_game_time())};
    const auto text_rect{create_centered_rect(get_center(r), 80, 48)};
    draw_text(s, text_rect, 32);
  }

  // Statistic
  std::map<game_info_statistic, std::map<side, double>> statistics;
  for (const side s: get_all_sides())
  {
    statistics[game_info_statistic::value][s] = get_f_value(view.get_game().get_pieces(), get_color(s));
    statistics[game_info_statistic::activity][s] = get_f_active(view.get_game().get_pieces(), get_color(s));
    statistics[game_info_statistic::protectedness][s] = get_f_protected(view.get_game().get_pieces(), get_color(s));
  }

  for (const game_info_statistic statistic: get_all_game_info_statistics())
  {
    // Time is displayed differently
    if (statistic == game_info_statistic::time) continue;

    // Relative, the two bars against each other
    {
      const double f{
        statistics[statistic][side::lhs] + statistics[statistic][side::rhs] == 0
        ? 0.5
        : static_cast<double>(statistics[statistic][side::lhs]) / static_cast<double>(statistics[statistic][side::lhs] + statistics[statistic][side::rhs])
      };
      assert(f >= 0.0);
      assert(f <= 1.0);
      const auto& r_border{layout.get_relative(statistic)};
      const auto r{create_rect_inside(r_border)};
      for (const side s: get_all_sides())
      {
        try
        {
          const double f_side{s == side::lhs ? f : 1.0 - f};
          assert(f_side >= 0.0);
          assert(f_side <= 1.0);
          const screen_rect bar_rect{create_partial_rect_from_side(s, r, f_side)};
          const sf::Color bar_color{game_resources::get().get_board_game_textures().get_bar_color(statistic, get_color(s))};
          draw_rectangle(bar_rect, bar_color);
        }
        catch (std::logic_error& e)
        {
          // OK
        }
      }
      draw_outline(
        r_border,
        game_resources::get().get_board_game_textures().get_outline_color(statistic),
        1
      );
    }
    // Absolute
    for (const side s: get_all_sides())
    {
      const double f{statistics[statistic][s]};
      assert(f >= 0.0);
      assert(f <= 1.0);
      const auto& r_border{layout.get_absolute(statistic, s)};
      try
      {
        const auto r{create_rect_inside(r_border)};
        // Start the bar at the player's side
        const screen_rect bar_rect{create_partial_rect_from_side(s, r, f)};
        const sf::Color bar_color{
          game_resources::get().get_board_game_textures().get_bar_color(
            statistic,
            get_color(s)
          )
        };
        draw_rectangle(
          bar_rect,
          bar_color
        );
        draw_outline(
          bar_rect,
          game_resources::get().get_board_game_textures().get_outline_color(statistic),
          1
        );
      }
      catch (std::logic_error& e)
      {
        // OK
      }
      draw_outline(
        r_border,
        game_resources::get().get_board_game_textures().get_outline_color(statistic),
        1
      );
    }
  }
}

void draw_navigation_controls(game_view& view)
{
  for (const auto s: get_all_sides())
  {
    draw_navigation_controls(
      view.get_layout().get_navigation_controls(s),
      s
    );
  }

}

void show_debug(game_view& view, const side player_side)
{
  const auto& g{view.get_game()};
  const auto& c{view.get_game_controller()};
  const auto& layout{view.get_layout()};
  sf::Text text;
  text.setFont(game_resources::get().get_fonts().get_arial_font());
  const piece& closest_piece{
    get_closest_piece_to(g, get_cursor_pos(c, player_side))
  };

  std::vector<sf::String> texts =
  {
    "Game position: " + to_notation(get_cursor_pos(c, player_side)),
    "Screen position: " + to_str(convert_to_screen_coordinate(get_cursor_pos(c, player_side), layout)),
    "Cursor position: " + to_str(get_cursor_pos(c, player_side)),
    "Closest piece: " + to_str(closest_piece.get_type()) + ": " + to_str(to_coordinat(closest_piece.get_current_square()))
  };
  draw_texts(
    texts,
    layout.get_debug(player_side),
    20,
    sf::Color::White
  );
}

void game_view::show_mouse_cursor()
{
  const auto& layout = m_layout;

  sf::CircleShape cursor;
  cursor.setRadius(16.0);
  cursor.setFillColor(sf::Color::Transparent);
  cursor.setOutlineColor(sf::Color::Black);
  cursor.setOutlineThickness(2.0);
  cursor.setScale(1.0, 1.0);
  cursor.setOrigin(16.0, 16.0);
  const screen_coordinate cursor_pos{
    convert_to_screen_coordinate(
      get_cursor_pos(m_game_controller, side::rhs),
      layout
    )
  };
  cursor.setPosition(
    cursor_pos.get_x(),
    cursor_pos.get_y()
  );
  get_render_window().draw(cursor);
}

void show_layout(game_view& view)
{
  const auto& layout{view.get_layout()};
  for (const auto& panel: get_panels(layout))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, panel);
    rectangle.setFillColor(sf::Color(0, 0, 0, 128));
    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(sf::Color::White);
    get_render_window().draw(rectangle);
  }
}

void draw_log(game_view& view, const side player)
{
  const auto& layout = view.get_layout();
  sf::Text text;
  text.setFont(game_resources::get().get_fonts().get_arial_font());
  std::stringstream s;
  s << get_last_log_messages(view, player);
  text.setString(s.str().c_str());
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_log(player).get_tl().get_x(),
    layout.get_log(player).get_tl().get_y()
  );
  get_render_window().draw(text);
}

void draw_background(game_view& view)
{
  const auto& layout{view.get_layout()};
  draw_texture(
    get_map_texture(get_race_of_color(chess_color::white)),
    screen_rect(layout.get_background())
  );
}


void show_occupied_squares(game_view& view)
{
  assert(game_options::get().get_show_occupied());
  const auto& pieces{get_pieces(view)};
  for (const auto& piece: pieces)
  {
    const auto& square{get_occupied_square(piece)};
    const screen_rect square_rect{
      convert_to_screen_rect(
        game_rect{to_game_rect(square)},
        get_layout(view)
      )
    };
    sf::RectangleShape s;
    set_rect(s, square_rect);
    s.setTexture(
      &game_resources::get().get_textures().get_occupied_square(
        to_color(square),
        piece.get_color()
      )
    );
    get_render_window().draw(s);
  }
}

void draw_pieces(game_view& view)
{
  const auto& game{view.get_game()};
  const board_layout layout(view.get_layout().get_board());
  for (const auto& piece: game.get_pieces())
  {
    const int x{piece.get_current_square().get_x()};
    const int y{piece.get_current_square().get_y()};
    const square_layout& square_layout(layout.get_square(x, y));

    // Transparency effect when moving
    sf::Color fill_color = sf::Color::Transparent;
    if (!piece.get_actions().empty()
      && piece.get_actions()[0].get_action_type() == piece_action_type::move
    )
    {
      const double f{piece.get_current_action_progress().get()};
      int alpha{0};
      if (f < 0.5)
      {
        alpha = 255 - static_cast<int>(f * 255.0);
      }
      else
      {
        alpha = static_cast<int>(f * 255.0);
      }
      fill_color = sf::Color(255, 255, 255, alpha);
    }

    const screen_rect sprite_rect(square_layout.get_piece());
    draw_texture(
      get_piece_texture(
        piece.get_race(),
        piece.get_color(),
        piece.get_type()
      ),
      sprite_rect,
      fill_color
    );

    // Show the piece is proteced
    const bool is_piece_protected{is_square_protected(game.get_pieces(), piece.get_current_square(), piece.get_color())};
    if (is_piece_protected)
    {
      draw_texture(
        game_resources::get().get_board_game_textures().get_shield(),
        square_layout.get_is_protected(),
        sf::Color::Blue
      );
    }
  }
}

void draw_possible_moves(game_view& view)
{
  const auto& g{view.get_game()};
  const auto& layout{view.get_layout()};
  const auto actions{collect_all_piece_actions(g)};
  for (const auto& action: actions)
  {
    if (!get_piece_at(g, action.get_from()).is_selected()) continue;
    sf::RectangleShape rectangle;
    set_rect(
      rectangle,
      convert_to_screen_rect(action.get_to(), layout)
    );
    rectangle.setOutlineColor(
      to_sfml_color(
        action.get_color(),
        action.get_action_type()
      )
    );
    rectangle.setOutlineThickness(get_square_width(layout) / 10.0);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setScale(0.5, 0.5);
    rectangle.setRotation(action.get_color() == chess_color::white ? 30 : -30);
    get_render_window().draw(rectangle);
  }
}

void show_sidebar(game_view& view, const side player_side)
{
  draw_unit_info(view, player_side);
  draw_controls(view, player_side);
  draw_log(view, player_side);
  if (game_options::get().get_show_debug_info()) show_debug(view, player_side);
}

void draw_squares(game_view& view)
{
  draw_squares(view.get_layout().get_board().get_board());
}

void draw_square_under_cursor(
  game_view& view,
  const side player
)
{
  const auto& c{view.get_game_controller()};
  const auto& layout{view.get_layout()};
  const int x{
    static_cast<int>(std::trunc(get_cursor_pos(c, player).get_x()))
  };
  if (x < 0 || x >= 8) return;
  const int y{
    static_cast<int>(std::trunc(get_cursor_pos(c, player).get_y()))
  };
  if (y < 0 || y >= 8) return;

  assert(x >= 0 && x < 8 && y >= 0 && y < 8);
  sf::RectangleShape s;
  const double square_width{get_square_width(layout)};
  const double square_height{get_square_height(layout)};
  s.setSize(sf::Vector2f(square_width + 1, square_height + 1));
  s.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));
  const screen_coordinate square_pos{
    convert_to_screen_coordinate(
      game_coordinate(x + 0.5, y + 0.5),
      layout
    )
  };
  s.setPosition(square_pos.get_x(), square_pos.get_y());
  const auto old_fill_color = s.getFillColor();
  const auto old_outline_color = s.getOutlineColor();
  const auto old_thickness = s.getOutlineThickness();
  const auto player_color{get_color(player)};
  s.setOutlineColor(to_sfml_color(player_color));
  s.setFillColor(sf::Color::Transparent);
  const bool valid{would_be_valid(view, player_color)};
  if (valid)
  {
    s.setOutlineThickness(std::max(4, static_cast<int>(square_height / 10)));
  }
  else
  {
    s.setOutlineThickness(std::max(2, static_cast<int>(square_height / 20)));
  }
  get_render_window().draw(s);
  s.setFillColor(old_fill_color);
  s.setOutlineColor(old_outline_color);
  s.setOutlineThickness(old_thickness);
}

void draw_unit_health_bars(game_view& view)
{
  const auto& game{view.get_game()};
  const board_layout bl(view.get_layout().get_board());

  for (const auto& piece: game.get_pieces())
  {
    const auto& square_layout{
      bl.get_square(
        piece.get_current_square().get_x(),
        piece.get_current_square().get_y()
      )
    };
    const auto outline_rect{
      square_layout.get_health_bar_outline()
    };

    // Black background
    draw_rectangle(
      outline_rect,
      sf::Color::Black
    );


    // Bar
    try
    {
      const double f_health{get_f_health(piece)};
      const auto bar_rect{square_layout.get_health_bar_value(f_health)};
      const sf::Color health_color{f_health_to_color(get_f_health(piece))};
      draw_rectangle(
        bar_rect,
        health_color
      );
    }
    catch (std::logic_error&)
    {
      // OK
    }
  }
}

void draw_unit_paths(game_view& view)
{
  const auto& game{view.get_game()};
  const auto& layout{view.get_layout()};
  for (const auto& piece: get_pieces(game))
  {
    if (is_idle(piece)) continue;
    const auto& actions{piece.get_actions()};
    for (const auto& action: actions)
    {
      const auto from_pixel{
        convert_to_screen_coordinate(
          to_coordinat(action.get_from()),
          layout
        )
      };
      const auto to_pixel{
        convert_to_screen_coordinate(
          to_coordinat(action.get_to()),
          layout
        )
      };
      const auto center_pixel{(to_pixel + from_pixel) / 2.0};
      const double length{calc_distance(from_pixel, to_pixel)};
      const double angle_degrees{calc_angle_degrees(center_pixel, to_pixel)};
      sf::RectangleShape rect;
      const double max_square_height{get_square_width(layout)};
      const double height{std::max(2.0, max_square_height * 0.05)};
      rect.setSize(sf::Vector2f(length, height));
      rect.setOrigin(length / 2, height / 2);
      rect.setPosition(
        center_pixel.get_x(),
        center_pixel.get_y()
      );
      rect.rotate(-angle_degrees);
      rect.setOutlineColor(to_sfml_color(get_other_color(piece.get_color())));
      rect.setOutlineThickness(2);
      rect.setFillColor(to_sfml_color(piece.get_color()));
      get_render_window().draw(rect);
    }



    // Collect all the coordinats for the path
    std::vector<screen_coordinate> coordinats;
    coordinats.reserve(piece.get_actions().size() + 1); // +1 for current position
    coordinats.push_back(
      convert_to_screen_coordinate(
        to_coordinat(piece.get_current_square()),
        layout
      )
    );
    std::transform(
      std::begin(actions),
      std::end(actions),
      std::back_inserter(coordinats),
      [layout](const auto& user_input)
      {
        return convert_to_screen_coordinate(
          to_coordinat(user_input.get_to()),
          layout
        );
      }
    );

    // Draw circles at the subgoals
    sf::CircleShape circle;
    for (const auto coordinat: coordinats)
    {
      const double full_diameter{get_square_width(layout)};
      const double diameter{0.25 * full_diameter};
      const double radius{diameter / 2.0};
      const double half_radius{radius/ 2.0};
      circle.setPosition(
        sf::Vector2f(
          coordinat.get_x() - half_radius,
          coordinat.get_y() - half_radius
        )
      );
      circle.setFillColor(to_sfml_color(piece.get_color()));
      circle.setRadius(radius);
      circle.setOrigin(half_radius, half_radius);
      get_render_window().draw(circle);
    }

    // Draw circle at current in-progress movement
    assert(!actions.empty());
    const auto& first_action{actions[0]};
    if (first_action.get_action_type() == piece_action_type::move)
    {
      const auto from_pixel{
        convert_to_screen_coordinate(
          to_coordinat(first_action.get_from()),
          layout
        )
      };
      const auto to_pixel{
        convert_to_screen_coordinate(
          to_coordinat(first_action.get_to()),
          layout
        )
      };
      const auto f{piece.get_current_action_progress().get()};
      assert(f >= 0.0);
      assert(f <= 1.0);
      const auto delta_pixel{to_pixel - from_pixel};
      const auto now_pixel{from_pixel + (delta_pixel * f)};

      const double full_diameter{get_square_width(layout)};
      const double diameter{0.25 * full_diameter};
      const double radius{diameter / 2.0};
      const double half_radius{radius/ 2.0};
      circle.setPosition(
        sf::Vector2f(
          now_pixel.get_x() - half_radius,
          now_pixel.get_y() - half_radius
        )
      );
      circle.setOutlineColor(to_sfml_color(get_other_color(piece.get_color())));
      circle.setOutlineThickness(std::max(2.0, radius / 10.0));
      circle.setFillColor(to_sfml_color(piece.get_color()));
      circle.setRadius(radius);
      circle.setOrigin(half_radius, half_radius);
      get_render_window().draw(circle);
    }
  }
}

void draw_unit_info(game_view& view, const side player_side)
{
  const auto& layout{view.get_layout()};
  const auto& r{layout.get_unit_info(player_side)};
  const auto player_color{get_player_color(player_side)};

  const auto selected_pieces{get_selected_pieces(view.get_game(), player_color)};
  if (selected_pieces.empty()) return;
  assert(selected_pieces.size() == 1);
  const auto& piece{selected_pieces[0]};

  // sprite of the piece
  draw_texture(get_piece_portrait_texture(
      piece.get_race(),
      piece.get_color(),
      piece.get_type()
    ),
    r
  );
  // text
  std::stringstream s;
  s << piece.get_type() << ": "
    << piece.get_health() << "/"
    << piece.get_max_health() << ", "
    << piece.get_current_square() << ", "
    << describe_actions(piece)
  ;
  const auto text_rect{
    screen_rect(
      screen_coordinate(r.get_tl().get_x(), r.get_br().get_y() - 32),
      screen_coordinate(r.get_br())
    )
  };
  draw_rectangle(text_rect, sf::Color(128, 128, 128, 128));
  draw_text(s.str(), text_rect, 20);
}

void game_view::start_impl()
{
  m_clock.restart();
  game_resources::get().get_songs().get_wonderful_time().setVolume(
    get_music_volume_as_percentage()
  );
  game_resources::get().get_songs().get_wonderful_time().play();
  game_resources::get().get_sound_effects().set_master_volume(
    game_options::get().get_sound_effects_volume()
  );
  m_game = game();
  assert(!is_active());
  set_is_active(true);
}

void game_view::stop_impl()
{
  assert(is_active());
  m_clock.restart();
  game_resources::get().get_songs().get_wonderful_time().stop();
  clear_next_state();
  set_is_active(false);
}

void test_game_view() //!OCLINT tests may be many
{
  #ifndef NDEBUG // no tests in release
  #endif //NDEBUG
}

bool would_be_valid(
  const game_view& view,
  chess_color player_color
)
{
  const auto& game{view.get_game()};
  const auto& c{view.get_game_controller()};
  return can_player_select_piece_at_cursor_pos(game, c, player_color);
}

#endif // LOGIC_ONLY
