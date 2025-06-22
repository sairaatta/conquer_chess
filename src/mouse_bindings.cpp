#include "mouse_bindings.h"

//#include "action_number.h"

#include <cassert>
#include <iostream>
#include <sstream>

mouse_bindings::mouse_bindings(
  const sf::Mouse::Button do_button,
  const sf::Mouse::Button next_button
) : m_do_button{do_button},
    m_next_button{next_button}
{

}

/* Unsure if this is relevant for a mouse
std::vector<user_input_type> mouse_bindings::create_actions(
  const sf::Mouse::Button key_pressed
) const noexcept
{
  if (key_pressed == get_key_for_move_up())
  {
    return { user_input_type::press_up };
  }
  else if (key_pressed == get_key_for_move_right())
  {
    return { user_input_type::press_right };
  }
  else if (key_pressed == get_key_for_move_down())
  {
    return { user_input_type::press_down };
  }
  else if (key_pressed == get_key_for_move_left())
  {
    return { user_input_type::press_left };
  }
  else if (key_pressed == get_key_for_action(action_number(1)))
  {
    return { user_input_type::press_action_1 };
  }
  else if (key_pressed == get_key_for_action(action_number(2)))
  {
    return { user_input_type::press_action_2 };
  }
  else if (key_pressed == get_key_for_action(action_number(3)))
  {
    return { user_input_type::press_action_3 };
  }
  else if (key_pressed == get_key_for_action(action_number(4)))
  {
    return { user_input_type::press_action_4 };
  }
  return {};

}
sf::Mouse::Button mouse_bindings::get_key_for_action(const action_number& number) const
{
  const int i{number.get_number() - 1};
  assert(i >= 0);
  assert(i < static_cast<int>(m_actions.size()));
  return m_actions[i];
}
*/

void test_mouse_bindings()
{
#ifndef NDEBUG
  // mouse_bindings::mouse_bindings
  {
    const mouse_bindings k;
    assert(k.get_do_button() == sf::Mouse::Button::Left);
    assert(k.get_do_button() == sf::Mouse::Button::Right);
  }
  /* Unsure if this is relevant for a mouse
  // create_actions
  {
    const mouse_bindings k{create_left_keyboard_mouse_bindings()};
    assert(!k.create_actions(k.get_key_for_move_up()).empty());
    assert(!k.create_actions(k.get_key_for_move_right()).empty());
    assert(!k.create_actions(k.get_key_for_move_down()).empty());
    assert(!k.create_actions(k.get_key_for_move_left()).empty());
    assert(!k.create_actions(k.get_key_for_action(action_number(1))).empty());
    assert(!k.create_actions(k.get_key_for_action(action_number(2))).empty());
    assert(!k.create_actions(k.get_key_for_action(action_number(3))).empty());
    assert(!k.create_actions(k.get_key_for_action(action_number(4))).empty());
    const mouse_bindings l{create_right_keyboard_mouse_bindings()};
    assert(k.create_actions(l.get_key_for_move_up()).empty());
    assert(k.create_actions(l.get_key_for_move_right()).empty());
    assert(k.create_actions(l.get_key_for_move_down()).empty());
    assert(k.create_actions(l.get_key_for_move_left()).empty());
    assert(k.create_actions(l.get_key_for_action(action_number(1))).empty());
    assert(k.create_actions(l.get_key_for_action(action_number(2))).empty());
    assert(k.create_actions(l.get_key_for_action(action_number(3))).empty());
    assert(k.create_actions(l.get_key_for_action(action_number(4))).empty());
  }
  // get_key_for_action
  {
    const mouse_bindings k{create_left_keyboard_mouse_bindings()};
    assert(k.get_key_for_action(action_number(1)) == get_key_for_action(k, action_number(1)));
  }
  */
  // operator==
  {
    const mouse_bindings mbs1(sf::Mouse::Button::Left, sf::Mouse::Button::Right);
    const mouse_bindings mbs2(sf::Mouse::Button::Left, sf::Mouse::Button::Right);
    const mouse_bindings mbs3(sf::Mouse::Button::Left, sf::Mouse::Button::Middle);
    const mouse_bindings mbs4(sf::Mouse::Button::Right, sf::Mouse::Button::Left);
    assert(mbs1 == mbs2);
    assert(!(mbs1 == mbs3));
    assert(!(mbs1 == mbs4));
    assert(!(mbs3 == mbs4));
  }
  // operator<<
  {
    const mouse_bindings mbs;
    std::stringstream s;
    s << mbs;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

bool operator==(const mouse_bindings& lhs, const mouse_bindings& rhs) noexcept
{
  return lhs.get_do_button() == rhs.get_do_button()
    && lhs.get_next_button() == rhs.get_next_button()
  ;
}

std::ostream& operator<<(std::ostream& os, const mouse_bindings& mbs) noexcept
{
  os
    << "Button for doing it: " << mbs.get_do_button() << '\n'
    << "Button for doing next: " << mbs.get_next_button() << '\n'
  ;
  return os;
}
