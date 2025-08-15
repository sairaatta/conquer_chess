#include "game_view_layout.h"

#include "action_number.h"
#include "board_layout.h"
#include "helper.h"
#include "game_rect.h"
#include "game_coordinate.h"
#include "square.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <sstream>

game_view_layout::game_view_layout(
  const screen_rect& r
) : m_background{r}
{

  // Vertical
  const int margin_height{get_default_margin_width()};
  const int debug_top_bar_height{24}; // The F3 info oneliners
  const int h{get_height(r)};


  const int controls_height{(2 + 4) * 64}; // 2 for the ASDW, then 1 per action key
  const int unit_info_height{300};
  const int game_info_height{80};

  const int log_panel_height{
    (
      h
      - (5 * margin_height)
      - unit_info_height
      - controls_height
      - game_info_height
      - debug_top_bar_height
    )
    / 2
  };
  const int debug_panel_height{log_panel_height};

  // Width
  const int margin_width{get_default_margin_width()};
  const int w{get_width(r)};

  const int panel_width{300};
  const int board_width_1{
    w - (2 * panel_width) - (4 * margin_width)
  };

  // Height again
  const int board_height_1{
    h
    - (5 * margin_height)
    //- unit_info_height // Here is where it hangs out
    //- controls_height // Here is where it hangs out
    - game_info_height
    - log_panel_height
    - debug_panel_height
    - debug_top_bar_height
  };
  const int board_width{std::min(board_width_1, board_height_1)};
  assert(board_width > 0);
  const int board_height{board_width};
  assert(board_height > 0);
  assert(board_width == board_height);

  const int x1{margin_width};
  const int x2{x1 + panel_width};
  const int x3{x2 + margin_width};
  const int x4{x3 + (board_width / 2) + (margin_width / 2)};
  const int x5{x4 + margin_width};
  const int x6{x5 + (board_width / 2)};
  const int x7{x6 + margin_width};
  const int x8{x7 + panel_width};

  const int y1{debug_top_bar_height + margin_height};
  const int y2{y1 + unit_info_height};
  const int y3{y2 + controls_height};
  const int y4{y3 + margin_height};
  const int y5{y4 + game_info_height};
  const int y6{y5 + margin_width};
  const int y7{y6 + log_panel_height};
  const int y8{y7 + margin_width};
  const int y9{y8 + debug_panel_height};

  // Unit info and controls
  for (const side s: get_all_sides())
  {
    int x_left{0};
    int x_right{0};
    switch (s)
    {
      case side::lhs: x_left = x1; x_right = x2; break;
      case side::rhs: x_left = x7; x_right = x8; break;
    }
    m_unit_info[s] = screen_rect(
      screen_coordinate(x_left, y1),
      screen_coordinate(x_right, y2)
      );
    assert(x_left < x_right);
    assert(y2 < y4);
    m_controls[s] = in_game_controls_layout(
      screen_rect(
        screen_coordinate(x_left, y2),
        screen_coordinate(x_right, y4)
      )
    );
  }

  // Log and debug
  for (const side s: get_all_sides())
  {
    int x_left{0};
    int x_right{0};
    switch (s)
    {
      case side::lhs: x_left = x1; x_right = x4; break;
      case side::rhs: x_left = x5; x_right = x8; break;
    }
    m_log[s] = screen_rect(
      screen_coordinate(x_left, y6),
      screen_coordinate(x_right, y7)
      );
    m_debug[s] = screen_rect(
      screen_coordinate(x_left, y8),
      screen_coordinate(x_right, y9)
    );
  }

  // Board
  m_board = board_layout(
    screen_rect(
      screen_coordinate(x3, y1),
      screen_coordinate(x3 + board_width, y1 + board_height)
    )
  );

  // Game info
  m_game_statistics = screen_rect(
    screen_coordinate(x1, y4),
    screen_coordinate(x8, y5)
  );

  // Must be square
  assert(get_width(m_board.get_board()) == get_height(m_board.get_board()));
  assert(get_board_width(*this) == get_board_height(*this));
  assert(get_square_width(*this) == get_square_height(*this));

}


std::vector<screen_rect> collect_screen_rects(const game_view_layout& layout)
{
  std::vector<screen_rect> v;
  v.push_back(layout.get_background());
  v.push_back(layout.get_board().get_board());
  v.push_back(layout.get_game_info().get_background());
  for (const auto s: get_all_sides())
  {
    v.push_back(layout.get_controls(s).get_background());
    v.push_back(layout.get_debug(s));
    v.push_back(layout.get_log(s));
    v.push_back(layout.get_unit_info(s));
  }
  return v;
}

game_coordinate convert_to_game_coordinate(
  const screen_coordinate& coordinat,
  const game_view_layout& layout
)
{
  // How many pixels on the board
  const int screen_on_board_x{
    coordinat.get_x() - layout.get_board().get_board().get_tl().get_x()
  };
  const int screen_on_board_y{
    coordinat.get_y() - layout.get_board().get_board().get_tl().get_y()
  };
  // Fraction of the board
  const double f_x{
    static_cast<double>(screen_on_board_x) / static_cast<double>(get_board_width(layout))
  };
  const double f_y{
    static_cast<double>(screen_on_board_y) / static_cast<double>(get_board_height(layout))
  };
  return game_coordinate(
    8.0 * f_x,
    8.0 * f_y
  );
}

