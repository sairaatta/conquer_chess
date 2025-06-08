#ifndef GAME_RECT_H
#define GAME_RECT_H

#include "game_coordinate.h"

#include <iosfwd>

/// A rectangle, as down on the screen
class game_rect
{
public:
  explicit game_rect(
    const game_coordinate& top_left = game_coordinate(),
    const game_coordinate& bottom_right = game_coordinate()
  );

  const auto& get_tl() const noexcept { return m_top_left; }
  const auto& get_br() const noexcept { return m_bottom_right; }

private:
  game_coordinate m_top_left;
  game_coordinate m_bottom_right;
};

game_coordinate get_center(const game_rect& r) noexcept;
int get_height(const game_rect& r) noexcept;
int get_width(const game_rect& r) noexcept;

/// Is the coordinat in the rectangle?
bool is_in(const game_coordinate& pos, const game_rect& r) noexcept;

/// Test this class and its free functions
void test_game_rect();

bool operator==(const game_rect& lhs, const game_rect& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const game_rect& r) noexcept;

#endif // GAME_RECT_H
