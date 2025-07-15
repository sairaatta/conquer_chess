#ifndef SQUARE_H
#define SQUARE_H

#include "ccfwd.h"
#include "chess_color.h"
#include "castling_type.h"
#include "piece_action_type.h"

#include <iosfwd>
#include <random>
#include <string>
#include <vector>

/// A chess square.
///
/// A chess square, e.g. e4.
///
/// ```text
/// +--+--+--+--+--+--+--+--+
/// |A1|A2|A3|A4|A5|A6|A7|A8|
/// +--+--+--+--+--+--+--+--+
/// |B1|B2|B3|B4|B5|B6|B7|B8|
/// +--+--+--+--+--+--+--+--+
/// |C1|C2|C3|C4|C5|C6|C7|C8|
/// +--+--+--+--+--+--+--+--+
/// |D1|D2|D3|D4|D5|D6|D7|D8|
/// +--+--+--+--+--+--+--+--+
/// |E1|E2|E3|E4|E5|E6|E7|E8|
/// +--+--+--+--+--+--+--+--+
/// |F1|F2|F3|F4|F5|F6|F7|F8|
/// +--+--+--+--+--+--+--+--+
/// |G1|G2|G3|G4|G5|G6|G7|G8|
/// +--+--+--+--+--+--+--+--+
/// |H1|H2|H3|H4|H5|H6|H7|H8|
/// +--+--+--+--+--+--+--+--+
///
/// +--+--+--+--+--+--+--+--+
/// |00|10|20|30|40|50|60|70|
/// +--+--+--+--+--+--+--+--+
/// |01|11|21|31|41|51|61|71|
/// +--+--+--+--+--+--+--+--+
/// |02|12|22|32|42|52|62|72|
/// +--+--+--+--+--+--+--+--+
/// |03|13|23|33|43|53|63|73|
/// +--+--+--+--+--+--+--+--+
/// |04|14|24|34|44|54|64|74|
/// +--+--+--+--+--+--+--+--+
/// |05|15|25|35|45|55|65|75|
/// +--+--+--+--+--+--+--+--+
/// |06|16|26|36|46|56|66|76|
/// +--+--+--+--+--+--+--+--+
/// |07|17|27|37|47|57|67|77|
/// +--+--+--+--+--+--+--+--+
/// ```
///
/// @seealso a \link{square_layout} defines the layout of a square.
class square
{
public:
  explicit square(const std::string& pos);
  explicit square(const game_coordinate& g);
  /// Create a square from the x and y coordinate
  ///
  /// For example, b7 is (6,1).
  ///
  /// @param x the x coordinat, the index for the number of a square, starting from 0 for 1, 1 for 2, 2 for 3, etc.
  /// @param y the y coordinat, the index for the letter of a square, starting from 0 for a, 1 for b, 3 for cetc.
  explicit square(const int x, const int y);

  /// Get the x coordinat, starting from 0 for a1/b1/c1/etc.
  /// As the board goes from a1 at top-left,
  /// to a8 at top-right,
  /// the x coordinat is the rank
  int get_x() const noexcept { return m_x; }

  /// Get the y coordinat, starting from 0 for a1/a2/a3/etc.
  /// As the board goes from a1 at top-left,
  /// to a8 at top-right,
  /// the y coordinat is the file
  int get_y() const noexcept { return m_y; }

private:

  int m_x;
  int m_y;
};

/// Are the squares adjacent (i.e. for a king, not a knight)
/// @see are_adjacent_for_knight to determine if squares are adjacent
///   for a knight
bool are_adjacent(const square& a, const square& b) noexcept;

/// Are the squares adjacent for a knight
/// @see are_adjacent to determine if squares are adjacent
///   for a king
bool are_adjacent_for_knight(const square& a, const square& b) noexcept;

/// Are all the squares unique?
bool are_all_unique(std::vector<square> squares);

/// Are the squares seperated by the jump of a knight?
///
/// e.g. b1 and c3
///
/// @seealso see \link{are_on_same_half_diagonal} to see if the
/// squares are seperated by multiple knight jumps in the same direction.
bool are_at_knights_jump_distance(const square& a, const square& b) noexcept;

/// Are these possible en-passant capture squares?
///
/// For example, a5xb6 (attacker square and target square respectively)
bool are_en_passant_capture_squares(
  const square& attacker_square,
  const square& target_square,
  const chess_color player_color
);

/// Are the squares adjacent on the same diagonal, e.g. d1 and e2
bool are_on_adjacent_diagonal(const square& a, const square& b) noexcept;

/// Are the squares on the same diagonal, e.g. d1 and a4
bool are_on_same_diagonal(const square& a, const square& b) noexcept;

/// Are the squares on the same file, e.g. e2 and e4
bool are_on_same_file(const square& a, const square& b) noexcept;

/// Are the squares on the same half-diagonal,
/// also 'the jump of a knight'
/// e.g. b1 and d5
///
/// @seealso see \link{are_at_knights_jump_distance} to see if the
/// squares are separated by one knight jump
bool are_on_same_half_diagonal(const square& a, const square& b) noexcept;

/// Are the squares on the same rank, e.g. a1 and a8
bool are_on_same_rank(const square& a, const square& b) noexcept;

/// Collect all the delta squares for a bishop
std::vector<std::pair<int, int>> collect_all_bishop_delta_pairs() noexcept;

