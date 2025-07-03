#include "replayer.h"

#include "game.h"
#include "game_controller.h"
#include "pieces.h"
#include "pgn_game_string.h"
#include <cassert>

#include <sstream>

replayer::replayer(
  const action_history& r,
  const game_controller& c
) :
    m_action_history{r},
    m_game_controller{c},
    m_index{0}
{

}


void replayer::do_move(const delta_t& dt)
{

  const auto& g{m_game_controller.get_game()};

  // All moves are done
  if (m_index == static_cast<int>(m_action_history.get().size()))
  {
    m_game_controller.tick(dt);
    return;
  }

  delta_t dt_to_do(dt);
  while (1)
  {
    assert(m_index < static_cast<int>(m_action_history.get().size()));

    const in_game_time next_action_time{m_action_history.get()[m_index].first};
    if (next_action_time > get_in_game_time(m_game_controller) + dt_to_do)
    {
      // Just pass the time in the timestep, without any action
      // but passing the time
      m_game_controller.tick(dt_to_do);
      return;
    }

    // Forward to the next move
    double forward_time_raw{next_action_time.get() - g.get_in_game_time().get()};
    assert(forward_time_raw >= -0.00000001);
    const delta_t forward_time{std::max(forward_time_raw, 0.0)};
    m_game_controller.tick(forward_time);
    dt_to_do = dt_to_do - forward_time;

    // Make the piece do the action retrieved from history
    const piece_action& next_action{m_action_history.get().at(m_index).second};
    get_piece_at(m_game_controller, next_action.get_from()).add_action(next_action);

    ++m_index;

    if (m_index == static_cast<int>(m_action_history.get().size()))
    {
      // Tick away the last time
      m_game_controller.tick(dt_to_do);
      assert(this->is_done());
      return;
    }
  }
}

game_statistics_in_time extract_game_statistics_in_time(
  const replayer& r_original,
  const delta_t& dt
)
{
  game_statistics_in_time s;
  replayer r(
    r_original.get_action_history(),
    create_game_controller_with_user_settings(create_game_with_user_settings())
  );
  assert(get_in_game_time(r) == in_game_time(0.0));
  while (!r.is_done())
  {
    std::clog << r.get_game().get_in_game_time() << '\n';
    s.add(r.get_game());
    r.do_move(dt);
  }
  return s;
}

const in_game_time& get_in_game_time(
  const replayer& r
) noexcept
{
  return r.get_game().get_in_game_time();
}

int get_n_moves(const replayer& r) noexcept
{
  return get_n_piece_actions(r.get_action_history());
}

replayer get_played_scholars_mate()
{
  // Scholar's mate
  // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
  replayer r(
    create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0")),
    create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::standard))
  );
  assert(get_n_moves(r) == 7);

  r.do_move(); // e2-e4
  r.do_move(); // e7-e5
  r.do_move(); // Qd1-h5
  r.do_move(); // Nb8-c6
  r.do_move(); // Bf1-c4
  r.do_move(); // Ng8-f6
  r.do_move(); // Qh5xf7#
  r.do_move(); // 1-0, which is ignored
  assert(is_checkmate(r.get_game().get_pieces(), chess_color::black));
  return r;
}

bool replayer::is_done() const noexcept
{
  assert(m_index >= 0);
  assert(m_index <= static_cast<int>(m_action_history.get().size()));
  return m_index == static_cast<int>(m_action_history.get().size());
}

bool is_piece_at(const replayer& r, const square& coordinate)
{
  return is_piece_at(r.get_game(), coordinate);
}

bool is_piece_at(const replayer& r, const std::string& square_str)
{
  return is_piece_at(r.get_game(), square_str);
}

