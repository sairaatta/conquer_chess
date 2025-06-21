#ifndef PIECE_LAYOUT_H
#define PIECE_LAYOUT_H

#include "screen_rect.h"

/// The layout of a piece
///
/// ```text
/// +------------------+
/// |+----------------+|
/// || health_bar     ||
/// |+----------------+|
/// || piece          ||
/// ||                ||
/// ||                ||
/// ||                ||
/// |+----------------+|
/// +------------------+
/// ```
class piece_layout
{
public:

  piece_layout(const screen_rect& square_rect);
  const screen_rect& get_health_bar() const noexcept { return m_health_bar; }
  const screen_rect& get_piece() const noexcept { return m_piece; }

private:

  screen_rect m_health_bar;
  screen_rect m_piece;
};

void test_piece_layout();

#endif // PIECE_LAYOUT_H
