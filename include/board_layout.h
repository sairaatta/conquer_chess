#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

#include "screen_rect.h"

#include "square_layout.h"

#include <vector>

/// The layout of a chess board.
///
/// It fits exactly within the screen rectangle
/// and there are no gaps or overlap between the squares
class board_layout
{
public:
  /// Create the layout of a board.
  ///
  /// The default size is the minimum, as this makes sure
  /// that there are enough pixels for a proper display of the pieces.
  board_layout(const screen_rect& r = screen_rect(screen_coordinate(0, 0), screen_coordinate(20 * 8, 20 * 8)));

  /// Get the size of the entire board
  const screen_rect& get_board() const noexcept;

  /// Get the rectangle for the square.
  ///
  /// The board is set up to have white at the left.
  /// - (0,0) is a1
  /// - (7,0) is a8
  /// - (0,7) is h1
  /// - (7,7) is h8
  const square_layout& get_square(const int x, const int y) const;

private:

  screen_rect m_background;

  //std::vector<std::vector<screen_rect>> m_rects;
  std::vector<std::vector<square_layout>> m_rects;
};

int get_height(const board_layout& b);

int get_width(const board_layout& b);

void test_board_layout();

#endif // BOARD_LAYOUT_H

