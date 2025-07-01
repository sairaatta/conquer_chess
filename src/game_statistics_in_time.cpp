#include "game_statistics_in_time.h"

#include "game.h"
#include "game_controller.h"
#include "lobby_options.h"
#include <cassert>

game_statistics_in_time::game_statistics_in_time()
{
}

void game_statistics_in_time::add(const game& g)
{
  m_statistics.push_back(game_statistics(g));
}

void game_statistics_in_time::add(const game_statistics& s)
{
  m_statistics.push_back(s);
}

game_statistics_in_time create_test_game_statistics_in_time()
{
  auto s = create_test_game_statistics();
  game_statistics_in_time t;

  for (int i=0; i!=100; ++i)
  {
    t.add(s);
  }
  return t;
}

game_statistics_in_time play_random_game_to_get_statistics_in_time(const int n_turns)
{
  game g{create_game_with_standard_starting_position()};
  game_controller c{create_game_controller_with_two_keyboards()};
  use_default_lobby_options();
  game_statistics_in_time s;

  std::random_device rd;
  std::default_random_engine rng_engine(rd());

  for (int i=0; i!=n_turns; ++i)
  {
    c.add_user_input(create_useful_random_user_input(rng_engine));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.1));
    s.add(g);
    if (g.get_winner().has_value()) break;
  }
  return s;
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
  // create_test_game_statistics_in_time
  {
    const game_statistics_in_time s{create_test_game_statistics_in_time()};
    assert(!s.get().empty());
  }
  #endif

}

