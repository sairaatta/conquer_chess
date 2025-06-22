#ifndef PHYSICAL_CONTROLLERS_H
#define PHYSICAL_CONTROLLERS_H

#include "ccfwd.h"
#include "physical_controller.h"

#include <vector>

/// The physical controllers used by all players.
class physical_controllers
{
public:

  /// This class is a Singleton
  physical_controllers(const physical_controllers&) = delete;
  physical_controllers& operator=(const physical_controllers&) = delete;
  physical_controllers(physical_controllers &&) = delete;
  physical_controllers& operator=(physical_controllers&&) = delete;
  static auto& get(){
    static physical_controllers r;
    return r;
  }

  /// Get the controller of a player
  const physical_controller& get_controller(const side player_side) const noexcept;

  /// Get the controller of a player
  physical_controller& get_controller(const side player_side) noexcept;

  /// Get the physical controllers
  const std::vector<physical_controller>& get_controllers() const noexcept;

  /// Set a controller
  void set(const side player_side, const physical_controller& controller);

private:
  physical_controllers(
    const std::vector<physical_controller> controllers = {
      create_left_keyboard_controller(),
      create_default_mouse_controller()
    }
  );

  std::vector<physical_controller> m_physical_controllers;
};

/// Count the number of controllers that use the keyboard
int count_keyboard_controllers(const physical_controllers& controllers) noexcept;

/// Count the number of controllers that use the keyboard
int count_mouse_controllers(const physical_controllers& controllers) noexcept;

/// Count the number of controllers
int count_n_controllers(const physical_controllers& controllers) noexcept;


/// Get two controllers,
/// where LHS player uses keyboard,
/// and RHS player uses mouse
///
/// Alternatives:
/// - \link{use_keyboard_mouse_controllers}
/// - \link{use_mouse_keyboard_controllers}
/// - \link{use_two_keyboard_controllers}
void use_keyboard_mouse_controllers() noexcept;

/// Get two controllers,
/// where LHS player uses mouse,
/// and RHS player uses keyboard
///
/// Alternatives:
/// - \link{use_keyboard_mouse_controllers}
/// - \link{use_mouse_keyboard_controllers}
/// - \link{use_two_keyboard_controllers}
void use_mouse_keyboard_controllers() noexcept;

/// Get two keyboard controllers.
/// @see use \link{create_default_controllers}
/// to create the defaults one-keyboard-one-mouse controllers
///
/// Alternatives:
/// - \link{use_keyboard_mouse_controllers}
/// - \link{use_mouse_keyboard_controllers}
/// - \link{use_two_keyboard_controllers}
void use_two_keyboard_controllers() noexcept;

/// Get the side of the controller that uses the keyboard.
/// Assumes there is one controller that uses the keyboard
side get_keyboard_user_player_side(const physical_controllers& controllers);

/// Get the side of the controller that uses the mouse.
/// Assumes there is one controller that uses the mouse
side get_mouse_user_player_side(const physical_controllers& controllers);

/// Determine if there is at least one controller that use the keyboard
bool has_keyboard_controller(const physical_controllers& controllers) noexcept;

/// Determine if there is at least one controller that use the mouse
bool has_mouse_controller(const physical_controllers& controllers) noexcept;

/// Test this class and its free functions
void test_physical_controllers();

#endif // PHYSICAL_CONTROLLERS_H
