#ifndef CLI_OPTIONS_H
#define CLI_OPTIONS_H

#include <string>
#include <vector>

/// Command line options
class cli_options
{
public:
  cli_options(const std::vector<std::string>& args = {});

  auto get_do_profile() const noexcept { return m_do_profile; }
  auto get_do_test() const noexcept { return m_do_test; }
  auto get_do_play_standard_random_game() const noexcept { return m_do_play_standard_random_game; }

private:

  bool m_do_profile{false};
  bool m_do_test{true};
  bool m_do_play_standard_random_game{false};
};

/// Get all the valid CLI arguments
std::vector<std::string> get_all_cli_args() noexcept;

/// Is this a valid CLI argument?
bool is_valid_cli_arg(const std::string& arg);

/// Convert CLI arguments to string
std::vector<std::string> collect_args(int argc, char **argv);


/// Test these functions
void test_cli_options();

#endif // CLI_OPTIONS_H
