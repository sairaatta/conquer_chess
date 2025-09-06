#ifndef PIECE_TYPE_H
#define PIECE_TYPE_H

#include <iosfwd>
#include <vector>
#include <string>

/// The type of chess piece.
enum class piece_type
{
  bishop,
  king,
  knight,
  pawn,
  queen,
  rook
};

/// Get all the piece types
std::vector<piece_type> get_all_piece_types() noexcept;

/// Get the value of the pieces.
///
/// A king has a value of zero, although in practice its value is infinite
int get_piece_value(const piece_type type);

/// Test this class and its free functions
void test_piece_type();

/// Convert the PGN notation of a piece to its type,
/// e.g. 'N' becomes a knight
piece_type to_piece_type(const char c);

/// Convert a piece_type to a string
std::string to_str(const piece_type type) noexcept;

std::ostream& operator<<(std::ostream& os, const piece_type type) noexcept;

#endif // PIECE_TYPE_H
