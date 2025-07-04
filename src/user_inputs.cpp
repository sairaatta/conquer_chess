#include "user_inputs.h"

#include "game.h"
#include "game_controller.h"
#include "square.h"
#include "game_coordinate.h"
#include "piece.h"
#include "piece_actions.h"

#include <cassert>
#include <iostream>
#include <sstream>

user_inputs::user_inputs(const std::vector<user_input>& inputs)
  : m_user_inputs{inputs}
{

}

void user_inputs::add(const user_input& action)
{
  m_user_inputs.push_back(action);
}

void add(user_inputs& current, const user_inputs& to_be_added)
{
  for (const auto& i: to_be_added.get_user_inputs())
  {
    current.add(i);
  }
}

int count_user_inputs(const user_inputs& a)
{
  return static_cast<int>(a.get_user_inputs().size());
}

void do_select(
  game_controller& c,
  const std::string& square_str,
  const side player_side
)
{
  move_cursor_to(c, square_str, player_side);
  add_user_input(c, create_press_action_1(player_side));
  c.apply_user_inputs_to_game();
}

void do_select_and_move_piece(
  game_controller& c,
  const std::string& from_square_str,
  const std::string& to_square_str,
  const side player_side,
  const physical_controller_type t
)
{
  do_select(c, from_square_str, player_side);
  move_cursor_to(c, to_square_str, player_side);
  add_user_input(
    c,
    get_user_input_to_select(player_side, t)
  );
  user_inputs inputs{c.get_user_inputs()};
  c.apply_user_inputs_to_game();
  c.tick();
}

bool is_empty(const user_inputs& inputs) noexcept
{
  return inputs.get_user_inputs().empty();
}

void start_en_passant_attack(
  game& g,
  const game_controller& c,
  const game_coordinate& coordinat,
  const chess_color player_color
)
{
  const auto actions{collect_all_piece_actions(g)};

  if (count_selected_units(c, player_color) == 0) return;

  for (auto& p: g.get_pieces())
  {
    if (is_selected(p, c) && p.get_color() == player_color)
    {
      const auto& from{p.get_current_square()};
      const auto& to{square(coordinat)};
      const piece_action action(
        p.get_color(),
        p.get_type(),
        piece_action_type::attack_en_passant,
        square(from),
        square(to)
      );
      if (is_in(action, actions))
      {
        clear_actions(p);
        p.add_action(action);
      }
      else
      {
        p.add_message(message_type::cannot);
      }
    }
  }
}

void start_attack(
  game& g,
  const game_controller& c,
  const game_coordinate& coordinat,
  const chess_color player_color
)
{
  const auto actions{collect_all_piece_actions(g)};

  if (count_selected_units(c, player_color) == 0) return;

  for (auto& p: g.get_pieces())
  {
    if (is_selected(p, c) && p.get_color() == player_color)
    {
      const auto& from{p.get_current_square()};
      const auto& to{square(coordinat)};
      const piece_action action(
        p.get_color(),
        p.get_type(),
        piece_action_type::attack,
        square(from),
        square(to)
      );
      if (is_in(action, actions))
      {
        clear_actions(p);
        p.add_action(action);
      }
      else
      {
        p.add_message(message_type::cannot);
      }
    }
  }
}

void start_move_unit(
  game& g,
  const game_controller& c,
  const game_coordinate& coordinat,
  const chess_color player_color
)
{
  if (count_selected_units(c, player_color) == 0) return;

  for (auto& p: g.get_pieces())
  {
    if (is_selected(p, c) && p.get_color() == player_color)
    {

      const auto& from{p.get_current_square()};
      const auto& to{square(coordinat)};
      if (from != to)
      {
        // No shift, so all current actions are void
        clear_actions(p);
        const auto action{
          piece_action(
            p.get_color(),
            p.get_type(),
            piece_action_type::move,
            square(from),
            square(to)
          )
        };
        p.add_action(action);
      }
    }
  }
}

