#include "replay_view.h"

#ifndef LOGIC_ONLY

#include "render_window.h"
#include "game_resources.h"
#include "sfml_helper.h"
#include "draw.h"
#include "draw_board.h"
#include "game.h"
#include "game_controller.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

// From https://github.com/jerr-it/SFGraphing
#include "../SFGraphing/include/SFGraphing/SFPlot.h"

replay_view::replay_view()
  : m_replayer{get_played_scholars_mate()}
{
  m_controls_bar.set_draw_player_controls(false);

  m_statistics = extract_game_statistics_in_time(m_replayer, delta_t(0.2));
}

bool replay_view::process_event_impl(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
    set_next_state(program_state::main_menu);
    return false;
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      set_next_state(program_state::main_menu);
      return false;
    }
    else if (key_pressed == sf::Keyboard::Key::F2)
    {
      std::ofstream file("replay.pgn");
      file << to_pgn(m_replayer.get_game()) << '\n';
      play_sound_effect(message(message_type::done, chess_color::white, piece_type::king));
    }
  }
  return false; // Do not close the window :-)
}

void replay_view::process_resize_event_impl(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  const screen_coordinate br(event.size.width, event.size.height);

  const screen_rect window_rect(screen_coordinate(0,0), br);
  m_layout = replay_view_layout(window_rect);
  m_controls_bar.set_screen_rect(create_controls_bar_area(br));
}


void replay_view::draw_impl()
{
  draw_board(*this);
  draw_statistics(*this);
  m_controls_bar.draw();
}

void replay_view::start_impl()
{
  assert(!is_active());
  set_is_active(true);
  m_replayer = get_played_scholars_mate();
}

void replay_view::stop_impl()
{
  assert(is_active());
  clear_next_state();
  set_is_active(false);
}

void replay_view::tick_impl(const delta_t dt)
{
  assert(dt.get() > 0.0);
}

void draw_board(replay_view& v)
{
  const auto& g{v.get_game()};
  const board_layout& layout{v.get_layout().get_board()};

  const bool semi_transparent{false};
  draw_squares(layout, semi_transparent);
  //TODO: draw_unit_paths(layout);
  draw_pieces(g.get_pieces(), layout);

  draw_unit_health_bars(g, layout);
  //assert(!"TODO");
}

void draw_statistics(replay_view& v)
{
  const auto screen_rect{v.get_layout().get_statistics()};

  std::vector<float> times;
  std::vector<float> lhs_piece_values;
  std::vector<float> rhs_piece_values;
  std::vector<float> lhs_activity;
  std::vector<float> rhs_activity;
  std::vector<float> lhs_protectedness;
  std::vector<float> rhs_protectedness;
  for (const auto& s: v.get_statistics().get())
  {
    times.push_back(s.get(game_statistic_type::time, side::lhs));
    lhs_piece_values.push_back(s.get(game_statistic_type::value, side::lhs));
    rhs_piece_values.push_back(s.get(game_statistic_type::value, side::rhs));
    lhs_activity.push_back(s.get(game_statistic_type::activity, side::lhs));
    rhs_activity.push_back(s.get(game_statistic_type::activity, side::rhs));
    lhs_protectedness.push_back(s.get(game_statistic_type::protectedness, side::lhs));
    rhs_protectedness.push_back(s.get(game_statistic_type::protectedness, side::rhs));
  }

  csrc::PlotDataSet lhs_piece_values_in_time(times, lhs_piece_values, sf::Color(255, 128, 128), "LHS piece value", csrc::PlottingType::LINE);
  csrc::PlotDataSet rhs_piece_values_in_time(times, rhs_piece_values, sf::Color(255, 0, 0), "RHS piece value", csrc::PlottingType::LINE);
  csrc::PlotDataSet lhs_activity_in_time(times, lhs_activity, sf::Color(128, 255, 128), "LHS activity", csrc::PlottingType::LINE);
  csrc::PlotDataSet rhs_activity_in_time(times, rhs_activity, sf::Color(0, 255, 0), "RHS activity", csrc::PlottingType::LINE);
  csrc::PlotDataSet lhs_protectedness_in_time(times, lhs_protectedness, sf::Color(128, 128, 255), "LHS protectedness", csrc::PlottingType::LINE);
  csrc::PlotDataSet rhs_protectedness_in_time(times, rhs_protectedness, sf::Color(0, 0, 255), "RHS protectedness", csrc::PlottingType::LINE);

  //Position, dimension, margin, font
  csrc::SFPlot plot(
    sf::Vector2f(screen_rect.get_tl().get_x(), screen_rect.get_tl().get_y()),
    sf::Vector2f(get_width(screen_rect), get_height(screen_rect)),
    32,
    get_arial_font(),
    "Time (chess moves)",
    "Fraction"
  );
  plot.AddDataSet(lhs_piece_values_in_time);
  plot.AddDataSet(rhs_piece_values_in_time);
  plot.AddDataSet(lhs_activity_in_time);
  plot.AddDataSet(rhs_activity_in_time);
  plot.AddDataSet(lhs_protectedness_in_time);
  plot.AddDataSet(rhs_protectedness_in_time);

  //x-minimum, x-maximum, y-minimum, y-maximum, x-step-size, y-step-size, Color of axes
  const double xmin{0.0};
  const double xmax{times.back()};
  const double ymin{0.0};
  const double ymax{1.0};
  const double tick_x{1.0};
  const double tick_y{1.0};
  plot.SetupAxes(xmin, xmax, ymin, ymax, tick_x, tick_y, sf::Color::White);
  plot.GenerateVertices();
  get_render_window().draw(plot);
}

#endif // LOGIC_ONLY
