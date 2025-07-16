#include "test_rules.h"

#include "game_controller.h"

#include <cassert>

void test_rule_1_2()
{
#ifndef NDEBUG // no tests in release

  /// [1.2] The player with the light-coloured pieces (White) makes the first move,
  /// then the players move alternately,
  /// with the player with the dark-coloured pieces (Black) making the next move.
  ///
  /// In Conquer Chess, white can make multiple moves during the time of the first
  /// move. After waiting for the duration of one chess move, black can start
  /// doing its many moves.
  {
    game_controller c;

    move_cursor_to(c, "e2", side::lhs);
    move_cursor_to(c, "e7", side::rhs);

    // Can move already
    assert(!get_piece_actions(c, side::lhs).empty());
    // Cannot move yet
    assert(get_piece_actions(c, side::rhs).empty());

    c.tick(delta_t(1.0));

    // Could move already
    assert(!get_piece_actions(c, side::lhs).empty());
    // Can move now
    assert(!get_piece_actions(c, side::rhs).empty());
  }
#endif // NDEBUG // no tests in release
}

void test_rule_1_4_1()
{
#ifndef NDEBUG // no tests in release

  // [1.4.1]
  /// 1.4.1. The player who achieves this goal is said to have ‘checkmated’
  ///   the opponent’s king and to have won the game.
  ///   Leaving one’s own king under attack,
  ///   exposing one’s own king to attack and also
  ///   ’capturing’ the opponent’s king is not allowed.
  ///

  // [1.4.1a]
  // The player who achieves this goal is said to have ‘checkmated’
  //   the opponent’s king and to have won the game.
  {
    game_controller c{
      game(get_pieces_before_scholars_mate())
    };
    assert(is_piece_at(c, square("h5"))); // White queen
    assert(count_selected_units(c, chess_color::white) == 0);
    assert(!is_checkmate(c.get_game().get_pieces(), chess_color::white));
    assert(!is_checkmate(c.get_game().get_pieces(), chess_color::black));
    assert(!has_winner(c));

    // h5xf7#
    do_select(c, "h5", side::lhs); // White queen
    move_cursor_to(c, "f7", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    for (int i{0}; i!=5; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("h5")));
    assert(is_piece_at(c, square("f7")));
    assert(get_piece_at(c, square("f7")).get_type() == piece_type::queen);
    assert(is_checkmate(c.get_game().get_pieces(), chess_color::black));
    assert(!is_checkmate(c.get_game().get_pieces(), chess_color::white));
    assert(has_winner(c));
    assert(get_winner(c).value() == chess_color::white);
  }
  // [1.4.1b]
  // Leaving one’s own king under attack is not allowed.
  #ifdef FIX_1_4_1_B
  {
    game_controller c{
      game(create_game_from_fen_string(fen_string("4k3/8/4q3/8/8/8/8/4K3 w - - 0 1")))
    };

    // King at e1 wants to move to an attacked square
    do_select(c, "e1", side::lhs); // White king
    move_cursor_to(c, "e2", side::lhs); // Attacked square
    assert(count_selected_units(c, chess_color::white) == 1);

    assert(get_piece_actions(c, side::lhs).empty()); // Cannot
  }
  #endif // FIX_1_4_1_B
  #ifdef FIX_1_4_1_C
  // [1.4.1c]
  // Exposing one’s own king to attack is not allowed
  {
    game_controller c{
      game(create_game_from_fen_string(fen_string("4k3/8/4q3/8/8/4Q3/8/4K3 w - - 0 1")))
    };

    // Queen at e3 wants to move away from the pin
    do_select(c, "e3", side::lhs); // White king
    move_cursor_to(c, "a3", side::lhs); // Out of pin
    assert(count_selected_units(c, chess_color::white) == 1);

    assert(get_piece_actions(c, side::lhs).empty()); // Cannot
  }
  #endif // FIX_1_4_1_C
  // [1.4.1d]
  //
  // ’capturing’ the opponent’s king is not allowed in regular chess.
  //
  // In Conquer Chess, 'capturing' the opponent's king is possible
  // (i.e. reducing that king's health to zero)
  // and results in a win.
  {
    game_controller c{
      game(create_game_from_fen_string(fen_string("4k3/8/8/8/4Q3/8/8/4K3 w - - 0 1")))
    };
    assert(is_piece_at(c, square("e4"))); // White queen
    assert(count_selected_units(c, chess_color::white) == 0);
    assert(!is_checkmate(c.get_game().get_pieces(), chess_color::white));
    assert(!is_checkmate(c.get_game().get_pieces(), chess_color::black));
    assert(!has_winner(c));

    // e4 attacks king at e8
    do_select(c, "e4", side::lhs); // White queen
    move_cursor_to(c, "e8", side::lhs); // Black king
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    for (int i{0}; i!=5; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("e4"))); // Queen captured king
    assert(get_piece_at(c, square("e8")).get_type() == piece_type::queen); // White queen
    assert(has_winner(c));
    assert(get_winner(c).value() == chess_color::white);
  }
#endif // NDEBUG // no tests in release

}

void test_rule_1_5()
{
#ifndef NDEBUG // no tests in release
  /// [1.5]. If the position is such that neither player can possibly checkmate
  /// the opponent’s king, the game is drawn (see Article 5.2.2).

  // Position without checkmate possible
  {
    game_controller c{
      game(create_game_from_fen_string(fen_string("4k3/8/8/8/8/8/8/4K3 w - - 0 1")))
    };
    assert(!has_winner(c));
    assert(is_draw(c));

  }
  // Position with checkmate possible
  {
    game_controller c;
    assert(!has_winner(c));
    assert(!is_draw(c));
  }
#endif // NDEBUG // no tests in release
}

void test_rules()
{
#ifndef NDEBUG // no tests in release
  test_rule_1_2();
  test_rule_1_4_1();
  test_rule_1_5();

#endif // NDEBUG // no tests in release
}

