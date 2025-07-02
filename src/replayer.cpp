#include "replayer.h"

#include "game.h"
#include "game_controller.h"
#include "pieces.h"
#include "pgn_game_string.h"
#include <cassert>

#include <sstream>

replayer::replayer(
  const action_history& r
) : m_last_time{-1.0},
    m_action_history{r}
{

}


void replayer::do_move(
  game_controller& c,
  game& g
)
{
  // Do one move per chess move
  if (g.get_in_game_time() - m_last_time < delta_t(1.0)) return;

  m_last_time = g.get_in_game_time();

  /*

  const int move_index{static_cast<int>(m_last_time.get())};
  if (move_index >= get_n_moves(m_replay)) return;

  // Do the move
  const auto& move{m_replay.get_moves().at(move_index)};
  const auto inputs{convert_move_to_user_inputs(g, c, move)};
  add_user_inputs(c, inputs);
  */
  c.apply_user_inputs_to_game(g);
  /*
  for (int i=0; i!=4; ++i)
  {
    g.tick(delta_t(0.25));
  }
  */
}

int get_n_moves(const replayer& r) noexcept
{
  return get_n_piece_actions(r.get_action_history());
}

game get_played_scholars_mate()
{
  // Scholar's mate
  // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
  replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0")));
  assert(get_n_moves(r) == 8);
  game g{create_game_with_starting_position(starting_position_type::standard)};
  game_controller c{create_game_controller_with_keyboard_mouse()};

  r.do_move(c, g); // e2-e4
  r.do_move(c, g); // e7-e5
  r.do_move(c, g); // Qd1-h5
  r.do_move(c, g); // Nb8-c6
  r.do_move(c, g); // Bf1-c4
  r.do_move(c, g); // Ng8-f6
  r.do_move(c, g); // Qh5xf7#
  r.do_move(c, g); // 1-0, which is ignored
  assert(is_checkmate(g.get_pieces(), chess_color::black));
  return g;
}

