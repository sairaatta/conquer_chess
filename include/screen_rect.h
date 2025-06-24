#ifndef SCREEN_RECT_H
#define SCREEN_RECT_H

#include "screen_coordinate.h"

#include <iosfwd>

/// A rectangle on the screen.
///
/// A rectangle on the screen, where `(0,0), (1,2)`
/// denotes the pixels from the top-left corner of the screen,
/// to the second pixel to the right and the third on down.
class screen_rect
{
public:
  explicit screen_rect(
    const screen_coordinate& top_left = screen_coordinate(0, 0),
    const screen_coordinate& bottom_right = screen_coordinate(20, 20)
  );

  const auto& get_tl() const noexcept { return m_top_left; }
  const auto& get_br() const noexcept { return m_bottom_right; }

private:
  screen_coordinate m_top_left;
  screen_coordinate m_bottom_right;
};


/// Create a rectangle that is centered around a point, with a custom height and width.
/// @param c the location of the center
/// @param w the width of the rectangle
/// @param h the height of the rectangle
screen_rect create_centered_rect(const screen_coordinate c, const int w, const int h) noexcept;

/// Create a rectangle that is exactly one pixel inside the given one
screen_rect create_rect_inside(const screen_rect& r) noexcept;

screen_coordinate get_center(const screen_rect& r) noexcept;

/// Get the bottom-left corner (i.e. 25% of the rect)
screen_rect get_bottom_left_corner(const screen_rect& r) noexcept;

/// Get the bottom-right corner (i.e. 25% of the rect)
screen_rect get_bottom_right_corner(const screen_rect& r) noexcept;

/// Get the default screen rectangle.
///
/// This is used mostly in testing of layouts.
screen_rect get_default_screen_rect() noexcept;

int get_height(const screen_rect& r) noexcept;

/// Get the left-hand side half (i.e. 50% of the rect)
screen_rect get_lhs_half(const screen_rect& r) noexcept;

/// Get the lower eighth (i.e. 12.5% of the rect,
/// half of the half of the lower half)
screen_rect get_lower_eighth(const screen_rect& r) noexcept;

/// Get the lower fourth (i.e. 25% of the rect, half of the lower half)
screen_rect get_lower_fourth(const screen_rect& r) noexcept;

/// Get the lower half (i.e. 50% of the rect)
screen_rect get_lower_half(const screen_rect& r) noexcept;

/// Get the right-hand side half (i.e. 50% of the rect)
screen_rect get_rhs_half(const screen_rect& r) noexcept;

/// Get the top-left corner (i.e. 25% of the rect)
screen_rect get_top_left_corner(const screen_rect& r) noexcept;

/// Get the top-right corner (i.e. 25% of the rect)
screen_rect get_top_right_corner(const screen_rect& r) noexcept;

/// Get the upper half (i.e. 50% of the rect)
screen_rect get_upper_half(const screen_rect& r) noexcept;

int get_width(const screen_rect& r) noexcept;

/// Is the coordinat in the rectangle?
bool is_in(const screen_coordinate& pos, const screen_rect& r) noexcept;

/// Test this class and its free functions
void test_screen_rect();

bool operator==(const screen_rect& lhs, const screen_rect& rhs) noexcept;
bool operator!=(const screen_rect& lhs, const screen_rect& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const screen_rect& r) noexcept;

#endif // SCREEN_RECT_H
