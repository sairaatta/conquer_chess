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

double get_attack_speed(const race r) noexcept
{
  switch (r)
  {
    case race::classic: return 1.0;
    case race::keiron: return 1.1;
    case race::genetron: return 1.0;
    default:
    case race::xayid:
      assert(r == race::xayid);
      return 0.9;
  }
}

double get_max_health(const race r)
{
  switch (r)
  {
    case race::classic: return 1.0;
    case race::keiron: return 0.5;
    case race::genetron: return 0.75;
    default:
    case race::xayid:
      assert(r == race::xayid);
      return 0.5;
  }
}

double get_max_shield(const race r)
{
  switch (r)
  {
    case race::classic: return 0.0;
    case race::keiron: return 0.5;
    case race::genetron: return 0.0;
    default:
    case race::xayid:
      assert(r == race::xayid);
      return 0.0;
  }
}

double get_movement_speed(const race r) noexcept
{
  switch (r)
  {
    case race::classic: return 1.0;
    case race::keiron: return 0.9;
    case race::genetron: return 1.0;
    default:
    case race::xayid:
      assert(r == race::xayid);
      return 1.1;
  }
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
  // get_attack_speed
  {
    for (const auto c: get_all_races())
    {
      assert(get_attack_speed(c) > 0.0);
    }
  }
  // get_max_health
  {
    assert(get_max_health(race::classic) == 1.0);
    assert(get_max_health(race::keiron) == 0.5);
    assert(get_max_health(race::genetron) == 0.75);
    assert(get_max_health(race::xayid) == 0.5);
  }
  // get_max_shield
  {
    assert(get_max_shield(race::classic) == 0.0);
    assert(get_max_shield(race::keiron) == 0.5);
    assert(get_max_shield(race::genetron) == 0.0);
    assert(get_max_shield(race::xayid) == 0.0);

  }
  // get_movement_speed
  {
    for (const auto c: get_all_races())
    {
      assert(get_movement_speed(c) > 0.0);
    }
  }
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
    assert(to_human_str(race::xayid) == "Xayid");
    assert(to_human_str(race::keiron) == "Keiron");
    assert(to_human_str(race::genetron) == "Genetron");
  }
  // to_str
  {
    assert(to_str(race::classic) == "classic");
    assert(to_str(race::xayid) == "xayid");
    assert(to_str(race::keiron) == "keiron");
    assert(to_str(race::genetron) == "genetron");
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
    s << race::xayid;
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
