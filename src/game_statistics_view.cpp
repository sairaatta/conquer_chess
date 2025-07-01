#include "game_statistics_view.h"

#ifndef LOGIC_ONLY

#include "render_window.h"
#include "game_resources.h"
#include "sfml_helper.h"
//#include "draw.h"
//#include "game.h"
//#include "game_controller.h"
#include <cassert>
#include <cmath>
//#include <fstream>
//#include <sstream>

// From https://github.com/jerr-it/SFGraphing
#include "../SFGraphing/include/SFGraphing/SFPlot.h"

game_statistics_view::game_statistics_view()
{

}

bool game_statistics_view::process_event_impl(sf::Event& event)
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
  }
  return false; // Do not close the window :-)
}

void game_statistics_view::process_resize_event_impl(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  const screen_coordinate br(event.size.width, event.size.height);

  const screen_rect window_rect(screen_coordinate(0,0), br);
  m_layout = game_statistics_view_layout(window_rect);
  m_controls_bar.set_screen_rect(create_controls_bar_area(br));
}


void game_statistics_view::draw_impl()
{
  draw_layout_panels(*this);
  draw_plot_panel(*this);
  m_controls_bar.draw();
}

void draw_layout_panels(game_statistics_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    get_render_window().draw(rectangle);
  }
}

void game_statistics_view::start_impl()
{
  assert(!is_active());
  set_is_active(true);

  m_statistics = play_random_game_to_get_statistics_in_time(1000);
}

void game_statistics_view::stop_impl()
{
  assert(is_active());
  clear_next_state();
  set_is_active(false);
}

void game_statistics_view::tick_impl(const delta_t dt)
{
  assert(dt.get() > 0.0);
}

void draw_plot_panel(game_statistics_view& v)
{
  const auto screen_rect{v.get_layout().get_text()};

  const auto stats{v.get_statistics()};
  std::vector<float> times;
  std::vector<float> lhs_piece_values;
  std::vector<float> rhs_piece_values;
  std::vector<float> lhs_activity;
  std::vector<float> rhs_activity;
  std::vector<float> lhs_protectedness;
  std::vector<float> rhs_protectedness;
  for (const auto& s: stats.get())
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

