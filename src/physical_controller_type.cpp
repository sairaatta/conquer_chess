#include "physical_controller_type.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

std::vector<physical_controller_type> get_all_physical_controller_types() noexcept
{
  const auto a{magic_enum::enum_values<physical_controller_type>()};
  std::vector<physical_controller_type> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

physical_controller_type get_next(const physical_controller_type t) noexcept
{
  if (t == physical_controller_type::mouse) return physical_controller_type::keyboard;
  assert(t == physical_controller_type::keyboard);
  return physical_controller_type::mouse;
}

physical_controller_type get_previous(const physical_controller_type t) noexcept
{
  if (t == physical_controller_type::mouse) return physical_controller_type::keyboard;
  assert(t == physical_controller_type::keyboard);
  return physical_controller_type::mouse;
}

void test_physical_controller_type()
{
#ifndef NDEBUG
  // to_human_str
  {
    assert(to_human_str(physical_controller_type::mouse) == "Mouse");
    assert(to_human_str(physical_controller_type::keyboard) == "Keyboard");
  }
  // to_str
  {
    assert(to_str(physical_controller_type::mouse) == "mouse");
    assert(to_str(physical_controller_type::keyboard) == "keyboard");
  }
  // get_next
  {
    assert(get_next(physical_controller_type::mouse) == physical_controller_type::keyboard);
    assert(get_next(physical_controller_type::keyboard) == physical_controller_type::mouse);

  }
  // get_previous
  {
    assert(get_previous(get_next(physical_controller_type::mouse)) == physical_controller_type::mouse);
    assert(get_previous(get_next(physical_controller_type::keyboard)) == physical_controller_type::keyboard);

  }
  // operator<<
  {
    std::stringstream s;
    s << physical_controller_type::mouse;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

std::string to_human_str(const physical_controller_type t) noexcept
{
  std::string s{to_str(t)};
  s[0] = std::toupper(s[0]);
  return s;
}

/*
std::u32string to_symbol(const physical_controller_type t) noexcept
{
  if (t == physical_controller_type::mouse) return std::u32string(U"\U0001F5B0");
  assert(t == physical_controller_type::keyboard);
  return std::u32string(U"\U00002328");
}
*/

std::string to_str(const physical_controller_type t) noexcept
{
  return std::string(magic_enum::enum_name(t));
  /*
  if (t == physical_controller_type::mouse) return "mouse";
  assert(t == physical_controller_type::keyboard);
  return "keyboard";
  */
}

std::ostream& operator<<(std::ostream& os, const physical_controller_type t) noexcept
{
  os << to_str(t);
  return os;
}
