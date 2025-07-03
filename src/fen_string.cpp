#include "fen_string.h"
#include "pieces.h"

#include <cassert>
#include <regex>

fen_string::fen_string(const std::string& fen_str)
  : m_fen_str{fen_str}
{

}

fen_string create_fen_string_of_standard_starting_position(
  const chess_color active_player
)
{
  return create_fen_string_with_starting_position(
    starting_position_type::standard,
    active_player
  );
}

fen_string create_fen_string_with_starting_position(
  const starting_position_type t,
  const chess_color active_player
)
{
  const auto pieces{get_starting_pieces(t)};
  return to_fen_str(
    pieces,
    active_player,
    "KQkq",
    "-",
    0,
    1
  );
}


chess_color get_color(const fen_string& s)
{
  if (std::regex_search(s.get(), std::regex(" w "))) return chess_color::white;
  assert(std::regex_search(s.get(), std::regex(" b ")));
  return chess_color::black;
}

/// Get a FEN string for a game that is over: white has no king anymore
fen_string get_fen_string_game_over_white_no_king() noexcept
{
  return fen_string("4k3/8/8/8/8/8/8/8 w - - 0 1");
}

/// Get a FEN string for a game that is over: black has no king anymore
fen_string get_fen_string_game_over_black_no_king() noexcept
{
  return fen_string("8/8/8/8/8/8/8/4K3 b - - 0 1");
}

/// Get a FEN string for a game that is over: white is in checkmate
fen_string get_fen_string_game_over_white_checkmate() noexcept
{
  return fen_string("8/8/8/8/8/4k3/4q3/4K3 w - - 0 1");
}

/// Get a FEN string for a game that is over: black is in checkmate
fen_string get_fen_string_game_over_black_checkmate() noexcept
{
  return fen_string("4k3/4Q3/4K3/8/8/8/8/8 b - - 0 1");
}

fen_string get_fen_string_wikipedia_0() noexcept
{
  return fen_string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

fen_string get_fen_string_wikipedia_1() noexcept
{

  return fen_string("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
}


fen_string get_fen_string_wikipedia_2() noexcept
{

  return fen_string("rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");
}


fen_string get_fen_string_wikipedia_3() noexcept
{
  return fen_string("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
}

void test_fen_string()
{
#ifndef NDEBUG
  // create_fen_str_of_standard_starting_position
  {
    const auto f1{create_fen_string_of_standard_starting_position()};
    const auto f2{get_fen_string_wikipedia_0()};
    assert(f1 == f2);
  }
  // get_color
  {
    assert(get_color(get_fen_string_wikipedia_0()) == chess_color::white);
    assert(get_color(get_fen_string_wikipedia_1()) == chess_color::black);
    assert(get_color(get_fen_string_wikipedia_2()) == chess_color::white);
    assert(get_color(get_fen_string_wikipedia_3()) == chess_color::black);
  }
  // get_fen_string_wikipedia_0
  {
    assert(!get_fen_string_wikipedia_0().get().empty());
  }
  // get_fen_string_wikipedia_1
  {
    assert(!get_fen_string_wikipedia_1().get().empty());
  }
  // get_fen_string_wikipedia_2
  {
    assert(!get_fen_string_wikipedia_2().get().empty());
  }
  // get_fen_string_wikipedia_3
  {
    assert(!get_fen_string_wikipedia_3().get().empty());
  }
#endif
}

bool operator==(const fen_string& lhs, const fen_string& rhs) noexcept
{
  return lhs.get() == rhs.get();
}
