#include "race.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

std::vector<race> get_all_races() noexcept
{
  const auto a{magic_enum::enum_values<race>()};
  std::vector<race> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}


race get_next(const race r) noexcept
{
  const auto v{get_all_races()};
  auto there{std::find(std::begin(v), std::end(v), r)};
  assert(there != std::end(v));
  if (there == std::end(v) - 1)
  {
    assert(!v.empty());
    const auto t{v.front()};
    return t;
  }
  return *(++there);
}

race get_previous(const race r) noexcept
{
  const auto v{get_all_races()};
  auto there{std::find(std::begin(v), std::end(v), r)};
  assert(there != std::end(v));
  if (there == std::begin(v))
  {
    assert(!v.empty());
    const auto t{v.back()};
    return t;
  }
  return *(--there);
}

void test_race()
{
#ifndef NDEBUG
  // get_next and get_prev
  {
    for (const auto c: get_all_races())
    {
      assert(get_previous(get_next(c)) == c);
      assert(get_next(get_previous(c)) == c);
    }
  }
  // to_human_str
  {
    assert(to_human_str(race::classic) == "Classic");
    assert(to_human_str(race::zerg) == "Zerg");
    assert(to_human_str(race::protoss) == "Protoss");
    assert(to_human_str(race::terran) == "Terran");
  }
  // to_str
  {
    assert(to_str(race::classic) == "classic");
    assert(to_str(race::zerg) == "zerg");
    assert(to_str(race::protoss) == "protoss");
    assert(to_str(race::terran) == "terran");
  }
  // to_str
  {
    for (const auto c: get_all_races())
    {
      assert(!to_str(c).empty());
    }
  }
  // operator<<
  {
    std::stringstream s;
    s << race::zerg;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

std::string to_human_str(const race c) noexcept
{
  std::string s{to_str(c)};
  assert(!s.empty());
  s[0] = std::toupper(s[0]);
  return s;
}

std::string to_str(const race c) noexcept
{
  return std::string(magic_enum::enum_name(c));
}

std::ostream& operator<<(std::ostream& os, const race c) noexcept
{
  os << to_str(c);
  return os;
}
