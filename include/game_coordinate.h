#ifndef GAME_COORDINATE_H
#define GAME_COORDINATE_H

#include "ccfwd.h"
#include "chess_color.h"

#include <iosfwd>
#include <random>

/// An exact coordinate anywhere on the board.
///
/// Unlike a \link{square}, a \link{game_coordinate} can indicate
/// a spot on a square.
///
/// @see use \link{screen_coordinate} for a coordinat on the screen
///
/// ```text
///        x coordinat
/// |-----------------------|
///
/// 0  1  2  3  4  5  6  7  8
/// +--+--+--+--+--+--+--+--+ 0  -
/// |  |  |  |  |  |  |  |  |    |
/// +--+--+--+--+--+--+--+--+ 1  |
/// .  .  .  .  .  .  .  .  .    | y coordinat
/// .  .  .  .  .  .  .  .  .    |
/// +--+--+--+--+--+--+--+--+ 7  |
/// |  |  |  |  |  |  |  |  |    |
/// +--+--+--+--+--+--+--+--+ 8  -
/// ```
class game_coordinate
{
public:
  explicit game_coordinate(const double x = 0, const double y = 0);

  double get_x() const noexcept { return m_x; }
  double get_y() const noexcept { return m_y; }

private:

  double m_x;
  double m_y;
};

/// Calculate the euclidean distance between two points
double calc_distance(const game_coordinate& lhs, const game_coordinate& rhs) noexcept;

/// Calculate the length of the vector,
/// a.k.a. the distance between the coordinat and the origin
double calc_length(const game_coordinate& coordinat) noexcept;

/// Create a random game_coordinate
game_coordinate create_random_game_coordinate(
  std::default_random_engine& rng_engine
);

/// Get the game coordinat one square above this one,
/// i.e. when the player presses up
game_coordinate get_above(const game_coordinate& coordinat) noexcept;

/// Get the game coordinat one square below this one,
/// i.e. when the player presses down
game_coordinate get_below(const game_coordinate& coordinat) noexcept;

/// Get the game coordinat one square left of this one,
/// i.e. when the player presses left
game_coordinate get_left(const game_coordinate& coordinat) noexcept;

/// Get the game coordinat one square right of this one,
/// i.e. when the player presses right
game_coordinate get_right(const game_coordinate& coordinat) noexcept;

/// Rotate the coordinat,
/// i.e. turn the board 180 degrees
game_coordinate get_rotated_coordinat(const game_coordinate& coordinat) noexcept;

/// Determine if the coordinat is on the chess board.
/// Any game_coordinate that is on the board can be
/// converted to a square.
bool is_coordinat_on_board(const game_coordinate& c) noexcept;

/// Is the 'to' coordinat forward,
/// i.e. at a rank forward,
/// i.e. can a pawn move/attack in that direction?
/// Note that, e.g., for white h8 and a8 are forward of a2,
/// i.e. the file is ignored
bool is_forward(
  const chess_color color,
  const square& from,
  const square& to
);

/// Test this class and its free function
void test_game_coordinate();

/// Convert to coordinat to chess notation, e.g. 'e2'
/// For invalid coordinats, returns '--'
std::string to_notation(const game_coordinate& g);

/// center a coordinat on the center of a square,
/// i.e. at coorddinat (x.5, y.5)
game_coordinate center_on_center(const game_coordinate& coordinat);

std::ostream& operator<<(std::ostream& os, const game_coordinate& coordinat);

bool operator==(const game_coordinate& lhs, const game_coordinate& rhs) noexcept;
bool operator!=(const game_coordinate& lhs, const game_coordinate& rhs) noexcept;
game_coordinate operator-(const game_coordinate& lhs, const game_coordinate& rhs) noexcept;
game_coordinate operator+(const game_coordinate& lhs, const game_coordinate& rhs) noexcept;
game_coordinate operator*(const game_coordinate& c, const double x) noexcept;
game_coordinate& operator+=(game_coordinate& lhs, const game_coordinate& rhs) noexcept;
game_coordinate operator/(const game_coordinate& coordinat, const double factor) noexcept;

#endif // GAME_COORDINATE_H
