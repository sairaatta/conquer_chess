#ifndef WHEN_TO_MAKE_A_MOVE_LAW
#define WHEN_TO_MAKE_A_MOVE_LAW

#include <string>
#include <vector>

/// @file The law when to make a move
enum class when_to_make_a_move_law {
  classic,
  rts,
  white_first
};

/// Get all the message_type values
std::vector<when_to_make_a_move_law> get_all_when_to_make_a_move_laws() noexcept;

/// Test this class and its free functions
void test_when_to_make_a_move_law();

/// Convert the type to a string that spelled identical to the C++ value
/// @example
/// assert(to_str(when_to_make_a_move_law::classic) == "classic");
std::string to_str(const when_to_make_a_move_law t) noexcept;

#endif // WHEN_TO_MAKE_A_MOVE_LAW
