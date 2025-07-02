#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "castling_type.h"
#include "piece_type.h"
#include "square.h"
#include "pgn_move_string.h"
#include "fen_string.h"

#include <string>
#include <vector>
#include <optional>
#include <iosfwd>

/// A chess move.
///
/// A chess move, e.g. 'e4'.
///
/// @note This called is called `chess_move` instead of `move`
///   to avoid conflicts with `std::move'`.
class chess_move
{
public:
  /// Moves always happen on a board
  explicit chess_move(
    const pgn_move_string& pgn_str,
    const fen_string& fen_str = create_fen_string_of_standard_starting_position()
  );

  /// The type of action this move is
  const auto& get_action_type() const noexcept { return m_action_type; }

  /// Get the castling type.
  /// Will be empty if this move is not a promotion
  const auto& get_castling_type() const noexcept { return m_castling_type; };

  /// Get the color of the player that did this move
  const auto& get_color() const noexcept { return m_color; };

  /// Get the source/from square, e.g. 'b1' in 'Pc3'.
  ///
  /// Result will be empty when winning (e.g. '1-0')
  const auto& get_from() const noexcept { return m_from; }

  /// Get the original PGN string back
  const auto& get_pgn_str() const noexcept { return m_pgn_str; }

  /// Get the promotion type.
  /// Will be empty if this move is not a promotion
  const auto& get_promotion_type() const noexcept { return m_promotion_type; };

  /// Get the target square, e.g. 'e4' in 'Pxe4'.
  /// Result will be empty when castling (e.g. 'O-O') or winning (e.g. '1-0')
  /// @see use the free function \link{get_from} to determine the
  /// square where the pieces comes from
  const auto& get_to() const noexcept { return m_to; }

  /// The type of chess piece.
  /// Result will be empty when castling (e.g. 'O-O') or winning (e.g. '1-0')
  const auto& get_piece_type() const noexcept { return m_piece_type; }

  /// Get the winner.
  ///  * If this is empty, the game is still on-going.
  ///  * If this has 1 element, that color is the winner.
  ///  * If this has 2 elements, the game ended in a draw.
  const auto& get_winner() const noexcept { return m_winner; }

  /// Is this move a capture?
  /// E.g. 'Qxf7' is a capture
  bool is_capture() const noexcept { return m_is_capture; }

private:

  /// The type of action a pieces does, if any
  ///
  /// For example, winning the game is not an action
  std::optional<piece_action_type> m_action_type;

  /// The type of castling. Empty if move is not a castling
  std::optional<castling_type> m_castling_type;

  chess_color m_color;

  std::optional<square> m_from;

  bool m_is_capture;

  /// The original PGN string
  pgn_move_string m_pgn_str;

  /// The type of piece a pawn promotes into.
  /// Empty if move is not a promotion
  std::optional<piece_type> m_promotion_type;

  std::optional<square> m_to;

  std::optional<piece_type> m_piece_type;

  /// Can be
  ///  * No winner yet: empty
  ///  * One winner: one element
  ///  * Draw: two elements
  std::vector<chess_color> m_winner;

};

/// Get the square the piece doing the move came from, if any.
///
/// Even with, e.g., castling, it is the king at e1 that
/// needed to be selected to do that move
std::optional<square> get_from(const fen_string& s, const pgn_move_string& m);

/// Get the square the bishop doing the move came from.
square get_from_for_bishop(const fen_string& s, const pgn_move_string& m);

/// Get the square the king doing the move came from.
/// Even with, e.g., castling for white, it is the king at e1 that
/// needed to be selected to do that move
square get_from_for_king(const fen_string& s, const pgn_move_string& m);

/// Get the square the knight doing the move came from.
square get_from_for_knight(const fen_string& s, const pgn_move_string& m);

/// Get the square the pawn doing the move came from.
square get_from_for_pawn(const fen_string& s, const pgn_move_string& m);

/// Get the square the queen doing the move came from.
square get_from_for_queen(const fen_string& s, const pgn_move_string& m);

/// Get the square the rook doing the move came from.
square get_from_for_rook(const fen_string& s, const pgn_move_string& m);

/// Get if the move is a capture
bool is_capture(const chess_move& move) noexcept;

/// Get if the move is a castling
bool is_castling(const chess_move& move) noexcept;

/// Get if the move is a win
bool is_draw(const chess_move& move) noexcept;

/// Get if the move is a move,
/// i.e. if it moves one piece, without a capture,
/// no castling, no promotion, no win and no draw
bool is_simple_move(const chess_move& move) noexcept;

/// Get if the move is a promotion
bool is_promotion(const chess_move& move) noexcept;

/// Get if the move is a win
bool is_win(const chess_move& move) noexcept;

/// Test this class and its free functions
void test_chess_move();

bool operator==(const chess_move& lhs, const chess_move& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const chess_move& m) noexcept;

#endif // CHESS_MOVE_H