/// Collect all the possible target squares for a bishop
/// in all directions
std::vector<std::vector<square>> collect_all_bishop_target_squares(const square& s) noexcept;

/// Collect all the possible target squares for a king
std::vector<square> collect_all_king_target_squares(const square& s) noexcept;

/// Collect all the delta squares for a knight
std::vector<std::pair<int, int>> collect_all_knight_delta_pairs() noexcept;

/// Collect all the possible target squares for a knight
std::vector<square> collect_all_knight_target_squares(const square& s) noexcept;

/// Collect all the delta squares for a queen
std::vector<std::pair<int, int>> collect_all_queen_delta_pairs() noexcept;

/// Collect all the possible target squares for a queen
/// in all directions
std::vector<std::vector<square>> collect_all_queen_target_squares(const square& s) noexcept;

/// Collect all the delta squares for a rook
std::vector<std::pair<int, int>> collect_all_rook_delta_pairs() noexcept;

/// Collect all the possible target squares for a rook
/// in all directions
std::vector<std::vector<square>> collect_all_rook_target_squares(const square& s) noexcept;

/// Concatenate the vectors
std::vector<square> concatenate(
  const std::vector<square>& a,
  const std::vector<square>& b
);

/// Create a random square
square create_random_square(
  std::default_random_engine& rng_engine
);

/// Get all the files, i.e. 'a' to and including 'h'
std::vector<char> get_all_files() noexcept;

/// Get all the ranks, i.e. 1 to and including 8
std::vector<int> get_all_ranks() noexcept;

/// Get all the ranks in reversed order, i.e. 8 to and including 1
std::vector<int> get_all_ranks_in_reversed_order() noexcept;

/// Get the square behind a pawn.
/// Examples:
///  * for white, behind e4 would be e3.
///  * for black, behind e5 would be e6.
square get_behind(
  const square& pawn_square,
  const chess_color color
);

/// Get the square that may be captured by en-passant
///
/// Assumes that the squares are valid, using
/// \link{are_en_passant_capture_squares}
square get_en_passant_capture_square(
  const square& attacker_square,
  const square& target_square
);

/// Get the file of a square, e.g. 'd' from 'd4'
char get_file(const square& s) noexcept;

/// Get the intial king square.
///
/// Which is:
///  - e1 for white
///  - e8 for black
square get_initial_king_square(const chess_color player_color) noexcept;


/// Get the default rook square,
/// for a castling direction.
/// These are:
///  * h1 for white for a kingside castle
///  * a1 for white for a queenside castle
///  * h8 for black for a kingside castle
///  * a8 for black for a queenside castle
square get_initial_rook_square(
  const chess_color player_color,
  const castling_type t
) noexcept;

/// Get the intermediate squares, in an inclusive way:
/// the first square will be 'from',
/// to last square will be 'to'
std::vector<square> get_intermediate_squares(
  const square& from,
  const square& to
);

/// Get the king target square when castling
///
/// Color|Castling type|Target square
/// -----|-------------|-------------
/// Black|Kingside     |e8 -> g8
/// Black|Queenside    |e8 -> c8
/// White|Kingside     |e1 -> g1
/// White|Queenside    |e1 -> c1
///
square get_king_target_square(const chess_color player_color, const piece_action_type action);

/// Get the rank of a square, e.g. '3' from 'e3'
int get_rank(const square& s) noexcept;

/// Get the rook target square when castling
///
/// Color|Castling type|Target square
/// -----|-------------|-------------
/// Black|Kingside     |h8 -> f8
/// Black|Queenside    |a8 -> d8
/// White|Kingside     |h1 -> f1
/// White|Queenside    |a1 -> d1
///
square get_rook_target_square(const chess_color player_color, const piece_action_type action);

/// Is the square 's' occupied?
bool is_occupied(
  const square& s,
  const std::vector<square>& occupied_squares
) noexcept;

/// Can the x and y be used to create a valid square?
bool is_valid_square_xy(const int x, const int y) noexcept;

/// Can the x and y not be used to create a valid square?
bool is_invalid_square_xy(const int x, const int y) noexcept;

/// Test this class and its free functions
void test_square();

/// What is the color of this square?
chess_color to_color(const square& s) noexcept;

// a1 == (0.5, 0.5)
// b1 == (0.5, 1.5)
game_coordinate to_coordinat(const square& s) noexcept;

// a1 == (0.5, 0.5)
// b1 == (0.5, 1.5)
game_coordinate to_coordinat(const std::string& notation) noexcept;


/// Convert a square to a rectangle
/// For example, a1 == ((0,0)-(1,1)) (notation is top-left, then bottom-left)
/// For example, b1 == ((0,1)-(1,2)) (notation is top-left, then bottom-left)
game_rect to_game_rect(const square& s) noexcept;

/// Convert the pairs of x-y-coordinats to valid squares.
std::vector<square> to_squares(std::vector<std::pair<int, int>> xys);

std::string to_str(const square& s) noexcept;

std::string to_str(const std::vector<square>& s) noexcept;

bool operator==(const square& lhs, const square& rhs) noexcept;
bool operator!=(const square& lhs, const square& rhs) noexcept;
bool operator<(const square& lhs, const square& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const square& s) noexcept;
std::ostream& operator<<(std::ostream& os, const std::vector<square>& s) noexcept;

#endif // SQUARE_H
