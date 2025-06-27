#include "in_game_controls_layout.h"

#include <cassert>
#include <sstream>

in_game_controls_layout::in_game_controls_layout(const screen_rect& r)
  : m_background{r}
{
  assert(r == m_background);
  const int symbol_height{64};
  const int symbol_width{64};
  assert(get_width(r) > symbol_width + 2);
  assert(get_height(r) >= symbol_height);

  // Navigation controls
  {
    const int x_mid{(r.get_tl().get_x() + r.get_br().get_x()) / 2};
    const int x1{x_mid - ((3 * 64) / 2)};
    const int x2{x1 + (3 * 64)};
    const int y1{r.get_tl().get_y()};
    const int y2{y1 + (2 * 64)};
    assert(x1 < x2);
    m_navigation_controls = navigation_controls_layout(
      screen_rect(
        screen_coordinate(x1, y1),
        screen_coordinate(x2, y2)
        )
      );
  }
  // Keys for actions
  {

    for (const auto n: get_all_action_numbers())
    {
      const int x1{r.get_tl().get_x()};
      const int x2{x1 + symbol_width};
      const int x3{r.get_br().get_x()};
      assert(x1 < x2);
      assert(x2 < x3);

      const int y1{r.get_tl().get_y() + ((1 + n.get_number()) * 64)};
      const int y2{y1 + symbol_height};
      assert(y1 < y2);

      const screen_rect symbol_rect{
        screen_coordinate(x1, y1),
        screen_coordinate(x2, y2)
      };
      const auto text_rect{
        screen_rect(
          screen_coordinate(x2, y1),
          screen_coordinate(x3, y2)
        )
      };
      m_action_key_symbol[n] = symbol_rect;
      m_action_key_text[n] = text_rect;
    }
  }
}

const screen_rect& in_game_controls_layout::get_action_key_symbol(
  const action_number& n
) const noexcept
{
  return m_action_key_symbol.at(n);
}

const screen_rect& in_game_controls_layout::get_action_key_text(
  const action_number& n
) const noexcept
{
  return m_action_key_text.at(n);
}

void test_in_game_controls_layout()
{
#ifndef NDEBUG
  // get_action_key_symbol
  {
    const in_game_controls_layout layout;
    const auto r1{layout.get_action_key_symbol(action_number(1))};
    const auto r2{layout.get_action_key_symbol(action_number(2))};
    const auto r3{layout.get_action_key_symbol(action_number(3))};
    const auto r4{layout.get_action_key_symbol(action_number(4))};
    assert(r1 != r2);
    assert(r1 != r3);
    assert(r1 != r4);
    assert(r2 != r3);
    assert(r2 != r4);
    assert(r3 != r4);

  }
  // get_action_key_text
  {
    const in_game_controls_layout layout;
    const auto r1{layout.get_action_key_text(action_number(1))};
    const auto r2{layout.get_action_key_text(action_number(2))};
    const auto r3{layout.get_action_key_text(action_number(3))};
    const auto r4{layout.get_action_key_text(action_number(4))};
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
  for (const auto n: get_all_action_numbers())
  {
    s
      << "Controls for action key " << n << ": "
      << layout.get_action_key_symbol(action_number(1))
      << " with text "
      << layout.get_action_key_text(action_number(1))
      << '\n'
    ;
  }

  std::string t{s.str()};
  assert(!t.empty());
  t.pop_back(); // the newline
  os << t;
  return os;
}

