#ifndef LAWS_H
#define LAWS_H

#include "when_to_make_a_move_law.h"

/// @file The Laws of Chess
class laws
{
public:

  auto get_when_to_make_a_move() const noexcept { return m_when_to_make_a_move; }

private:
  when_to_make_a_move_law m_when_to_make_a_move{when_to_make_a_move_law::rts};
};

/// Test these functions
void test_laws();

#endif // LAWS_H
