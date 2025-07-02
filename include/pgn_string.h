#ifndef PGN_STRING_H
#define PGN_STRING_H

#include <string>
#include <vector>

/// A string that is a PGN notation string.
///
/// For example, a game with a scholar's mate can have the PGN string
/// `1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0`
class pgn_string
{
public:
  pgn_string(const std::string& pgn_str = "");

  const auto& get() const noexcept { return m_pgn_str; }

private:
  std::string m_pgn_str;
};

pgn_string get_replay_1_as_pgn_str() noexcept;

pgn_string get_scholars_mate_as_pgn_str() noexcept;

/// Split the PGN string into its moves
/// E.g. '1. e4 e5 2. Nc3' will be split into {'e4', 'e5', 'Nc3'}
std::vector<std::string> split_pgn_str(const pgn_string& pgn_str);

void test_pgn_string();



#endif // PGN_STRING_H
