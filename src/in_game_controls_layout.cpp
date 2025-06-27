#include "in_game_controls_layout.h"

#include <cassert>
#include <sstream>

in_game_controls_layout::in_game_controls_layout(const screen_rect& r)
  : m_background{r}
{
  const int x_mid{(r.get_tl().get_x() + r.get_br().get_x()) / 2};
  const int cx1{x_mid - ((3 * 64) / 2)};
  const int cx2{cx1 + (3 * 64)};
  const int cy1{r.get_tl().get_y()};
  const int cy2{cy1 + (2 * 64)};
  m_navigation_controls = navigation_controls_layout(
    screen_rect(
      screen_coordinate(cx1, cy1),
      screen_coordinate(cx2, cy2)
    )
  );
  for (const auto n: get_all_action_numbers())
  {
    m_controls_key[n] = screen_rect(
      r.get_tl() + screen_coordinate(0 * 64, (1 + n.get_number()) * 64),
      screen_coordinate(r.get_br().get_x(), r.get_tl().get_y() + (2 + n.get_number()) * 64)
    );
  }
}

const screen_rect& in_game_controls_layout::get_controls_key(
  const action_number& key
) const noexcept
{
  return m_controls_key.at(key);
}

void test_in_game_controls_layout()
{
#ifndef NDEBUG
  {
    const in_game_controls_layout layout;
    const auto r1{layout.get_controls_key(action_number(1))};
    const auto r2{layout.get_controls_key(action_number(2))};
    const auto r3{layout.get_controls_key(action_number(3))};
    const auto r4{layout.get_controls_key(action_number(4))};
    assert(r1 != r2);
    assert(r1 != r3);
    assert(r1 != r4);
    assert(r2 != r3);
    assert(r2 != r4);
    assert(r3 != r4);

  }
  // operator<<
  {
    const in_game_controls_layout layout;
    std::stringstream s;
    s << layout;
    assert(!s.str().empty());

  }
#endif // NDEBUG

}


std::ostream& operator<<(std::ostream& os, const in_game_controls_layout& layout) noexcept
{
  std::stringstream s;
  s
    << "Controls key 1: " << layout.get_controls_key(action_number(1)) << '\n'
    << "Controls key 2: " << layout.get_controls_key(action_number(2)) << '\n'
    << "Controls key 3: " << layout.get_controls_key(action_number(3)) << '\n'
    << "Controls key 4: " << layout.get_controls_key(action_number(4)) << '\n'
  ;

  std::string t{s.str()};
  assert(!t.empty());
  t.pop_back(); // the newline
  os << t;
  return os;
}

