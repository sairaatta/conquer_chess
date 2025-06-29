#ifndef SQUARE_LAYOUT_H
#define SQUARE_LAYOUT_H

#include "race.h"
#include "screen_rect.h"

/// The layout of a square.
///
/// This layout is only relevant if there is a piece in this square
///
/// ```text
/// +------------------+
/// |+----------------+|
/// || health_bar     ||
/// ||                ||
/// ||                ||
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
///
/// Or for a protoss piece:
///
/// ```text
/// +------------------+
/// |+----------------+|
/// || health_bar     ||
/// |+----------------+|
/// || shield_bar     ||
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
class square_layout
{
public:

  square_layout(const screen_rect& square_rect = screen_rect(screen_coordinate(0, 0), screen_coordinate(20, 20)));

  /// The full square
  const screen_rect& get_square() const noexcept { return m_square; }

  /// The outline of the health bar
  const screen_rect& get_health_bar_outline() const noexcept { return m_health_bar; }

  /// The health bar value, a bar within the outline of the health bar
  screen_rect get_health_bar_value(const double f, const race r = race::classic) const;

  /// The shield bar value, a bar within the lower half of the outline of the health bar
  screen_rect get_shield_bar_value(const double f) const;

  const screen_rect& get_piece() const noexcept { return m_piece; }
  const screen_rect& get_is_protected() const noexcept { return m_is_protected; }

private:


  screen_rect m_health_bar;
  screen_rect m_is_protected;
  screen_rect m_piece;
  screen_rect m_square;
};

void test_piece_layout();

#endif // SQUARE_LAYOUT_H
