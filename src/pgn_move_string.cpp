#include "pgn_move_string.h"

#include <cassert>
#include <sstream>
#include <regex>

pgn_move_string::pgn_move_string(const std::string& pgn_str)
  : m_pgn_str{pgn_str}
{

}

std::optional<piece_type> get_piece_type(const pgn_move_string& s)
{
  if (!get_square(s).has_value()) return {};

  // Needs to start with a captital,
  // else 'e8=Q' gets interpreted as a queen move
  const std::regex big_piece_regex("^[BKNQR]");
  std::smatch m;
  const bool is_big_piece_there{std::regex_search(s.get(), m, big_piece_regex)};
  if (!is_big_piece_there)
  {
     return piece_type::pawn;
  }
  assert(!m.str().empty());
  assert(m.str().size() == 1);
  return to_piece_type(m.str()[0]);
}

std::optional<piece_type> get_promotion_type(const pgn_move_string& s)
{
  const std::regex e("=[BKNQR]");
  std::smatch m;
  const bool is_there{std::regex_search(s.get(), m, e)};
  if (!is_there) return {};
  assert(!m.str().empty());
  assert(m.str().size() == 2);
  assert(m.str()[0] == '=');
  return to_piece_type(m.str()[1]);
}

std::optional<square> get_square(const pgn_move_string& s)
{
  const std::regex e("[a-h][1-8]");
  std::smatch m;
  const bool is_there{std::regex_search(s.get(), m, e)};
  if (is_there)
  {
    return square(m.str());
  }
  return {};
}

std::optional<square> get_to(const pgn_move_string& pgn_str)
{
  return get_square(pgn_str);
}

std::vector<chess_color> get_winner(const pgn_move_string& s)
{
  assert(std::regex_match(s.get(), std::regex("^(0-1)|1-0|(1/2-1/2)$")));
  if (s.get() == "1-0") return {  chess_color::white };
  if (s.get() == "0-1") return {  chess_color::black };
  assert(s.get() == "1/2-1/2");
  return { chess_color::white, chess_color::black };
}

void test_pgn_move_string()
{
  #ifndef NDEBUG
  {
    const pgn_move_string s;
  }
  //get_piece_type
  {
    assert(get_piece_type(pgn_move_string("Nc3")).value() == piece_type::knight);
    assert(get_piece_type(pgn_move_string("e4")).value() == piece_type::pawn);
    assert(get_piece_type(pgn_move_string("e3")).value() == piece_type::pawn);
  }
  //get_square
  {
    assert(get_square(pgn_move_string("Nc3")).value() == square("c3"));
    assert(get_square(pgn_move_string("e4")).value() == square("e4"));
    assert(!get_square(pgn_move_string("1-0")).has_value());
  }
  // get_to
  {
    assert(get_to(pgn_move_string("Bxh6")).value() == square("h6"));
    assert(get_to(pgn_move_string("gxh6")).value() == square("h6"));
    assert(get_to(pgn_move_string("Qe8#")).value() == square("e8"));
    assert(!get_to(pgn_move_string("1-0")).has_value());
  }
  // operator==
  {
    const std::string s{"e4"};
    const pgn_move_string t(s);
    assert(t.get() == s);
  }
  // operator<<
  {
    const pgn_move_string t("e4");
    std::stringstream s;
    s << t;
    assert(!s.str().empty());
  }
  // get_winner
  {
    assert(get_winner(pgn_move_string("0-1")).at(0) == chess_color::black);
    assert(get_winner(pgn_move_string("1-0")).at(0) == chess_color::white);
    assert(get_winner(pgn_move_string("1/2-1/2")).size() == 2);
  }
  #endif
}

bool operator==(const pgn_move_string& lhs, const pgn_move_string& rhs) noexcept
{
  return lhs.get() == rhs.get();
}

std::ostream& operator<<(std::ostream& os, const pgn_move_string& s) noexcept
{
  os << s.get();
  return os;
}
