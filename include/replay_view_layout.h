#ifndef REPLAY_VIEW_LAYOUT_H
#define REPLAY_VIEW_LAYOUT_H

#include "screen_rect.h"
#include "board_layout.h"
#include "game_statistics_widget_layout.h"

//#include <vector>

/// The layout of a 'Played game' dialog.
///
/// ```text
///   x          x x           x4
///   1          2 3
///
/// +-----------------------------+
/// |                             |
/// | +----------+ +------------+ | y1
/// | |          | |            | |
/// | | board    | | statistics | |
/// | |          | |            | |
/// | |          | |            | |
/// | +----------+ +------------+ | y2
/// |                             |
/// | +-------------------------+ | y3
/// | | statistics width        | |
/// | +-------------------------+ | y4
/// |                             |
/// +-----------------------------+
///
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

  const auto& get_statistics_widget() const noexcept { return m_statistics_widget; }

private:

  screen_rect m_background;
  board_layout m_board;
  screen_rect m_statistics;
  game_statistics_widget_layout m_statistics_widget;
};

/// Test the replay_view_layout class
void test_replay_view_layout();

#endif // REPLAY_VIEW_LAYOUT_H
