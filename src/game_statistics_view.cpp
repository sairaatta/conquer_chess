#include "game_statistics_view.h"

#ifndef LOGIC_ONLY

#ifdef USE_SFGRAPHING
// From https://github.com/jerr-it/SFGraphing
#include "../SFGraphing/include/SFGraphing/SFPlot.h"
#endif

#include "render_window.h"
#include "game_resources.h"
#include "sfml_helper.h"
#include "draw.h"
#include "game.h"
#include "game_controller.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

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
  m_layout = game_statistics_view_layout(
    window_rect,
    get_default_margin_width()
  );
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
  #ifdef USE_SFGRAPHING
  const auto screen_rect{v.get_layout().get_text()};

  std::vector<float> xAxis = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::vector<float> yAxis = {1, 2, 3, 4, 5, 6, 5, 6, 7, 8};
  csrc::PlotDataSet set(xAxis, yAxis, sf::Color::Green, "Green Data", csrc::PlottingType::LINE);

  //Position, dimension, margin, font
  csrc::SFPlot plot(
    sf::Vector2f(screen_rect.get_tl().get_x(), screen_rect.get_tl().get_y()),
    sf::Vector2f(get_width(screen_rect), get_height(screen_rect)),
    50,
    get_arial_font(),
    "X Axis",
    "Y Axis"
  );
  plot.AddDataSet(set);

  //x-minimum, x-maximum, y-minimum, y-maximum, x-step-size, y-step-size, Color of axes
  plot.SetupAxes(0, 10, 0, 10, 1, 1, sf::Color::White);
  plot.GenerateVertices();
  get_render_window().draw(plot);
  #else
  const auto& g{v.get_game()};
  const auto screen_rect{v.get_layout().get_text()};
  std::stringstream s;
  s << to_pgn(g);
  if (s.str().empty()) s << "[none]";
  draw_text(s.str(), screen_rect, 16);
  #endif
}

#endif // LOGIC_ONLY
