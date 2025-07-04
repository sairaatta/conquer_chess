#include "game_statistics.h"

#include "game.h"
#include "lobby_options.h"
#include "pieces.h"
#include "game_controller.h"

#include <cassert>

game_statistics::game_statistics(const game_controller& c)
{
  for (const side s: get_all_sides())
  {
    m_statistics[game_statistic_type::time][s] = get_in_game_time(c).get();
    m_statistics[game_statistic_type::value][s] = get_f_value(c.get_game().get_pieces(), c.get_lobby_options().get_color(s));
    m_statistics[game_statistic_type::activity][s] = get_f_active(c.get_game().get_pieces(), c.get_lobby_options().get_color(s));
    m_statistics[game_statistic_type::protectedness][s] = get_f_protected(c.get_game().get_pieces(), c.get_lobby_options().get_color(s));
  }
}

game_statistics::game_statistics(const std::map<game_statistic_type, std::map<side, double>>& s)
  : m_statistics{s}
{
  // Check if all relevant values are in
  // assert(!flatten_to_row(s).empty());
}

double game_statistics::calc_relative(const game_statistic_type s) const
{
  const double lhs{m_statistics.at(s).at(side::lhs)};
  const double rhs{m_statistics.at(s).at(side::rhs)};
  const double f{
     lhs + rhs == 0
      ? 0.5
      : static_cast<double>(lhs) / static_cast<double>(lhs + rhs)
  };
  assert(f >= 0.0);
  assert(f <= 1.0);
  return f;
}

game_statistics create_test_game_statistics()
{
  std::map<game_statistic_type, std::map<side, double>> s;
  s[game_statistic_type::time][side::lhs] = 0.0;
  s[game_statistic_type::value][side::lhs] = 1.0;
  s[game_statistic_type::value][side::rhs] = 1.0;
  s[game_statistic_type::activity][side::lhs] = 0.0;
  s[game_statistic_type::activity][side::rhs] = 0.0;
  s[game_statistic_type::protectedness][side::lhs] = 14.0 / 16.0;
  s[game_statistic_type::protectedness][side::rhs] = 14.0 / 16.0;
  return game_statistics(s);
}

std::vector<double> flatten_to_row(const game_statistics& s)
{
  return
  {
    s.get(game_statistic_type::time, side::lhs),
    s.get(game_statistic_type::value, side::lhs),
    s.get(game_statistic_type::value, side::rhs),
    s.get(game_statistic_type::activity, side::lhs),
    s.get(game_statistic_type::activity, side::rhs),
    s.get(game_statistic_type::protectedness, side::lhs),
    s.get(game_statistic_type::protectedness, side::rhs)
  };
}

double game_statistics::get(const game_statistic_type stat, const side s) const
{
  return m_statistics.at(stat).at(s);
}

std::vector<std::string> get_column_headers() noexcept
{
  return {
    "time",
    "value_lhs",
    "value_rhs",
    "activity_lhs",
    "activity_rhs",
    "protectedness_lhs",
    "protectedness_rhs"
  };
}

double get_f_active(
  const std::vector<piece>& pieces,
  const chess_color c
)
{
  int n_total{0};
  int n_active{0};
  for (const auto& p: pieces)
  {
    if (p.get_color() != c) continue;
    ++n_total;
    if (!p.get_actions().empty()) ++n_active;
  }
  assert(n_total > 0);
  return static_cast<double>(n_active) / static_cast<double>(n_total);
}

/// Get the fraction of pieces that is protected by another
double get_f_protected(
  const std::vector<piece>& pieces,
  const chess_color c
)
{
  int n_total{0};
  int n_protected{0};
  for (const auto& p: pieces)
  {
    if (p.get_color() != c) continue;
    ++n_total;
    if (is_square_protected(pieces, p.get_current_square(), c)) ++n_protected;
  }
  assert(n_total > 0);
  return static_cast<double>(n_protected) / static_cast<double>(n_total);
}

double get_f_value(
  const std::vector<piece>& pieces,
  const chess_color c
)
{
  const int sum_value{get_total_pieces_value(pieces, c)};
  const int max_value{get_max_pieces_value()};
  assert(max_value == 40);
  return static_cast<double>(sum_value) / static_cast<double>(max_value);
}


void test_game_statistics()
{
#ifndef NDEBUG
  // Calc relative
  {
    game_controller c{
      game(get_pieces_bishop_and_knight_end_game()),
      lobby_options()
    };
    //const game_controller c{create_game_controller_with_two_keyboards(create_game_with_starting_position(starting_position_type::bishop_and_knight_end_game))};
    const game_statistics s(c);
    assert(s.calc_relative(game_statistic_type::value) == 1.0); // Black has no pieces
    assert(s.calc_relative(game_statistic_type::activity) == 0.5);
  }
  // get_column_headers and flatten_to_row have an equal amount of elements
  {
    const game_controller c;
    const game_statistics s(c);
    const auto values{flatten_to_row(s)};
    const auto headers{get_column_headers()};
    assert(values.size() == headers.size());
  }
  // get_column_headers
  {
    const auto headers{get_column_headers()};
    assert(!headers.empty());
  }
  // get_f_active
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(get_f_active(pieces, chess_color::white) == 0.0);

  }
  // get_f_protected
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(get_f_protected(pieces, chess_color::white) > 0.0);

  }
  // get_f_value
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(get_f_value(pieces, chess_color::white) == 1.0);
  }
#endif
}
