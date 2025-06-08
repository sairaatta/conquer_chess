#ifndef SCREEN_COORDINATE_H
#define SCREEN_COORDINATE_H

#include <iosfwd>

/// The coordinate on a screen.
///
/// The coordinate on the screen, where (0,0) denotes
/// the top-left pixel of the screen.
/// @see \link{game_coordinate} for the chessboard coordinats
class screen_coordinate
{
public:
  explicit screen_coordinate(const int x = 0, const int y = 0);

  int get_x() const noexcept { return m_x; }
  int get_y() const noexcept { return m_y; }

private:

  int m_x;
  int m_y;
};

/// Calculate the angle in degrees
/// dx | dy | o'clock | degrees
/// ---|----|---------|--------
///  1 |  0 |       3 | 0
///  0 |  1 |       6 | 270
/// -1 |  0 |       9 | 180
///  0 | -1 |      12 | 90
double calc_angle_degrees(const screen_coordinate& from, const screen_coordinate& to);

/// Calculate the angle in degrees
/// dx | dy | o'clock | degrees
/// ---|----|---------|--------
///  1 |  0 |       3 | 0
///  0 |  1 |       6 | 270
/// -1 |  0 |       9 | 180
///  0 | -1 |      12 | 90
double calc_angle_degrees(const screen_coordinate& delta);

/// Calculate the Euclidean distance
double calc_distance(const screen_coordinate& a, const screen_coordinate& b) noexcept;

/// Get the default main about screen size, where x is the width and y is the height
screen_coordinate get_default_about_screen_size() noexcept;

/// Get the default controls screen size, where x is the width and y is the height
screen_coordinate get_default_controls_screen_size() noexcept;

/// Get the default loading screen size, where x is the width and y is the height
screen_coordinate get_default_loading_screen_size() noexcept;

/// Get the default lobby screen size, where x is the width and y is the height
screen_coordinate get_default_lobby_screen_size() noexcept;

/// Get the default main menu screen size, where x is the width and y is the height
screen_coordinate get_default_main_menu_screen_size() noexcept;

/// Get the default played game screen size, where x is the width and y is the height
screen_coordinate get_default_played_game_screen_size() noexcept;

/// Get the default screen size, where x is the width and y is the height
screen_coordinate get_default_screen_size() noexcept;


std::ostream& operator<<(std::ostream& os, const screen_coordinate& coordinat);

bool operator==(const screen_coordinate& lhs, const screen_coordinate& rhs) noexcept;

screen_coordinate operator+(const screen_coordinate& lhs, const screen_coordinate& rhs) noexcept;
screen_coordinate operator-(const screen_coordinate& lhs, const screen_coordinate& rhs) noexcept;
screen_coordinate operator*(const screen_coordinate& lhs, const double& factor) noexcept;
screen_coordinate operator/(const screen_coordinate& lhs, const double& factor);

screen_coordinate& operator+=(screen_coordinate& lhs, const screen_coordinate& rhs) noexcept;

/// Run the tests for the screen coordinat
void test_screen_coordinate();

#endif // SCREEN_COORDINATE_H
