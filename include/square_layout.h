#ifndef SQUARE_LAYOUT_H
#define SQUARE_LAYOUT_H

#include "screen_rect.h"

/// The layout of a square.
///
/// This layout is only relevant if there is a piece in this square
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
/// ``
/// `
class square_layout
{
public:

  square_layout(const screen_rect& square_rect);
  const screen_rect& get_health_bar() const noexcept { return m_health_bar; }
  const screen_rect& get_piece() const noexcept { return m_piece; }
  const screen_rect& get_is_protected() const noexcept { return m_is_protected; }

private:

  screen_rect m_health_bar;
  screen_rect m_is_protected;
  screen_rect m_piece;
};

void test_piece_layout();

#endif // SQUARE_LAYOUT_H
