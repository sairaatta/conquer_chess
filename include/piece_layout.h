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
/// ||                ||
/// ||                ||
/// ||                ||
/// ||                ||
/// ||             +-+||
/// ||             |1|||
/// ||             +-+||
/// |+----------------+|
/// +------------------+
///
/// - 1: symbol to show if the piece is protected
/// ```
class piece_layout
{
public:

  piece_layout(const screen_rect& square_rect);
  const screen_rect& get_health_bar() const noexcept { return m_health_bar; }
  const screen_rect& get_piece() const noexcept { return m_piece; }
  const screen_rect& get_is_protected() const noexcept { return m_is_protected; }

private:

  screen_rect m_health_bar;
  screen_rect m_is_protected;
  screen_rect m_piece;
};

void test_piece_layout();

#endif // PIECE_LAYOUT_H
