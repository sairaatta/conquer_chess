#include "square_layout.h"

#include "screen_coordinate.h"

#include <cassert>

square_layout::square_layout(const screen_rect& r)
  : m_square(r)
{
  const int square_width{get_width(r)};
  const int square_height{get_height(r)};

  // Health bar
  {
    const int x1 = r.get_tl().get_x() + (0.1 * square_width);
    const int y1 = r.get_tl().get_y() + (0.05 * square_height);
    const int x2 = r.get_br().get_x() - (0.1 * square_width);
    const int y2 = r.get_tl().get_y() + (0.15 * square_height);
    m_health_bar = screen_rect(
      screen_coordinate(x1, y1),
      screen_coordinate(x2, y2)
    );
  }
  // Piece
  {
    const int x1 = r.get_tl().get_x() + (0.1 * square_width);
    const int y1 = r.get_tl().get_y() + (0.2 * square_height);
    const int x2 = r.get_br().get_x() - (0.1 * square_width);
    const int y2 = r.get_br().get_y() - (0.0 * square_height);
    m_piece = screen_rect(
      screen_coordinate(x1, y1),
      screen_coordinate(x2, y2)
    );
  }
  // Is protected
  {
    const int x1 = m_piece.get_br().get_x() - 4 - 16;
    const int y1 = m_piece.get_br().get_y() - 4 - 16;
    const int x2 = m_piece.get_br().get_x() - 4;
    const int y2 = m_piece.get_br().get_y() - 4;
    m_is_protected = screen_rect(
      screen_coordinate(x1, y1),
      screen_coordinate(x2, y2)
    );
  }
}

void test_piece_layout()
{
#ifndef NDEBUG
  // Constructor
  {
    const screen_rect r(
      screen_coordinate(0, 0),
      screen_coordinate(100, 100)
    );
    const auto layout{square_layout(r)};
    assert(layout.get_health_bar().get_tl().get_x() == 10);
    assert(layout.get_health_bar().get_tl().get_y() == 5);
    assert(layout.get_health_bar().get_br().get_x() == 90);
    assert(layout.get_health_bar().get_br().get_y() == 15);

    assert(layout.get_piece().get_tl().get_x() == 10);
    assert(layout.get_piece().get_tl().get_y() == 20);
    assert(layout.get_piece().get_br().get_x() == 90);
    assert(layout.get_piece().get_br().get_y() == 100);

    assert(get_width(layout.get_is_protected()) < get_width(layout.get_piece()));
    assert(get_height(layout.get_is_protected()) < get_height(layout.get_piece()));

  }
#endif

}
