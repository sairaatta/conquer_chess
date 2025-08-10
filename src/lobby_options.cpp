#include "lobby_options.h"

#include <cassert>
#include <iostream>
#include <sstream>

lobby_options::lobby_options(
  const chess_color lhs_color,
  const race lhs_race,
  const race rhs_race
)
{
  m_color[side::lhs] = lhs_color;
  m_color[side::rhs] = get_other_color(lhs_color);
  m_race[side::lhs] = lhs_race;
  m_race[side::rhs] = rhs_race;
}

chess_color lobby_options::get_color(const side player_side) const noexcept
{
  return m_color.at(player_side);
}

race lobby_options::get_race(const chess_color player_color) const noexcept
{
  if (get_color(side::lhs) == player_color) return get_race(side::lhs);
  assert(get_color(side::rhs) == player_color);
  return get_race(side::rhs);
}

race lobby_options::get_race(const side player_side) const noexcept
{
  return m_race.at(player_side);
}

side lobby_options::get_side(const chess_color player_color) const noexcept
{
  if (get_color(side::lhs) == player_color) return side::lhs;
  assert(get_color(side::rhs) == player_color);
  return side::rhs;
}

void lobby_options::set_color(const chess_color color, const side player_side) noexcept
{
  m_color[player_side] = color;
  m_color[get_other_side(player_side)] = get_other_color(color);
}

void lobby_options::set_race(const race r, const side player_side) noexcept
{
  m_race[player_side] = r;
}

void test_lobby_options()
{
  #ifndef NDEBUG
  // default construction
  {
    const lobby_options lo;
    assert(lo.get_color(side::lhs) == chess_color::white);
    assert(lo.get_color(side::rhs) == chess_color::black);
    assert(lo.get_race(side::lhs) == race::classic);
    assert(lo.get_race(side::rhs) == race::classic);
    assert(lo.get_race(chess_color::white) == race::classic);
    assert(lo.get_race(chess_color::black) == race::classic);
    assert(lo.get_side(chess_color::white) == side::lhs);
    assert(lo.get_side(chess_color::black) == side::rhs);

  }
  // constructor, complex setting
  {
    const chess_color lhs_color{chess_color::black};
    const race lhs_race{race::keiron};
    const race rhs_race{race::xayid};

    const lobby_options lo(lhs_color, lhs_race, rhs_race);
    assert(lo.get_color(side::lhs) == chess_color::black);
    assert(lo.get_color(side::rhs) == chess_color::white);
    assert(lo.get_race(side::lhs) == race::keiron);
    assert(lo.get_race(side::rhs) == race::xayid);
    assert(lo.get_race(chess_color::white) == race::xayid);
    assert(lo.get_race(chess_color::black) == race::keiron);
    assert(lo.get_side(chess_color::white) == side::rhs);
    assert(lo.get_side(chess_color::black) == side::lhs);
  }
  // get_color and set_color
  {
    lobby_options options;
    options.set_color(chess_color::white, side::lhs);
    assert(options.get_color(side::lhs) == chess_color::white);
    options.set_color(chess_color::black, side::lhs);
    assert(options.get_color(side::lhs) == chess_color::black);
    options.set_color(chess_color::white, side::rhs);
    assert(options.get_color(side::rhs) == chess_color::white);
    options.set_color(chess_color::black, side::rhs);
    assert(options.get_color(side::rhs) == chess_color::black);
  }
  // get_race and set_race
  {
    lobby_options options;
    options.set_race(race::keiron, side::lhs);
    assert(options.get_race(side::lhs) == race::keiron);
    options.set_race(race::xayid, side::lhs);
    assert(options.get_race(side::lhs) == race::xayid);
    options.set_race(race::keiron, side::rhs);
    assert(options.get_race(side::rhs) == race::keiron);
    options.set_race(race::xayid, side::rhs);
    assert(options.get_race(side::rhs) == race::xayid);
  }
  // get_race_of_side
  {
    lobby_options options;
    options.set_race(race::keiron, side::lhs);
    assert(options.get_race(side::lhs) == race::keiron);
    options.set_race(race::xayid, side::lhs);
    assert(options.get_race(side::lhs) == race::xayid);
    options.set_race(race::keiron, side::rhs);
    assert(options.get_race(side::rhs) == race::keiron);
    options.set_race(race::xayid, side::rhs);
    assert(options.get_race(side::rhs) == race::xayid);
  }
  // 76: set_color ensures the other player has the other color
  {
    lobby_options options;
    options.set_color(chess_color::white, side::lhs);
    assert(options.get_color(side::lhs) == chess_color::white);
    assert(options.get_color(side::rhs) == chess_color::black);
    options.set_color(chess_color::black, side::lhs);
    assert(options.get_color(side::lhs) == chess_color::black);
    assert(options.get_color(side::rhs) == chess_color::white);
    options.set_color(chess_color::white, side::rhs);
    assert(options.get_color(side::lhs) == chess_color::black);
    assert(options.get_color(side::rhs) == chess_color::white);
    options.set_color(chess_color::black, side::rhs);
    assert(options.get_color(side::lhs) == chess_color::white);
    assert(options.get_color(side::rhs) == chess_color::black);
  }
  // ::get_color
  {
    lobby_options options;
    assert(options.get_color(side::lhs) == chess_color::white);
    assert(options.get_color(side::rhs) == chess_color::black);
  }
  // get_race_of_color
  {
    lobby_options options;
    assert(options.get_color(side::lhs) == chess_color::white);
    assert(options.get_color(side::rhs) == chess_color::black);
    assert(options.get_race(chess_color::white) == options.get_race(side::lhs));
    assert(options.get_race(chess_color::black) == options.get_race(side::rhs));
    assert(options.get_race(chess_color::white) == options.get_race(side::lhs));
    assert(options.get_race(chess_color::black) == options.get_race(side::rhs));
  }
  // operator<<
  {
    lobby_options options;
    std::stringstream s;
    s << options;
    assert(!s.str().empty());
  }
  #endif
}

std::ostream& operator<<(std::ostream& os, const lobby_options& options) noexcept
{
  os
    << "LHS color: " << options.get_color(side::lhs) << '\n'
    << "RHS color: " << options.get_color(side::rhs) << '\n'
    << "LHS race: " << options.get_race(side::lhs) << '\n'
    << "RHS race: " << options.get_race(side::rhs)
  ;
  return os;
}
