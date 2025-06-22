#include "action_history.h"

#include "delta_t.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

action_history::action_history(
  const std::vector<std::pair<in_game_time, piece_action>>& timed_actions
) : m_timed_actions{timed_actions}
{
  assert(
    std::is_sorted(
      std::begin(m_timed_actions),
      std::end(m_timed_actions),
      [](const auto& lhs, const auto& rhs)
      {
        return lhs.first < rhs.first;
      }
    )
  );
}

void action_history::add_action(const in_game_time& in_game_time, const piece_action& action) noexcept
{
  m_timed_actions.push_back(std::make_pair(in_game_time, action));
}

std::vector<piece_action> collect_actions_in_timespan(
  const action_history& history,
  const in_game_time from,
  const in_game_time to
)
{
  assert(from < to);
  std::vector<piece_action> actions;
  for (const auto& p: history.get_timed_actions())
  {
    const in_game_time& t{p.first};
    if (t >= from && t <= to) actions.push_back(p.second);
  }
  return actions;
}

const piece_action& get_last_action(const action_history& history)
{
  assert(has_actions(history));
  return history.get_timed_actions().back().second;
}

bool has_actions(const action_history& history) noexcept
{
  return !history.get_timed_actions().empty();
}

bool has_just_double_moved(
  const action_history& history,
  const in_game_time when
)
{
  // It takes 1 time unit to do a move
  const in_game_time start_earliest{
    when - delta_t(1.0) - delta_t(1.0)};
  const in_game_time start_latest{when - delta_t(1.0)};
  const std::vector<piece_action> actions{
    collect_actions_in_timespan(
      history,
      start_earliest,
      start_latest
    )
  };
  for (const piece_action a: actions)
  {
    if (is_double_move(a)) return true;
  }
  return false;
}

action_history merge_action_histories(const std::vector<action_history> histories)
{
  std::vector<std::pair<in_game_time, piece_action>> timed_actions;
  for (const auto& history: histories)
  {
    const auto tas{history.get_timed_actions()};
    std::copy(std::begin(tas), std::end(tas), std::back_inserter(timed_actions));
  }
  std::sort(
    std::begin(timed_actions),
    std::end(timed_actions),
    [](const auto& lhs, const auto& rhs)
    {
      return lhs.first < rhs.first;
    }
  );
  return action_history(timed_actions);
}

void test_action_history()
{
#ifndef NDEBUG
  // upon construction there are timed actions
  {
    const action_history h;
    assert(h.get_timed_actions().empty());
  }
  // merge_action_histories
  {
    const action_history a{
      {
        std::make_pair(in_game_time(0.1), get_test_piece_action()),
        std::make_pair(in_game_time(0.3), get_test_piece_action())
      }
    };

    const action_history b{
      {
        std::make_pair(in_game_time(0.2), get_test_piece_action()),
        std::make_pair(in_game_time(0.4), get_test_piece_action())
      }
    };
    const auto c{merge_action_histories( { a, b } ) };
    assert(
      a.get_timed_actions().size() + b.get_timed_actions().size()
      == c.get_timed_actions().size()
    );
  }
  // operator<<
  {
    std::stringstream s;
    action_history h;
    s << h; // Still empty
    assert(s.str().empty());
    h.add_action(
      in_game_time(1.0),
      get_test_piece_action()
    );
    s << h;
    assert(!s.str().empty());
  }
#endif
}

std::string to_str(const action_history& history) noexcept
{
  std::stringstream s;
  for (const auto& timed_action: history.get_timed_actions())
  {
    s << to_human_str(timed_action.first) << ": " << timed_action.second << '\n';
  }
  std::string t{s.str()};
  if (!t.empty()) t.pop_back();
  return t;
}

std::ostream& operator<<(std::ostream& os, const action_history& history) noexcept
{
  os << to_str(history);
  return os;
}
