#ifndef GAME_STATISTICS_VIEW_LAYOUT_H
#define GAME_STATISTICS_VIEW_LAYOUT_H

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
/// | | text     | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | +----------+ | y2
/// |              |
/// +--------------+
///
///   ^          ^
///   |          |
///   +----------+
///    panel_width
/// ```text
class game_statistics_view_layout
{
public:
  explicit game_statistics_view_layout(
    const screen_rect& r = get_default_screen_rect()
  );

  /// The full screen area of this layout
  const auto& get_background() const noexcept { return m_background; }

  const auto& get_text() const noexcept { return m_text; }

private:

  screen_rect m_background;
  screen_rect m_text;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const game_statistics_view_layout& layout);

/// Test the game_statistics_view_layout class
void test_game_statistics_view_layout();

#endif // GAME_STATISTICS_VIEW_LAYOUT_H
