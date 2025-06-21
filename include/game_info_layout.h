#ifndef GAME_INFO_LAYOUT_H
#define GAME_INFO_LAYOUT_H

#include "screen_rect.h"

/// The layout of the game info
///
/// ```text
/// +-----------------------+
/// |                       |
/// |   game  time          |
/// |                       |
/// +-----------------------+
/// |relative piece value   |
/// |piece value lhs        |
/// |piace value rhs        |
/// +-----------------------+
/// |relative activity      |
/// |activity lhs           |
/// |activity rhs           |
/// +-----------------------+
/// |relative protectedness |
/// |protectedness lhs      |
/// |protectedness rhs      |
/// +-----------------------+
/// ```
class game_info_layout
{
public:

  game_info_layout(const screen_rect& square_rect);

  const screen_rect& get_background() const noexcept { return m_background; }
  const screen_rect& get_time() const noexcept { return m_time; }

private:

  screen_rect m_background;
  screen_rect m_time;
};

void test_game_info_layout();

#endif // GAME_INFO_LAYOUT_H
