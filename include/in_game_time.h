#ifndef IN_GAME_TIME_H
#define IN_GAME_TIME_H

#include "ccfwd.h"
#include <iosfwd>

/// The time, in chess moves.
///
/// The time that passes
/// where
///   * 0.01 means that only 1% of the full move is done
///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
class in_game_time
{
public:
  explicit in_game_time(const double in_game_time);

  double get() const noexcept { return m_in_game_time; }

private:

  double m_in_game_time;
};

/// Test this class and its free functions
void test_in_game_time();

bool operator==(const in_game_time& lhs, const in_game_time& rhs) noexcept;
bool operator<(const in_game_time& lhs, const in_game_time& rhs) noexcept;
bool operator<=(const in_game_time& lhs, const in_game_time& rhs) noexcept;
bool operator>(const in_game_time& lhs, const in_game_time& rhs) noexcept;
bool operator>=(const in_game_time& lhs, const in_game_time& rhs) noexcept;

in_game_time& operator+=(in_game_time& game_time, const delta_t& dt) noexcept;
in_game_time operator+(const in_game_time& game_time, const delta_t& dt) noexcept;
in_game_time operator-(const in_game_time& game_time, const delta_t& dt) noexcept;
delta_t operator-(const in_game_time& lhs, const in_game_time& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const in_game_time& dt) noexcept;

#endif // IN_GAME_TIME_H
