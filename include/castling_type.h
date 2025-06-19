#ifndef CASTLING_TYPE_H
#define CASTLING_TYPE_H

#include <string>
#include <vector>

/// The type of castling.
enum class castling_type
{
  king_side,
  queen_side
};

/// Get all the message_type values
std::vector<castling_type> get_all_castling_types() noexcept;

/// Test this class and its free functions
void test_castling_type();

std::string to_str(const castling_type t) noexcept;


#endif // CASTLING_TYPE_H
