#include "piece_type.h"

#include <cassert>
#include <iostream>
#include <sstream>

std::vector<piece_type> get_all_piece_types() noexcept
{
  return
  {
    piece_type::bishop,
    piece_type::king,
    piece_type::knight,
    piece_type::pawn,
    piece_type::queen,
    piece_type::rook
  };
}

int get_piece_value(const piece_type type)
{
  switch (type)
  {
    case piece_type::bishop: return 3;
    case piece_type::king: return 0;
    case piece_type::knight: return 3;
    case piece_type::pawn: return 1;
    case piece_type::queen: return 10;
    default:
    case piece_type::rook:
      assert(type == piece_type::rook);
      return 5;
  }
}

void test_piece_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(piece_type::king) == "king");
    assert(to_str(piece_type::pawn) == "pawn");
    assert(to_str(piece_type::rook) == "rook");
    assert(to_str(piece_type::queen) == "queen");
    assert(to_str(piece_type::bishop) == "bishop");
    assert(to_str(piece_type::knight) == "knight");
  }
  // to_str, all
  {
    for (const auto t: get_all_piece_types())
    {
      assert(!to_str(t).empty());
    }
  }
  // get_piece_value
  {
    assert(get_piece_value(piece_type::bishop) == 3);
    assert(get_piece_value(piece_type::king) == 0);
    assert(get_piece_value(piece_type::knight) == 3);
    assert(get_piece_value(piece_type::pawn) == 1);
    assert(get_piece_value(piece_type::queen) == 10);
    assert(get_piece_value(piece_type::rook) == 5);
  }
  // to_piece_type
  {
    assert(to_piece_type('B') == piece_type::bishop);
    assert(to_piece_type('K') == piece_type::king);
    assert(to_piece_type('N') == piece_type::knight);
    assert(to_piece_type('Q') == piece_type::queen);
    assert(to_piece_type('R') == piece_type::rook);
    assert(to_piece_type(' ') == piece_type::pawn);
  }
  // operator<<
  {
    std::stringstream s;
    s << piece_type::king;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

piece_type to_piece_type(const char c)
{
  switch (c)
  {
    case 'B': return piece_type::bishop;
    case 'K': return piece_type::king;
    case 'N': return piece_type::knight;
    case 'Q': return piece_type::queen;
    case 'R': return piece_type::rook;
    default:
    case ' ':
      assert(c == ' ');
      return piece_type::pawn;
  }
}

std::string to_str(const piece_type type) noexcept
{
  switch (type)
  {
    case piece_type::bishop: return "bishop";
    case piece_type::king: return "king";
    case piece_type::knight: return "knight";
    case piece_type::queen: return "queen";
    case piece_type::pawn: return "pawn";
    default:
    case piece_type::rook:
      assert(type == piece_type::rook);
      return "rook";
  }
}

std::ostream& operator<<(std::ostream& os, const piece_type type) noexcept
{
  os << to_str(type);
  return os;
}