screen_coordinate convert_to_screen_coordinate(
  const game_coordinate& c,
  const game_view_layout& layout
)
{
  const screen_rect r{layout.get_board().get_board()};
  const double square_width{(1.0 / 8.0) * get_width(r)};
  const double square_height{(1.0 / 8.0) * get_height(r)};
  assert(square_width > 1.0);
  assert(square_height > 1.0);

  const double n_squares_up{c.get_x()}; // This is what it means
  const double n_squares_down{c.get_y()};
  const int pixel_x = r.get_tl().get_x() + (n_squares_up * square_width);
  const int pixel_y = r.get_tl().get_y() + (n_squares_down * square_height);
  const screen_coordinate pixel(pixel_x, pixel_y);
  return pixel;
}

screen_rect convert_to_screen_rect(
  const game_rect& r,
  const game_view_layout& layout
)
{
  const auto& tl{convert_to_screen_coordinate(r.get_tl(), layout)};
  const auto& br{convert_to_screen_coordinate(r.get_br(), layout)};
  return screen_rect(tl, br);
}

screen_rect convert_to_screen_rect(
  const square& s,
  const game_view_layout& layout
)
{
  const game_coordinate tl(s.get_x(), s.get_y());
  const game_coordinate br(s.get_x() + 1, s.get_y() + 1);
  const game_rect rect(tl, br);
  return convert_to_screen_rect(rect, layout);
}

int get_board_height(const game_view_layout& layout) noexcept
{
  return get_height(layout.get_board());
}

int get_board_width(const game_view_layout& layout) noexcept
{
  return get_width(layout.get_board());
}


const in_game_controls_layout& game_view_layout::get_controls(const side player) const noexcept
{
  return m_controls.at(player);
}

const screen_rect& game_view_layout::get_debug(const side player) const noexcept
{
  return m_debug.at(player);
}

const screen_rect& game_view_layout::get_log(const side player) const noexcept
{
  return m_log.at(player);
}

const navigation_controls_layout& game_view_layout::get_navigation_controls(const side player) const noexcept
{

  return m_controls.at(player).get_navigation_controls();
}

double get_square_height(const game_view_layout& layout) noexcept
{
  return static_cast<double>(get_board_height(layout)) / 8.0;
}

double get_square_width(const game_view_layout& layout) noexcept
{
  return static_cast<double>(get_board_width(layout)) / 8.0;
}

const screen_rect& game_view_layout::get_unit_info(const side player) const noexcept
{
  return m_unit_info.at(player);
}

