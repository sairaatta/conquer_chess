#ifndef RACE_H
#define RACE_H

#include <iosfwd>
#include <string>
#include <vector>

/// The chess race.
enum class race
{
  classic,
  protoss,
  terran,
  zerg
};

std::vector<race> get_all_races() noexcept;

/// Get the attack speed of a race.
///
/// The value of 1.0 is the baseline.
double get_attack_speed(const race r) noexcept;

race get_next(const race r) noexcept;
race get_previous(const race r) noexcept;


/// Get the movement speed of a race.
///
/// The value of 1.0 is the baseline.
double get_movement_speed(const race r) noexcept;

/// Test the race functions
void test_race();

std::string to_human_str(const race c) noexcept;

std::string to_str(const race c) noexcept;

std::ostream& operator<<(std::ostream& os, const race c) noexcept;

#endif // RACE_H
