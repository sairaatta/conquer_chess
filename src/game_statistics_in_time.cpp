#include "game_statistics_in_time.h"

#include "game.h"
#include "game_controller.h"
#include "lobby_options.h"
#include <cassert>

game_statistics_in_time::game_statistics_in_time()
{
}

void game_statistics_in_time::add(const game_controller& c)
{
  m_statistics.push_back(game_statistics(c));
}

void game_statistics_in_time::add(const game_statistics& s)
{
  m_statistics.push_back(s);
}

game_statistics_in_time create_test_game_statistics_in_time()
{
  auto s = create_test_game_statistics();
  game_statistics_in_time t;

  for (int i=0; i!=2; ++i)
  {
    t.add(s);
  }
  return t;
}

game_statistics_in_time play_random_game_to_get_statistics_in_time(const int n_turns)
{
  game_controller c(
    create_game_with_standard_starting_position(),
    lobby_options()
  );
  game_statistics_in_time s;

  std::random_device rd;
  std::default_random_engine rng_engine(rd());

  for (int i=0; i!=n_turns; ++i)
  {
    c.add_user_input(create_useful_random_user_input(rng_engine));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.1));
    s.add(c);
    if (get_winner(c).has_value()) break;
  }
  return s;
}

void test_game_statistics_in_time()
{
  #ifndef NDEBUG
  {
    game_statistics_in_time s;
    assert(s.get().size() == 0);
    const game_controller c;
    s.add(c);
    assert(s.get().size() == 1);
  }
  // create_test_game_statistics_in_time
  {
    const game_statistics_in_time s{create_test_game_statistics_in_time()};
    assert(!s.get().empty());
  }
  // play_random_game_to_get_statistics_in_time
  {
    const int n{2};
    const auto g{play_random_game_to_get_statistics_in_time(n)};
    assert(n == static_cast<int>(g.get().size()));
  }
  #endif

}

