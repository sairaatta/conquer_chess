#ifndef ABOUT_VIEW_LAYOUT_H
#define ABOUT_VIEW_LAYOUT_H

#include "about_view_item.h"
#include "screen_rect.h"
#include "layout.h"

#include <vector>

/// The layout of the menu view.
///
/// ```text
///                    x x          x x
///                    1 2          3 4
/// +----------------------------------------------------+
/// |                                                    |
/// |                  +--------------+                  |   y1
/// |                  |              |                  |
/// |                  | +----------+ |                  |   y2
/// |                  | |          | |                  |
/// |                  | | title    | |                  |
/// |                  | | 2 panels | |                  |
/// |                  | +----------+ |                  |   y3
/// |                  |              |                  |
/// |                  | +----------+ |                  |   y4
/// |                  | | subtitle | |                  |
/// |                  | +----------+ |                  |   y5
/// |                  |              |                  |
/// |                  | +----------+ |                  |   y6
/// |                  | | copyright| |                  |
/// |                  | +----------+ |                  |   y7
/// |                  |              |                  |
/// |                  | +----------+ |                  |   y8
/// |                  | | url      | |                  |
/// |                  | +----------+ |                  |   y9
/// |                  |              |                  |
/// |                  | +----------+ |                  |   y10
/// |                  | | contri-  | |                  |
/// |                  | | butors   | |                  |
/// |                  | |          | |                  |
/// |                  | |          | |                  |
/// |                  | +----------+ |                  |   y11
/// |                  |              |                  |
/// |                  +--------------+                  |   y12
/// |                                                    |
/// +----------------------------------------------------+
///                      ^          ^
///                      |          |
///                      +----------+
///                      panel_width
/// ```
///
class about_view_layout
{
public:
  explicit about_view_layout(
    const screen_coordinate& window_size = get_default_screen_size(),
    const int margin_width = get_default_margin_width()
  );

  const auto& get_background() const noexcept { return m_background; }
  const auto& get_copyright() const noexcept { return m_copyright; }
  const auto& get_panel_outline() const noexcept { return m_panel_outline; }
  const auto& get_subtitle() const noexcept { return m_subtitle; }
  const auto& get_contributors() const noexcept { return m_contributors; }
  const auto& get_title() const noexcept { return m_title; }
  const auto& get_url() const noexcept { return m_url; }

  screen_coordinate get_window_size() const noexcept { return m_window_size; }

private:

  screen_rect m_background;
  screen_rect m_contributors;
  screen_rect m_copyright;
  screen_rect m_panel_outline;
  screen_rect m_subtitle;
  screen_rect m_title;
  screen_rect m_url;

  /// The size of the window
  screen_coordinate m_window_size;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const about_view_layout& layout);

/// Test the about_view_layout class
void test_about_view_layout();

#endif // ABOUT_VIEW_LAYOUT_H
