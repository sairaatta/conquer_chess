#include "game_statistics_in_time.h"

#include "game.h"

#include <cassert>

game_statistics_in_time::game_statistics_in_time()
{
}

void game_statistics_in_time::add(const game& g)
{
  m_statistics.push_back(game_statistics(g));
}

void test_game_statistics_in_time()
{
  #ifndef NDEBUG
  {
    game_statistics_in_time s;
    assert(s.get().size() == 0);
    const game g{create_game_with_standard_starting_position()};
    s.add(g);
    assert(s.get().size() == 1);
  }
  #endif

}

