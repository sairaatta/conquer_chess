#ifndef PIECE_ID_H
#define PIECE_ID_H

#include <iosfwd>

/// A unique ID.
///
/// An ID, each one being unique.
///
/// Cannot call this class `id`, as this conflicts with SFML.
class piece_id
{
public:
  int get() const noexcept { return m_value; }

private:
  piece_id();

  static int sm_next_value;

  int m_value;

  friend piece_id create_new_id() noexcept;
};

piece_id create_new_id() noexcept;

/// Test this class and its free functions
void test_id();

bool operator==(const piece_id& lhs, const piece_id& rhs) noexcept;
bool operator!=(const piece_id& lhs, const piece_id& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const piece_id& i) noexcept;

#endif // PIECE_ID_H
