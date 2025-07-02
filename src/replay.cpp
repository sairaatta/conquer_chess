#include "replay.h"

#include "helper.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

replay::replay(const pgn_string& pgn_str)
{
  const std::string s{pgn_str.get()};
  if (s.empty()) return;

  assert(!s.empty());
  assert(std::count(std::begin(s), std::end(s), '\n') == 0);
  const std::vector<std::string> pgn_moves{
    split_pgn_str(s)
  };

  m_moves.reserve(pgn_moves.size());
  chess_color color{chess_color::white};
  for (const auto& pgn_move: pgn_moves)
  {
    m_moves.push_back(chess_move(pgn_move, color));
    color = get_other_color(color);
  }
}

int get_n_moves(const replay& r) noexcept
{
  return static_cast<int>(r.get_moves().size());
}



void test_replay()
{
#ifndef NDEBUG
  // replay::replay from empty string is OK
  {
    const replay r(pgn_string(""));
    assert(get_n_moves(r) == 0);
  }
  // replay::replay from one move
  {
    const replay r(pgn_string("1. e4"));
    assert(get_n_moves(r) == 1);
  }
  // replay::replay from two moves move
  {
    const replay r(pgn_string("1. e4 e5"));
    assert(get_n_moves(r) == 2);
  }
  // replay::replay from get_scholars_mate_as_pgn_str
  {
    const replay r(get_scholars_mate_as_pgn_str());
    assert(get_n_moves(r) == 8);
  }
  // replay::replay from get_replay_1_as_pgn_str
  {
    const replay r(get_replay_1_as_pgn_str());
    assert(get_n_moves(r) > 8);
  }
  // 45: operator<<
  {
    const replay r(get_replay_1_as_pgn_str());
    std::stringstream s;
    s << r;
    assert(!s.str().empty());
  }
  // operator==
  {
    const replay r(get_replay_1_as_pgn_str());
    const replay s(get_replay_1_as_pgn_str());
    const replay t(get_scholars_mate_as_pgn_str());
    assert(r == s);
    assert(!(r == t));
  }
#endif // NDEBUG
}

bool operator==(const replay& lhs, const replay& rhs) noexcept
{
  return lhs.get_moves() == rhs.get_moves();
}

std::ostream& operator<<(std::ostream& os, const replay& r) noexcept
{
  std::stringstream s;
  const int n = r.get_moves().size();
  for (int i{0}; i!=n; ++i)
  {
    s << i << ": " << r.get_moves()[i] << ", ";
  }
  std::string t{s.str()};
  if (t.empty()) return os;
  assert(!t.empty());
  t.pop_back();
  assert(!t.empty());
  t.pop_back();
  os << t;
  return os;
}