void test_replayer()
{
#ifndef NDEBUG
  // replayer contructor
  {
    const replayer r;
    assert(get_n_moves(r) == 0);
    assert(r.get_last_time() == in_game_time(-1.0));
  }
  // replayer::do_move on empty replay does nothing
  {
    replayer r;
    game g;
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(r.get_last_time() == in_game_time(-1.0));
    r.do_move(c, g);
    assert(r.get_last_time() == in_game_time(0.0));
  }
  // replayer::do_move does not increase last_time in 0.1 interval
  {
    replayer r;
    game g;
    game_controller c{create_game_controller_with_keyboard_mouse()};
    r.do_move(c, g);
    assert(r.get_last_time() == in_game_time(0.0));
    g.tick(delta_t(0.1));
    r.do_move(c, g);
    assert(r.get_last_time() == in_game_time(0.0));
  }
  // replayer can forward a pawn two squares forward
  //#define FIX_REPLAYER_USING_ACTION_HISTORY
  #ifdef FIX_REPLAYER_USING_ACTION_HISTORY
  {
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4")));
    assert(get_n_moves(r) == 1);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("e2")));
    assert(!is_piece_at(g, square("e4")));
    r.do_move(c, g);
    assert(!is_piece_at(g, square("e2")));
    assert(is_piece_at(g, square("e4")));
  }
  // replayer can forward a pawn one square
  {
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e3")));
    assert(get_n_moves(r) == 1);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("e2")));
    assert(!is_piece_at(g, square("e3")));
    r.do_move(c, g);
    assert(!is_piece_at(g, square("e2")));
    assert(is_piece_at(g, square("e3")));
  }
  // replayer can do Na3
  {
    replayer r(create_action_history_from_pgn(pgn_game_string("1. Na3")));
    assert(get_n_moves(r) == 1);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("b1")));
    assert(!is_piece_at(g, square("a3")));
    r.do_move(c, g);
    assert(!is_piece_at(g, square("b1")));
    assert(is_piece_at(g, square("a3")));
  }
  // replayer can do Nc3
  {
    replayer r(create_action_history_from_pgn(pgn_game_string("1. Nc3")));
    assert(get_n_moves(r) == 1);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("b1")));
    assert(!is_piece_at(g, square("c3")));
    r.do_move(c, g);
    assert(!is_piece_at(g, square("b1")));
    assert(is_piece_at(g, square("c3")));
  }
  // replayer can do two moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5")));
    assert(get_n_moves(r) == 2);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(c, g); // e2-e4

    assert(is_piece_at(g, square("e7")));
    assert(!is_piece_at(g, square("e5")));
    r.do_move(c, g); // e7-e5
    assert(!is_piece_at(g, square("e7")));
    assert(is_piece_at(g, square("e5")));
  }
  // replayer can do three moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5")));
    assert(get_n_moves(r) == 3);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(c, g); // e2-e4
    r.do_move(c, g); // e7-e5

    assert(is_piece_at(g, square("d1")));
    assert(!is_piece_at(g, square("h5")));
    r.do_move(c, g); // Qd1-h5
    assert(!is_piece_at(g, square("d1")));
    assert(is_piece_at(g, square("h5")));
  }
  // replayer can do four moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6")));
    assert(get_n_moves(r) == 4);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(c, g); // e2-e4
    r.do_move(c, g); // e7-e5
    r.do_move(c, g); // Qd1-h5

    assert(is_piece_at(g, square("b8")));
    assert(!is_piece_at(g, square("c6")));
    r.do_move(c, g); // Nb8-c6
    assert(!is_piece_at(g, square("b8")));
    assert(is_piece_at(g, square("c6")));
  }
  // replayer can do five moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4")));
    assert(get_n_moves(r) == 5);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(c, g); // e2-e4
    r.do_move(c, g); // e7-e5
    r.do_move(c, g); // Qd1-h5
    r.do_move(c, g); // Nb8-c6

    assert(is_piece_at(g, square("f1")));
    assert(!is_piece_at(g, square("c4")));
    r.do_move(c, g); // Bf1-c4
    assert(!is_piece_at(g, square("f1")));
    assert(is_piece_at(g, square("c4")));
  }
  // replayer can do six moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0

    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6")));
    assert(get_n_moves(r) == 6);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    r.do_move(c, g); // e2-e4
    r.do_move(c, g); // e7-e5
    r.do_move(c, g); // Qd1-h5
    r.do_move(c, g); // Nb8-c6
    r.do_move(c, g); // Bf1-c4

    assert(is_piece_at(g, square("g8")));
    assert(!is_piece_at(g, square("f6")));
    r.do_move(c, g); // Ng8-f6
    assert(!is_piece_at(g, square("g8")));
    assert(is_piece_at(g, square("f6")));
  }
  // replayer can do seven moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7#")));
    assert(get_n_moves(r) == 7);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(c, g); // e2-e4
    r.do_move(c, g); // e7-e5
    r.do_move(c, g); // Qd1-h5
    r.do_move(c, g); // Nb8-c6
    r.do_move(c, g); // Bf1-c4
    r.do_move(c, g); // Ng8-f6

    assert(is_piece_at(g, square("h5")));
    assert(is_piece_at(g, square("f7"))); // The piece to be captures
    r.do_move(c, g); // Qh5xf7#
    assert(!is_piece_at(g, square("h5")));
    assert(is_piece_at(g, square("f7")));
  }
  // replayer can do seven moves with score
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0")));
    assert(get_n_moves(r) == 8);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(c, g); // e2-e4
    r.do_move(c, g); // e7-e5
    r.do_move(c, g); // Qd1-h5
    r.do_move(c, g); // Nb8-c6
    r.do_move(c, g); // Bf1-c4
    r.do_move(c, g); // Ng8-f6
    r.do_move(c, g); // Qh5xf7#
    r.do_move(c, g); // 1-0, which is ignored
  }
  // replayer can do seven moves with score and conclude checkmate
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0")));
    assert(get_n_moves(r) == 8);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(c, g); // e2-e4
    r.do_move(c, g); // e7-e5
    r.do_move(c, g); // Qd1-h5
    r.do_move(c, g); // Nb8-c6
    r.do_move(c, g); // Bf1-c4
    r.do_move(c, g); // Ng8-f6
    r.do_move(c, g); // Qh5xf7#
    r.do_move(c, g); // 1-0, which is ignored
    assert(is_checkmate(g.get_pieces(), chess_color::black));
  }
  // replayer can do fools mate and conclude checkmate
  {
    // Fools's mate
    // 1. f3 e6 2. g4 Qh4#
    replayer r(create_action_history_from_pgn(pgn_game_string("1. f3 e6 2. g4 Qh4#")));
    assert(get_n_moves(r) == 4);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(c, g); // e2-e4
    r.do_move(c, g); // e7-e5
    r.do_move(c, g); // Qd1-h5
    r.do_move(c, g); // Nb8-c6
    assert(is_checkmate(g.get_pieces(), chess_color::white));
    assert(!is_checkmate(g.get_pieces(), chess_color::black));
  }
  // get_played_scholars_mate
  {
    const auto g{get_played_scholars_mate()};
    assert(is_checkmate(g.get_pieces(), chess_color::black));
    assert(g.get_winner().value() == chess_color::white);
  }
  // to_fen_string
  {
    // Scholar's mate
    // 1. e4 c5 Nf3
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 c5 2. Nf3")));
    assert(get_n_moves(r) == 3);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    const auto s1{to_fen_str(g.get_pieces())};
    assert(s1 == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    r.do_move(c, g); // e2-e4

    const auto s2{
      to_fen_str(
        g.get_pieces(),
        chess_color::black,
        "KQkq",
        "e3",
        0,
        1
      )
    };
    assert(s2 == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    r.do_move(c, g); // c5

    const auto s3{
      to_fen_str(
        g.get_pieces(),
        chess_color::white,
        "KQkq",
        "c6",
        0,
        2
      )
    };
    assert(s3 == "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");

    r.do_move(c, g); // Nf3

    const auto s4{
      to_fen_str(
        g.get_pieces(),
        chess_color::black,
        "KQkq",
        "-",
        1,
        2
      )
    };
    assert(s4 == "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
  }
  #endif // FIX_REPLAYER_USING_ACTION_HISTORY

  // operator<<
  {
    const replayer r;
    std::stringstream s;
    s << r;
    assert(!s.str().empty());
  }
  // operator==
  {
    const replayer a;
    const replayer b;
    assert(a == b);
  }
#endif // NDEBUG
}

bool operator==(const replayer& lhs, const replayer& rhs) noexcept
{
  return lhs.get_action_history() == rhs.get_action_history()
    && lhs.get_last_time() == rhs.get_last_time()
  ;
}

std::ostream& operator<<(std::ostream& os, const replayer& r) noexcept
{
  os
    << "Last time: " << r.get_last_time() << '\n'
    << "Action history: " << r.get_action_history()
  ;
  return os;

}

#ifdef UNDERSTAND_THE_PURPOSE_OF_REPLAYER

#endif // UNDERSTAND_THE_PURPOSE_OF_REPLAYER