void test_replayer()
{
#ifndef NDEBUG
  // replayer contructor
  {
    const replayer r;
    assert(get_n_moves(r) == 0);
    assert(r.get_index() == 0);
  }
  // replayer::do_move on empty replay does nothing
  {
    replayer r;
    //game g;
    //game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(r.get_index() == 0);
    assert(get_in_game_time(r) == in_game_time(0.0));
    r.do_move();
    assert(r.get_index() == 0);
    assert(get_in_game_time(r) == in_game_time(1.0));
  }
  // replayer::do_move does not increase last_time in 0.1 interval
  {
    replayer r;
    //game g;
    //game_controller c{create_game_controller_with_keyboard_mouse()};
    r.do_move();
    assert(r.get_index() == 0);
    r.do_move(delta_t(0.1));
    assert(r.get_index() == 0);
  }
  // replayer can forward a pawn two squares forward
  {
    replayer r(
      create_action_history_from_pgn(pgn_game_string("1. e4")),
      create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::standard))
    );
    assert(get_n_moves(r) == 1);
    assert(is_piece_at(r, square("e2")));
    assert(!is_piece_at(r, square("e4")));
    r.do_move();
    assert(get_in_game_time(r) == in_game_time(1.0));
    assert(!is_piece_at(r, square("e2")));
    assert(is_piece_at(r, square("e4")));
  }
  // replayer can forward a pawn one square
  {
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e3")));
    assert(get_n_moves(r) == 1);
    //game g{create_game_with_starting_position(starting_position_type::standard)};
    //game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(r, square("e2")));
    assert(!is_piece_at(r, square("e3")));
    r.do_move();
    assert(!is_piece_at(r, square("e2")));
    assert(is_piece_at(r, square("e3")));
  }
  // replayer can do Na3
  {
    replayer r(create_action_history_from_pgn(pgn_game_string("1. Na3")));
    assert(get_n_moves(r) == 1);
    //game g{create_game_with_starting_position(starting_position_type::standard)};
    //game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(r, square("b1")));
    assert(!is_piece_at(r, square("a3")));
    r.do_move();
    assert(!is_piece_at(r, square("b1")));
    assert(is_piece_at(r, square("a3")));
  }
  // replayer can do Nc3
  {
    replayer r(create_action_history_from_pgn(pgn_game_string("1. Nc3")));
    assert(get_n_moves(r) == 1);
    //game g{create_game_with_starting_position(starting_position_type::standard)};
    //game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(r, square("b1")));
    assert(!is_piece_at(r, square("c3")));
    r.do_move();
    assert(!is_piece_at(r, square("b1")));
    assert(is_piece_at(r, square("c3")));
  }
  // replayer can do two moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5")));
    assert(get_n_moves(r) == 2);
    //game g{create_game_with_starting_position(starting_position_type::standard)};
    //game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(); // e2-e4

    assert(is_piece_at(r, square("e7")));
    assert(!is_piece_at(r, square("e5")));
    r.do_move(); // e7-e5
    assert(!is_piece_at(r, square("e7")));
    assert(is_piece_at(r, square("e5")));
  }
  // replayer can do three moves
  {
    // Scholar's mate
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5")));
    assert(get_n_moves(r) == 3);

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5

    assert(is_piece_at(r, square("d1")));
    assert(!is_piece_at(r, square("h5")));
    r.do_move(); // Qd1-h5
    assert(!is_piece_at(r, square("d1")));
    assert(is_piece_at(r, square("h5")));
  }
  // replayer can do four moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6")));
    assert(get_n_moves(r) == 4);
    //game g{create_game_with_starting_position(starting_position_type::standard)};
    //game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5

    assert(is_piece_at(r, square("b8")));
    assert(!is_piece_at(r, square("c6")));
    r.do_move(); // Nb8-c6
    assert(!is_piece_at(r, square("b8")));
    assert(is_piece_at(r, square("c6")));
  }
  // replayer can do five moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4")));
    assert(get_n_moves(r) == 5);
    //game g{create_game_with_starting_position(starting_position_type::standard)};
    //game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6

    assert(is_piece_at(r, square("f1")));
    assert(!is_piece_at(r, square("c4")));
    r.do_move(); // Bf1-c4
    assert(!is_piece_at(r, square("f1")));
    assert(is_piece_at(r, square("c4")));
  }
  // replayer can do six moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0

    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6")));
    assert(get_n_moves(r) == 6);
    //game g{create_game_with_starting_position(starting_position_type::standard)};
    //game_controller c{create_game_controller_with_keyboard_mouse()};
    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6
    r.do_move(); // Bf1-c4

    assert(is_piece_at(r, square("g8")));
    assert(!is_piece_at(r, square("f6")));
    r.do_move(); // Ng8-f6
    assert(!is_piece_at(r, square("g8")));
    assert(is_piece_at(r, square("f6")));
  }
  // replayer can do seven moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7#")));
    assert(get_n_moves(r) == 7);
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6
    r.do_move(); // Bf1-c4
    r.do_move(); // Ng8-f6

    assert(is_piece_at(r, square("h5")));
    assert(is_piece_at(r, square("f7"))); // The piece to be captures
    r.do_move(); // Qh5xf7#
    assert(!is_piece_at(r, square("h5")));
    assert(is_piece_at(r, square("f7")));
  }
  // replayer can do seven moves with score
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0")));
    assert(get_n_moves(r) == 7);
    //game g{create_game_with_starting_position(starting_position_type::standard)};
    //game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6
    r.do_move(); // Bf1-c4
    r.do_move(); // Ng8-f6
    r.do_move(); // Qh5xf7#
    r.do_move(); // 1-0, which is ignored
  }
  // replayer can do seven moves with score and conclude checkmate
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replayer r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0")));
    assert(get_n_moves(r) == 7);
    //game g{create_game_with_starting_position(starting_position_type::standard)};
    //game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6
    r.do_move(); // Bf1-c4
    r.do_move(); // Ng8-f6
    r.do_move(); // Qh5xf7#
    r.do_move(); // 1-0, which is ignored
    assert(is_checkmate(r.get_game().get_pieces(), chess_color::black));
  }
  // replayer can do fools mate and conclude checkmate
  {
    // Fools's mate
    // 1. f3 e6 2. g4 Qh4#
    replayer r(create_action_history_from_pgn(pgn_game_string("1. f3 e6 2. g4 Qh4#")));
    assert(get_n_moves(r) == 4);
    //game g{create_game_with_starting_position(starting_position_type::standard)};
    //game_controller c{create_game_controller_with_keyboard_mouse()};

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6
    assert(is_checkmate(r.get_game().get_pieces(), chess_color::white));
    assert(!is_checkmate(r.get_game().get_pieces(), chess_color::black));
  }
  // extract_game_statistics_in_time
  {
    const auto r = get_played_scholars_mate();
    const auto statistics = extract_game_statistics_in_time(r, delta_t(0.2));
    assert(!statistics.get().empty());
  }
  // get_played_scholars_mate
  {
    const auto r{get_played_scholars_mate()};
    assert(is_checkmate(r.get_game().get_pieces(), chess_color::black));
    assert(r.get_game().get_winner().value() == chess_color::white);
  }
  // to_fen_string
  {
    // Scholar's mate
    // 1. e4 c5 Nf3
    replayer r(
      create_action_history_from_pgn(pgn_game_string("1. e4 c5 2. Nf3")),
      create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::standard))
    );
    assert(get_n_moves(r) == 3);

    const auto s1{to_fen_str(r.get_game().get_pieces())};
    assert(s1 == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    r.do_move(); // e2-e4

    const auto s2{
      to_fen_str(
        r.get_game().get_pieces(),
        chess_color::black,
        "KQkq",
        "e3",
        0,
        1
      )
    };
    assert(s2 == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    r.do_move(); // c5

    const auto s3{
      to_fen_str(
        r.get_game().get_pieces(),
        chess_color::white,
        "KQkq",
        "c6",
        0,
        2
      )
    };
    assert(s3 == "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");

    r.do_move(); // Nf3

    const auto s4{
      to_fen_str(
        r.get_game().get_pieces(),
        chess_color::black,
        "KQkq",
        "-",
        1,
        2
      )
    };
    assert(s4 == "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
  }
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
    && lhs.get_index() == rhs.get_index()
  ;
}

std::ostream& operator<<(std::ostream& os, const replayer& r) noexcept
{
  os
    << "Last time: " << r.get_index() << '\n'
    << "Action history: " << r.get_action_history()
  ;
  return os;

}