void test_game_view_layout()
{
  #ifndef NDEBUG
  {
    const game_view_layout layout;
    assert(layout.get_board().get_board().get_br().get_x() > 0.0);
    assert(layout.get_board().get_board().get_br().get_y() > 0.0);
    assert(layout.get_board().get_board().get_tl().get_x() > 0.0);
    assert(layout.get_board().get_board().get_tl().get_y() > 0.0);

    assert(get_height(layout.get_game_info()) > 20);
    assert(get_width(layout.get_game_info()) > 20);


    assert(layout.get_debug(side::lhs).get_br().get_x() > 0.0);
    assert(layout.get_debug(side::lhs).get_br().get_y() > 0.0);
    assert(layout.get_unit_info(side::lhs).get_tl().get_x() > 0.0);
    assert(layout.get_unit_info(side::lhs).get_tl().get_y() > 0.0);
    assert(get_board_width(layout) > 0.0);
    assert(get_board_height(layout) > 0.0);
    assert(get_square_width(layout) > 0.0);
    assert(get_square_height(layout) > 0.0);
    assert(get_board_width(layout) == get_board_height(layout));

    assert(get_square_width(layout) == get_square_height(layout));

    // Monitor is wider than high, #138
    assert(get_width(layout.get_background()) > get_height(layout.get_background()));

    // Debug is in screen, #138
    assert(is_in(layout.get_debug(side::lhs), layout.get_background()));

    // All rects within background
    for (const auto r: collect_screen_rects(layout))
    {
      const auto background{layout.get_background()};
      if (r == background) continue;
      assert(is_in(r, background));
    }
  }
  // collect_screen_rects
  {
    const game_view_layout layout;
    const auto v{collect_screen_rects(layout)};
    assert(v.size() == 11);
  }
  // get_controls_key
  {
    const game_view_layout layout;
    for (const auto player: get_all_sides())
    {
      for (const auto& key: get_all_action_numbers())
      {
        assert(layout.get_controls(player).get_action_key_text(key).get_br().get_x() >= 0);
      }
    }
  }
  //--------------------------------------------------------------------------
  // game -> screen
  //--------------------------------------------------------------------------
  // in-game (0,0) must be top-left of screen board
  {
    const game_view_layout layout;
    const auto tl_board = convert_to_screen_coordinate(
      game_coordinate(0.0, 0.0),
      layout
    );

    assert(tl_board.get_x() == layout.get_board().get_board().get_tl().get_x());
    assert(tl_board.get_y() == layout.get_board().get_board().get_tl().get_y());
  }
  // convert_to_screen_rect
  {
    const game_view_layout layout;
    const auto board_rect = convert_to_screen_rect(
      game_rect(
        game_coordinate(0.0, 0.0),
        game_coordinate(8.0, 8.0)
      ),
      layout
    );
    const auto expected{layout.get_board()};
    assert(board_rect == expected.get_board());
  }
  // in-game (8,8) must be bottom-right of screen board
  // (no piece can ever have its top-right at the bottom-right of the board)
  {
    const game_view_layout layout;
    const auto br_board = convert_to_screen_coordinate(
      game_coordinate(8.0, 8.0),
      layout
    );
    assert(br_board.get_x() == layout.get_board().get_board().get_br().get_x());
    assert(br_board.get_y() == layout.get_board().get_board().get_br().get_y());
  }
  //--------------------------------------------------------------------------
  // screen -> game
  //--------------------------------------------------------------------------
  // top-left of screen board must be in-game (0,0)
  // bottom-right of screen board must be in-game (8,8)
  {
    const game_view_layout layout;
    const auto br_board = convert_to_game_coordinate(
      layout.get_board().get_board().get_br(),
      layout
    );
    const auto tl_board = convert_to_game_coordinate(
      layout.get_board().get_board().get_tl(),
      layout
    );
    assert(tl_board.get_x() == 0.0);
    assert(tl_board.get_y() == 0.0);
    assert(br_board.get_x() == 8.0);
    assert(is_close(br_board.get_y(), 8.0, 0.03));
  }
  // convert_to_screen_rect
  {
    const game_view_layout layout;
    const square a1("a1");
    const square h8("h8");
    const screen_rect a1_rect{convert_to_screen_rect(a1, layout)};
    const screen_rect h8_rect{convert_to_screen_rect(h8, layout)};
    assert(a1_rect.get_tl() == layout.get_board().get_board().get_tl());
    assert(h8_rect.get_br() == layout.get_board().get_board().get_br());
  }
  // 39: operator<<
  {
    const game_view_layout layout;
    std::stringstream s;
    s << layout;
    assert(!s.str().empty());
  }
  // get_controls_key_input
  {
    const game_view_layout layout;
    for (const auto& n: get_all_action_numbers())
    {
      for (const auto& s: get_all_sides())
      {
        assert(
          is_in(
            get_center(layout.get_controls(s).get_action_key_symbol(n)),
            layout.get_controls(s).get_background()
          )
        );
        assert(
          is_in(
            get_center(layout.get_controls(s).get_action_key_text(n)),
            layout.get_controls(s).get_background()
          )
        );
      }
    }
  }
  // get_navigation_controls
  {
    const game_view_layout layout;
    assert(layout.get_navigation_controls(side::lhs) != layout.get_navigation_controls(side::rhs));
  }
  // to_wkt
  {
    const game_view_layout layout;
    const std::string wkt{to_wkt(layout)};
    assert(!wkt.empty());
  }
  // to_wkt_file
  {
    const game_view_layout layout;
    const std::string filename{"tmp_game_view_layout.wkt"};
    if (std::filesystem::exists(filename))
    {
      std::filesystem::remove(filename);
    }
    assert(!std::filesystem::exists(filename));
    to_wkt_file(layout, filename);
    assert(std::filesystem::exists(filename));
  }
  // to_wkt_file for 1600x900
  {
    const game_view_layout layout;
    const std::string filename{"tmp_game_view_layout_1600_x_900.wkt"};
    if (std::filesystem::exists(filename))
    {
      std::filesystem::remove(filename);
    }
    assert(!std::filesystem::exists(filename));
    to_wkt_file(layout, filename);
    assert(std::filesystem::exists(filename));
  }
  #endif
}

std::string to_wkt(const game_view_layout& layout)
{
  const auto rs = collect_screen_rects(layout);
  std::stringstream s;
  for (const screen_rect& r: rs)
  {
    s << to_wkt(r) << '\n';
  }
  std::string t{s.str()};
  assert(!t.empty());
  t.pop_back();
  return t;
}

void to_wkt_file(const game_view_layout& layout, const std::string filename)
{
  std::ofstream f(filename);
  f << to_wkt(layout);
}

std::ostream& operator<<(std::ostream& os, const game_view_layout& layout) noexcept
{
  std::stringstream s;
  s
    << "background: " << layout.get_background() << '\n'
    << "board: " << layout.get_board().get_board() << '\n'
  ;
  for (const auto side: get_all_sides())
  {
    s
      << side << " units: " << layout.get_unit_info(side) << '\n'
      << side << " controls: " << layout.get_controls(side) << '\n'
      << side << " log: " << layout.get_log(side) << '\n'
      << side << " debug: " << layout.get_debug(side) << '\n'
    ;
  }
  std::string t{s.str()};
  assert(!t.empty());
  t.pop_back(); // the newline
  os << t;
  return os;
}
