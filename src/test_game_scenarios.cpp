#include "test_game.h"

#include "game.h"
#include "fen_string.h"

#include <cassert>

#ifdef GAME_CONTROLLER_H
#error 'game' must know nothing about 'game_controller'
#endif

/// Test scenarios expressed in chess notation
void test_game_scenarios()
{
#ifndef NDEBUG // no tests in release
  // Game without a white king is won by black
  {
    const game g = create_game_from_fen_string(get_fen_string_game_over_white_no_king());
    assert(g.get_winner().has_value());
    assert(g.get_winner().value() == chess_color::black);
  }
  // Game without a black king is won by white
  {
    const game g = create_game_from_fen_string(get_fen_string_game_over_black_no_king());
    assert(g.get_winner().has_value());
    assert(g.get_winner().value() == chess_color::white);
  }
  // Game where white is checkmated is won by black
  {
    const game g = create_game_from_fen_string(get_fen_string_game_over_white_checkmate());
    assert(g.get_winner().has_value());
    assert(g.get_winner().value() == chess_color::black);
  }
  // Game where black is checkmated is won by white
  {
    const game g = create_game_from_fen_string(get_fen_string_game_over_black_checkmate());
    assert(g.get_winner().has_value());
    assert(g.get_winner().value() == chess_color::white);
  }
#endif // NDEBUG // no tests in release
}
