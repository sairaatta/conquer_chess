#ifndef REPLAY_VIEW_LAYOUT_H
#define REPLAY_VIEW_LAYOUT_H

#include "screen_rect.h"
#include "layout.h"

#include <vector>

/// The layout of a 'Played game' dialog.
///
/// ```text
///   x          x
///   1          2
///
/// +--------------+
/// |              |
/// | +----------+ | y1
/// | |          | |
/// | | board    | |
/// | |          | |
/// | |          | |
/// | +----------+ | y2
/// |              |
/// | +----------+ | y3
/// | |          | |
/// | |statistics| |
/// | |          | |
/// | |          | |
/// | +----------+ | y4
/// |              |
/// +--------------+
///
///   ^          ^
///   |          |
///   +----------+
///    panel_width
/// ```text
class replay_view_layout
{
public:
  explicit replay_view_layout(
    const screen_rect& r = get_default_screen_rect()
  );

  /// The full screen area of this layout
  const auto& get_background() const noexcept { return m_background; }

  const auto& get_board() const noexcept { return m_board; }

  const auto& get_statistics() const noexcept { return m_statistics; }

private:

  screen_rect m_background;
  screen_rect m_board;
  screen_rect m_statistics;
};

/// Test the replay_view_layout class
void test_replay_view_layout();

#endif // REPLAY_VIEW_LAYOUT_H