void test_user_inputs()
{
#ifndef NDEBUG
  // Empty on construction
  {
    const user_inputs c;
    assert(c.get_user_inputs().empty());
  }
  // Move up does something
  {
    game_controller c;
    const game_coordinate before{get_cursor_pos(c, side::lhs)};
    c.add_user_input(create_press_up_action(side::lhs));
    c.apply_user_inputs_to_game();
    const game_coordinate after{get_cursor_pos(c, side::lhs)};
    assert(before != after);
  }
  // Move right does something
  {
    game_controller c;
    const game_coordinate before{get_cursor_pos(c, side::lhs)};
    c.add_user_input(create_press_right_action(side::lhs));
    c.apply_user_inputs_to_game();
    const game_coordinate after{get_cursor_pos(c, side::lhs)};
    assert(before != after);
  }
  // Move down does something
  {
    game_controller c;
    const game_coordinate before{get_cursor_pos(c, side::lhs)};
    c.add_user_input(create_press_down_action(side::lhs));
    c.apply_user_inputs_to_game();
    const game_coordinate after{get_cursor_pos(c, side::lhs)};
    assert(before != after);
  }
  // Move left does something
  {
    game_controller c;
    const game_coordinate before{get_cursor_pos(c, side::lhs)};
    c.add_user_input(create_press_left_action(side::lhs));
    c.apply_user_inputs_to_game();
    const game_coordinate after{get_cursor_pos(c, side::lhs)};
    assert(before != after);
  }
  // 37: operator<< for no actions
  {
    const user_inputs actions;
    std::stringstream s;
    s << actions;
    assert(s.str().empty());
  }
  // 37: operator<< for one action
  {
    user_inputs actions;
    actions.add(create_press_action_1(side::lhs));
    std::stringstream s;
    s << actions;
    assert(!s.str().empty());
  }
  // 64: move white's cursor to e2
  {
    game_controller c;
    assert(square(get_cursor_pos(c, side::lhs)) != square("e2"));
    auto inputs{
      get_user_inputs_to_move_cursor_to(c, square("e2"), side::lhs, physical_controller_type::keyboard)
    };
    assert(!is_empty(inputs));
    add_user_inputs(c, inputs);
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));
    assert(square(get_cursor_pos(c, side::lhs)) == square("e2"));
  }
  // move white's cursor to e2 and select the pawn
  {
    game_controller c;
    move_cursor_to(c, "e2", side::lhs);

    assert(!is_selected(get_piece_at(c.get_game(), "e2"), c));
    const user_input input{get_user_input_to_select(side::lhs, physical_controller_type::keyboard)};
    add_user_input(c, input);
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));
    assert(is_selected(get_piece_at(c.get_game(), "e2"), c));
  }
  // do_select
  {
    game_controller c;
    do_select(c, "e2", side::lhs);
    assert(is_selected(get_piece_at(c.get_game(), "e2"), c));
  }
  // do_select_and_move_piece
  {
    game_controller c;
    assert(is_piece_at(c, "e2"));
    do_select_and_move_piece(c, "e2", "e4", side::lhs, physical_controller_type::keyboard);
    assert(!is_piece_at(c, "e2"));
    assert(is_piece_at(c, "e4"));
  }
  // operator== and operator!=
  {
    user_inputs a;
    const user_inputs b;
    assert(a == b);
    assert(!(a != b));
    a.add(create_press_action_1(side::lhs));
    assert(!(a == b));
    assert(a != b);
  }
#endif // NDEBUG
}

bool operator==(const user_inputs& lhs, const user_inputs& rhs) noexcept
{
  return lhs.get_user_inputs() == rhs.get_user_inputs();
}

bool operator!=(const user_inputs& lhs, const user_inputs& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const user_inputs& actions) noexcept
{
  const int n = actions.get_user_inputs().size();
  if (n == 0) return os;
  std::stringstream s;
  for (int i{0}; i!=n; ++i)
  {
    s << i << ": " << actions.get_user_inputs()[i] << ", ";
  }
  std::string u{s.str()};
  assert(!u.empty());
  u.pop_back();
  assert(!u.empty());
  u.pop_back();
  os << u;
  return os;
}
