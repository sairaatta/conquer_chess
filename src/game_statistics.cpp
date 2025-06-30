#include "game_statistics.h"

#include "game.h"
#include "lobby_options.h"
#include "pieces.h"

#include <cassert>

game_statistics::game_statistics(const game& g)
{
  for (const side s: get_all_sides())
  {
    m_statistics[game_info_statistic::value][s] = get_f_value(g.get_pieces(), get_color(s));
    m_statistics[game_info_statistic::activity][s] = get_f_active(g.get_pieces(), get_color(s));
    m_statistics[game_info_statistic::protectedness][s] = get_f_protected(g.get_pieces(), get_color(s));
  }
}

double game_statistics::calc_relative(const game_info_statistic s) const
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

double game_statistics::get(const game_info_statistic stat, const side s) const
{
  return m_statistics.at(stat).at(s);
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
