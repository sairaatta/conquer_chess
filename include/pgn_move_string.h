#ifndef PGN_MOVE_STRING_H
#define PGN_MOVE_STRING_H

#include "square.h"
#include "piece_type.h"

#include <optional>
#include <string>
//#include <vector>

/// A string that is a PGN notation string.
///
/// For example, a game with a scholar's mate can have the PGN string
/// `1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0`
class pgn_move_string
{
public:
  pgn_move_string(const std::string& pgn_str = "");

  const auto& get() const noexcept { return m_pgn_str; }

private:
  std::string m_pgn_str;
};

/// Get a piece type for a string, if any.
///
/// E.g. 'Nc3' will result in a knight, 'e4' will result in a pawn
std::optional<piece_type> get_piece_type(const pgn_move_string& pgn_str);

/// Get the square from a string, if any.
///
/// E.g. 'Nc3' will result in 'c3'
///
/// @seealso same as \link{get_to}
std::optional<square> get_square(const pgn_move_string& pgn_str);

/// Get the type the piece is promoted to.
/// Will be empty if this is no promotion
std::optional<piece_type> get_promotion_type(const pgn_move_string& pgn_str);

/// Get the square a piece is moving to, if any.
///
/// E.g. 'Nc3' will result in 'c3'
///
/// @seealso same as \link{get_square}
std::optional<square> get_to(const pgn_move_string& pgn_str);


/// Get the winner from a notation.
/// Assumes a win ('1-0' or '0-1') or a draw ('1/2-1/2').
std::vector<chess_color> get_winner(const pgn_move_string& pgn_str);

/// Conclude if the move is a capture from a PGN string
/// E.g. 'Nxc3' will result in true
bool is_capture(const pgn_move_string& pgn_str);

void test_pgn_move_string();

bool operator==(const pgn_move_string& lhs, const pgn_move_string& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const pgn_move_string& s) noexcept;

#endif // PGN_MOVE_STRING_H
