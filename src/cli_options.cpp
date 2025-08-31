#include "cli_options.h"

#include <algorithm>
#include <cassert>

cli_options::cli_options(const std::vector<std::string>& args)
{
  const bool do_profile = std::count(std::begin(args), std::end(args), "--profile");
  const bool do_not_profile = std::count(std::begin(args), std::end(args), "--no-profile");
  assert(!(do_profile && do_not_profile));
  if (do_profile) m_do_profile = true;
  if (do_not_profile) m_do_profile = false;

  const bool do_test = std::count(std::begin(args), std::end(args), "--test");
  const bool do_not_test = std::count(std::begin(args), std::end(args), "--no-test");
  assert(!(do_test && do_not_test));
  if (do_test) m_do_test = true;
  if (do_not_test) m_do_test = false;

  const bool do_play_standard_random_game = std::count(std::begin(args), std::end(args), "--play_standard_random_game");
  //const bool do_not_play_standard_random_game = std::count(std::begin(args), std::end(args), "--no-play_standard_random_game");
  //assert(!(do_play_standard_random_game && do_not_play_standard_random_game));
  if (do_play_standard_random_game) m_do_play_standard_random_game = true;
  //if (do_not_play_standard_random_game) m_do_play_standard_random_game = false;
}

std::vector<std::string> collect_args(int argc, char **argv)
{
  std::vector<std::string> v(argv, argv + argc);
  return v;
}

std::vector<std::string> get_all_cli_args() noexcept
{
  return
  {
    "--no-test",
    "--no-profile",
    "--play_standard_random_game",
    "--profile",
    "--test"
  };
}

bool is_valid_cli_arg(const std::string& arg)
{
  const auto valid_args{get_all_cli_args()};
  return std::find(std::begin(valid_args), std::end(valid_args), arg) != std::end(valid_args);
}

void test_cli_options()
{
#ifndef NDEBUG
  // constructor
  {
    const cli_options options;
    assert(options.get_do_test());
  }
  // --play_standard_random_game
  {
    const cli_options options_1;
    assert(!options_1.get_do_play_standard_random_game());
    const cli_options options_2( { "--play_standard_random_game" } );
    assert(options_2.get_do_play_standard_random_game());
  }
  // --profile
  {
    const cli_options options_1( { "--profile" } );
    assert(options_1.get_do_profile());
    const cli_options options_2( { "--no-profile" } );
    assert(!options_2.get_do_profile());
  }
  // --test
  {
    const cli_options options_1( { "--test" } );
    assert(options_1.get_do_test());
    const cli_options options_2( { "--no-test" } );
    assert(!options_2.get_do_test());
  }
  // is_valid_cli_arg
  {
    assert(is_valid_cli_arg("--profile"));
    assert(is_valid_cli_arg("--no-profile"));
    assert(is_valid_cli_arg("--play_standard_random_game"));
    assert(is_valid_cli_arg("--test"));
    assert(is_valid_cli_arg("--no-test"));
  }
#endif // NDEBUG
}

