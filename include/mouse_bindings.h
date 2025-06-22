#ifndef MOUSE_BINDINGS_H
#define MOUSE_BINDINGS_H

#include "ccfwd.h"
//#include "user_input_type.h"

#include <SFML/Window/Mouse.hpp>
//#include <SFML/Window/Event.hpp>

#include <iosfwd>

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
    const sf::Mouse::Button do_button = sf::Mouse::Button::Left,
    const sf::Mouse::Button next_button = sf::Mouse::Button::Right
  );

  /* Unsure if this is relevant for a mouse
  /// From an event, create zero or one user inputs.
  /// Return the 'user_input_type' corresponding to an event
  std::vector<user_input_type> create_actions(
    const sf::Mouse::Button k
  ) const noexcept;
  */

  /// Get the button to do the selected action
  const sf::Mouse::Button& get_do_button() const noexcept { return m_do_button; }

  /// Get the button to select the next action
  const sf::Mouse::Button& get_next_button() const noexcept { return m_next_button; }

private:

  sf::Mouse::Button m_do_button;
  sf::Mouse::Button m_next_button;
};

/* Unsure if this is relevant for a mouse
/// Get the key for a specific action
sf::Mouse::Button get_key_for_action(
  const mouse_bindings& k,
  const action_number& action
);
*/

/// Test this class and its free functions
void test_mouse_bindings();

bool operator==(const mouse_bindings& lhs, const mouse_bindings& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const mouse_bindings& keys) noexcept;

#endif // MOUSE_BINDINGS_H
