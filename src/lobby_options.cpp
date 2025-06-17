#include "lobby_options.h"

#include <cassert>
#include <iostream>
#include <sstream>

lobby_options::lobby_options(
  const chess_color lhs_color,
  const race lhs_race,
  const race rhs_race
)
  : m_lhs_color{lhs_color},
    m_lhs_race{lhs_race},
    m_rhs_race{rhs_race}
{

}

void use_default_lobby_options() noexcept
{
  lobby_options::get().set_color(chess_color::white, side::lhs);
  lobby_options::get().set_race(race::classic, side::lhs);
  lobby_options::get().set_race(race::classic, side::rhs);
}

chess_color lobby_options::get_color(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_color;
  }
  assert(player_side == side::rhs);
  return get_other_color(m_lhs_color);
}

chess_color get_color(const side player_side) noexcept
{
  const auto& options{lobby_options::get()};
  return options.get_color(player_side);
}

race lobby_options::get_race(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_race;
  }
  assert(player_side == side::rhs);
  return m_rhs_race;
}

race get_race_of_color(const chess_color c) noexcept
{
  const auto& options{lobby_options::get()};
  if (options.get_color(side::lhs) == c) return options.get_race(side::lhs);
  assert(options.get_color(side::rhs) == c);
  return options.get_race(side::rhs);
}

race get_race_of_side(const side player_side) noexcept
{
  return lobby_options::get().get_race(player_side);
}

void lobby_options::set_color(const chess_color color, const side player_side) noexcept
{
  if (player_side == side::lhs)
  {
    m_lhs_color = color;
  }
  else
  {
    assert(player_side == side::rhs);
    m_lhs_color = get_other_color(color);
  }
}

void lobby_options::set_race(const race r, const side player_side) noexcept
{
  if (player_side == side::lhs)
  {
    m_lhs_race = r;
  }
  else
  {
    assert(player_side == side::rhs);
    m_rhs_race = r;
  }
}

void test_lobby_options()
{
  #ifndef NDEBUG
  // get_color and set_color
  {
    use_default_lobby_options();
    auto& options{lobby_options::get()};
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
    use_default_lobby_options();
    auto& options{lobby_options::get()};
    options.set_race(race::protoss, side::lhs);
    assert(options.get_race(side::lhs) == race::protoss);
    options.set_race(race::zerg, side::lhs);
    assert(options.get_race(side::lhs) == race::zerg);
    options.set_race(race::protoss, side::rhs);
    assert(options.get_race(side::rhs) == race::protoss);
    options.set_race(race::zerg, side::rhs);
    assert(options.get_race(side::rhs) == race::zerg);
  }
  // get_race_of_side
  {
    use_default_lobby_options();
    auto& options{lobby_options::get()};
    options.set_race(race::protoss, side::lhs);
    assert(get_race_of_side(side::lhs) == race::protoss);
    options.set_race(race::zerg, side::lhs);
    assert(get_race_of_side(side::lhs) == race::zerg);
    options.set_race(race::protoss, side::rhs);
    assert(get_race_of_side(side::rhs) == race::protoss);
    options.set_race(race::zerg, side::rhs);
    assert(get_race_of_side(side::rhs) == race::zerg);
  }
  // 76: set_color ensures the other player has the other color
  {
    use_default_lobby_options();
    auto& options{lobby_options::get()};
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
    use_default_lobby_options();
    assert(get_color(side::lhs) == chess_color::white);
    assert(get_color(side::rhs) == chess_color::black);
  }
  // get_race_of_color
  {
    use_default_lobby_options();
    auto& options{lobby_options::get()};
    assert(get_color(side::lhs) == chess_color::white);
    assert(get_color(side::rhs) == chess_color::black);
    assert(get_race_of_color(chess_color::white) == options.get_race(side::lhs));
    assert(get_race_of_color(chess_color::black) == options.get_race(side::rhs));
  }
  // operator<<
  {
    use_default_lobby_options();
    auto& options{lobby_options::get()};
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
