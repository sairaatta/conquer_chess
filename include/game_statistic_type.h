#ifndef GAME_STATISTIC_TYPE_H
#define  GAME_STATISTIC_TYPE_H

#include <iosfwd>
#include <string>
#include <vector>

enum class game_statistic_type
{
  time,
  value,
  activity,
  protectedness
};

/// Get all the possible values of \link{game_statistic_type}
std::vector<game_statistic_type> get_all_game_statistic_types() noexcept;

/// Get the next controller type,
/// i.e. when the player presses right
game_statistic_type get_next(const game_statistic_type t) noexcept;

/// Get the previous controller type,
/// i.e. when the player presses left
game_statistic_type get_previous(const game_statistic_type t) noexcept;

/// Test this class and its free functions
void test_game_statistic_type();

/// Convert to string to be read by a human
std::string to_human_str(const game_statistic_type t) noexcept;

/// Convert to string of the same name as the enum
std::string to_str(const game_statistic_type t) noexcept;

std::ostream& operator<<(std::ostream& os, const game_statistic_type t) noexcept;

#endif // GAME_STATISTIC_TYPE_H
