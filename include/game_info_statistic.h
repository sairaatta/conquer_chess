#ifndef GAME_INFO_STATISTIC_H
#define  GAME_INFO_STATISTIC_H

#include <iosfwd>
#include <string>
#include <vector>

enum class game_info_statistic
{
  time,
  value,
  activity,
  protectedness
};

/// Get all the possible values of \link{game_info_statistic}
std::vector<game_info_statistic> get_all_game_info_statistics() noexcept;

/// Get the next controller type,
/// i.e. when the player presses right
game_info_statistic get_next(const game_info_statistic t) noexcept;

/// Get the previous controller type,
/// i.e. when the player presses left
game_info_statistic get_previous(const game_info_statistic t) noexcept;

/// Test this class and its free functions
void test_game_info_statistic();

/// Convert to string to be read by a human
std::string to_human_str(const game_info_statistic t) noexcept;

/// Convert to string of the same name as the enum
std::string to_str(const game_info_statistic t) noexcept;

std::ostream& operator<<(std::ostream& os, const game_info_statistic t) noexcept;

#endif // GAME_INFO_STATISTIC_H
