#ifndef DELTA_T_H
#define DELTA_T_H

#include <iosfwd>

/// A change of \link{in_game_time}.
///
/// The change of time that passes can go from:
/// - 0.01 means that only 1% of the full move is done
/// - 1.0 denotes the time a full move takes
class delta_t
{
public:
  explicit delta_t(const double dt);

  double get() const noexcept { return m_delta_t; }

private:

  double m_delta_t;
};

/// Test this class and its free functions
void test_delta_t();

bool operator==(const delta_t& lhs, const delta_t& rhs) noexcept;
bool operator!=(const delta_t& lhs, const delta_t& rhs) noexcept;
bool operator<(const delta_t& lhs, const delta_t& rhs) noexcept;
bool operator<=(const delta_t& lhs, const delta_t& rhs) noexcept;
bool operator>(const delta_t& lhs, const delta_t& rhs) noexcept;
bool operator>=(const delta_t& lhs, const delta_t& rhs) noexcept;

delta_t operator*(const delta_t& lhs, const delta_t& rhs) noexcept;
delta_t operator+(const delta_t& lhs, const delta_t& rhs) noexcept;
delta_t operator-(const delta_t& lhs, const delta_t& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const delta_t& dt) noexcept;

#endif // DELTA_T_H
