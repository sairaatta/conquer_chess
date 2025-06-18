#ifndef MENU_VIEW_LAYOUT_H
#define MENU_VIEW_LAYOUT_H

#include "menu_view_item.h"
#include "screen_rect.h"
#include "layout.h"

#include <vector>

/// The layout of the Menu dialog
///
/// ```text
/// Margins      _          _ _                    _
/// ..........0 x x        x x x                  x width
/// ..........  1 2        3 4 5                  6
/// ..........
/// ..........+-----------------------------------------+
/// ..........|                                         |
/// ..........| +--------------+                        | y1
/// ..........| |              |                        |
/// ..........| | +----------+ |                        | y2
/// ..........| | |          | |                        |
/// ..........| | | title    | |                        |
/// ..........| | |          | |                        |
/// ..........| | +----------+ |                        | y3, y4
/// ..........| | | subtitle | |                        |
/// ..........| | +----------+ |                        | y5
/// ..........| |              |                        |
/// ..........| | +----------+ |                        | y6
/// ..........| | |spaces    | |                        |
/// ..........| | +----------+ |                        | y7
/// ..........| |              |                        |
/// ..........| | +----------+ |                        | y8
/// ..........| | | start    | |                        |
/// ..........| | +----------+ |                        | y9
/// ..........| |              |                        |
/// ..........| | +----------+ |                        | y10
/// ..........| | | options  | |                        |
/// ..........| | +----------+ |                        | y11
/// ..........| |              |                        |
/// ..........| | +----------+ |                        | y12
/// ..........| | | about    | |                        |
/// ..........| | +----------+ |                        | y13
/// ..........| |              |                        |
/// ..........| | +----------+ |                        | y14
/// ..........| | | quit     | |                        |
/// ..........| | +----------+ |                        | y15
/// ..........| |              |                        |
/// ..........| +--------------+                        | y16
/// ..........|                                         |
/// ..........| +-------------------------------------+ | y17
/// ..........| | Controls                            | |
/// ..........| +-------------------------------------+ | y18
/// ..........|                                         |
/// ..........+-----------------------------------------+
/// ..........
/// ..........    ^          ^
/// ..........    |          |
/// ..........    +----------+
/// ..........     panel_width
/// ```
class menu_view_layout
{
public:
  explicit menu_view_layout(
    const screen_coordinate& window_size = get_default_screen_size(),
    const int margin_width = get_default_margin_width()
  );

  /// Get a screen rect
  const screen_rect& get_selectable_rect(const menu_view_item item) const noexcept;

  /// The rectangle around the buttons
  const auto& get_menu_panel() const noexcept { return m_menu_panel; }

  /// The screen area where the frames per seconds are displayed
  const auto& get_title() const noexcept { return m_title; }
  const auto& get_subtitle() const noexcept { return m_subtitle; }
  const auto& get_background_image() const noexcept { return m_background_image; }
  const auto& get_start() const noexcept { return m_start; }
  const auto& get_about() const noexcept { return m_about; }
  const auto& get_options() const noexcept { return m_options; }
  const auto& get_quit() const noexcept { return m_quit; }
  const auto& get_controls() const noexcept { return m_controls; }

  screen_coordinate get_window_size() const noexcept { return m_window_size; }


private:

  /// The panel around the buttons
  screen_rect m_menu_panel;

  screen_rect m_title;
  screen_rect m_subtitle;
  screen_rect m_background_image;
  screen_rect m_start;
  screen_rect m_options;
  screen_rect m_about;
  screen_rect m_quit;
  screen_rect m_controls;

  /// The size of the window
  screen_coordinate m_window_size;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const menu_view_layout& layout);

/// Test the menu_view_layout class
void test_menu_view_layout();

#endif // MENU_VIEW_LAYOUT_H
