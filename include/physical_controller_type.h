#ifndef PHYSICAL_CONTROLLER_TYPE_H
#define PHYSICAL_CONTROLLER_TYPE_H

#include "ccfwd.h"
#include "side.h"

#include <iosfwd>
#include <string>
#include <vector>

/// The type of controller.
///
/// The type of controller that is supported.
enum class physical_controller_type
{
  keyboard,
  mouse
};


/// Get all the possible values of \link{physical_controller_type}
std::vector<physical_controller_type> get_all_physical_controller_types() noexcept;

/// Get the next controller type,
/// i.e. when the player presses right
physical_controller_type get_next(const physical_controller_type t) noexcept;

/// Get the physical controller type
// TODO: remove this, a game controller has nothing to do with a physical
// game controller
physical_controller_type get_physical_controller_type(
  const game_controller& c,
  const side player_side
) noexcept;

/// Get the previous controller type,
/// i.e. when the player presses left
physical_controller_type get_previous(const physical_controller_type t) noexcept;

/// Test this class and its free functions
void test_physical_controller_type();

/// Convert to string to be read by a human
std::string to_human_str(const physical_controller_type t) noexcept;

/// Convert to string of the same name as the enum
std::string to_str(const physical_controller_type t) noexcept;

std::ostream& operator<<(std::ostream& os, const physical_controller_type t) noexcept;

#endif // PHYSICAL_CONTROLLER_TYPE_H
