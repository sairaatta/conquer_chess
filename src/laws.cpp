#include "laws.h"

#include <cassert>

void test_laws()
{
#ifndef NDEBUG
  // constructor
  {
    const laws l;
    assert(l.get_when_to_make_a_move() == when_to_make_a_move_law::rts);
  }
#endif // NDEBUG
}

