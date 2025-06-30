#ifndef MOUSE_BINDINGS_H
#define MOUSE_BINDINGS_H

#include "ccfwd.h"
#include "action_number.h"

#include <SFML/Window/Mouse.hpp>

#include <iosfwd>
#include <map>

/// The mouse bindings.
///
/// This class connects a mouse button to a mouse action:
///
/// - do the selected action
/// - select the next action
class mouse_bindings
{
public:
  mouse_bindings(
    const sf::Mouse::Button button_1 = sf::Mouse::Button::Left,
    const sf::Mouse::Button button_2 = sf::Mouse::Button::Right
  );

  /// Get the button to do the selected action
  const sf::Mouse::Button& get_button_for_action(const action_number& n) const;

private:

  std::map<action_number, sf::Mouse::Button> m_buttons;

};

/// Test this class and its free functions
void test_mouse_bindings();

bool operator==(const mouse_bindings& lhs, const mouse_bindings& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const mouse_bindings& keys) noexcept;

#endif // MOUSE_BINDINGS_H
