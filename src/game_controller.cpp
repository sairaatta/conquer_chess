#include "game_controller.h"

#include "chess_move.h"
#include "game.h"
#include "physical_controllers.h"
#include "piece.h"
#include "message_type.h"

#include <cassert>
#include <sstream>

game_controller::game_controller()
  : m_lhs_cursor_pos{0.5, 4.5},
    m_mouse_user_selector{},
    m_rhs_cursor_pos{7.5, 4.5}
{
  if (has_mouse_controller(physical_controllers::get()))
  {
    m_mouse_user_selector = action_number(1);
  }

}

void game_controller::add_user_input(const user_input& a)
{
  m_user_inputs.add(a);
}

void add_user_input(game_controller& c, const user_input& input)
{
  c.add_user_input(input);
}

void add_user_inputs(game_controller& c, const user_inputs& inputs)
{
  for (const auto i: inputs.get_user_inputs())
  {
    add_user_input(c, i);
  }
}

void game_controller::apply_user_inputs_to_game(
  game& g
)
{
  for (const auto& action: m_user_inputs.get_user_inputs())
  {
    if (action.get_user_input_type() == user_input_type::press_action_1)
    {
      process_press_action_1(g, *this, action);
    }
    else if (action.get_user_input_type() == user_input_type::press_action_2)
    {
      process_press_action_2(g, *this, action);
    }
    else if (action.get_user_input_type() == user_input_type::press_action_3)
    {
      process_press_action_3(g, *this, action);
    }
    else if (action.get_user_input_type() == user_input_type::press_action_4)
    {
      process_press_action_4(g, *this, action);
    }
    else if (action.get_user_input_type() == user_input_type::press_down)
    {
      const auto pos{get_cursor_pos(action.get_player())};
      set_cursor_pos(get_below(pos), action.get_player());
    }
    else if (action.get_user_input_type() == user_input_type::press_left)
    {
      const auto pos{get_cursor_pos(action.get_player())};
      set_cursor_pos(get_left(pos), action.get_player());
    }
    else if (action.get_user_input_type() == user_input_type::press_right)
    {
      const auto pos{get_cursor_pos(action.get_player())};
      set_cursor_pos(get_right(pos), action.get_player());
    }
    else if (action.get_user_input_type() == user_input_type::press_up)
    {
      const auto pos{get_cursor_pos(action.get_player())};
      set_cursor_pos(get_above(pos), action.get_player());
    }
    else if (action.get_user_input_type() == user_input_type::mouse_move)
    {
      if (has_mouse_controller(*this))
      {
        assert(action.get_coordinat());
        set_cursor_pos(action.get_coordinat().value(), action.get_player());
        assert(get_cursor_pos(action.get_player()) == action.get_coordinat().value());
        assert(square(get_cursor_pos(action.get_player())) == square(action.get_coordinat().value()));
      }
    }
    #ifdef FIX_ISSUE_46
    // Below you see that LMB selects and RMB moves
    // Instead, the mouse controller has an active action
    // triggered by LMB, and RMB goes to the next
    #endif // FIX_ISSUE_46
    else if (action.get_user_input_type() == user_input_type::lmb_down)
    {
      if (!has_mouse_controller(*this)) return;
      process_press_action_1_or_lmb_down(g, *this, action);

    }
    else if (action.get_user_input_type() == user_input_type::rmb_down)
    {
      const auto maybe_index{
        get_mouse_user_selector()
      };
      assert(maybe_index);
      set_mouse_user_selector(
        get_next(maybe_index.value())
      );
    }
  }
  m_user_inputs = std::vector<user_input>();
}

bool can_do(
  const game& g,
  const game_controller& c,
  const piece_action_type action,
  const side player_side
)
{
  if (action == piece_action_type::select)
  {
    return can_player_select_piece_at_cursor_pos(g, c, player_side);
  }
  return false;
}

bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const game_controller& c,
  const chess_color cursor_color
)
{
  const auto& cursor_pos{
    get_cursor_pos(c, get_player_side(cursor_color))
  };
  if (!is_piece_at(g, cursor_pos, game_options::get().get_click_distance()))
  {
    return false;
  }
  const auto& piece{get_closest_piece_to(g, cursor_pos)};
  const bool is_piece_already_selected{piece.is_selected()};
  const bool does_player_own_that_piece{piece.get_color() == cursor_color};

  return !is_piece_already_selected && does_player_own_that_piece;
}

bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const game_controller& c,
  const side player_side
)
{
  return can_player_select_piece_at_cursor_pos(
    g, c, get_player_color(player_side)
  );
}

std::vector<user_inputs> collect_all_user_inputses(
  const game& g,
  const game_controller& c
)
{
  std::vector<user_inputs> user_inputs;

  const auto piece_actions{collect_all_piece_actions(g)};
  user_inputs.reserve(piece_actions.size());
  for (const auto& piece_action: piece_actions) {
    user_inputs.push_back(to_user_inputs(piece_action, c));
  }
  return user_inputs;
}

user_inputs convert_move_to_user_inputs(
  const game& g,
  const game_controller& c,
  const chess_move& m
)
{
  const auto player_side{get_player_side(m.get_color())};
  const square from{get_from(g, m)};

  user_inputs inputs;
  // Move the cursor to piece's square
  {
    const auto v{get_user_inputs_to_move_cursor_to(c, from, player_side)};
    add(inputs, v);
  }
  // Select the piece
  {
    const auto i{get_user_input_to_select(c, player_side)};
    inputs.add(i);
  }
  // Move the cursor to target's square
  {
    assert(m.get_to().has_value());
    const auto v{
      get_user_inputs_to_move_cursor_from_to(
        c,
        from,
        m.get_to().value(),
        player_side
      )
    };
    add(inputs, v);
  }
  // Do the action
  {
    const auto i{get_user_input_to_do_action_1(c, player_side)};
    inputs.add(i);
  }
  return inputs;
}

int count_user_inputs(const game_controller& c) noexcept
{
  return count_user_inputs(c.get_user_inputs());
}

/*
game_controller create_game_controller_with_keyboard_mouse()
{
  use_keyboard_mouse_controllers();
  return game_controller();
}
*/

game_controller create_game_controller_with_keyboard_mouse()
{
  use_keyboard_mouse_controllers();
  return game_controller();
}

game_controller create_game_controller_with_mouse_keyboard()
{
  use_mouse_keyboard_controllers();
  return game_controller();
}

game_controller create_game_controller_with_two_keyboards()
{
  use_two_keyboard_controllers();
  return game_controller();
}

const game_coordinate& game_controller::get_cursor_pos(const side player) const noexcept
{
  if (player == side::lhs) return m_lhs_cursor_pos;
  assert(player == side::rhs);
  return m_rhs_cursor_pos;
}

const game_coordinate& get_cursor_pos(
  const game_controller&c,
  const side player_side
) noexcept
{
  return c.get_cursor_pos(player_side);
}

const game_coordinate& get_cursor_pos(
  const game_controller& c,
  const chess_color cursor_color
)
{
  return get_cursor_pos(c, get_player_side(cursor_color));
}

square get_cursor_square(
  const game_controller& c,
  const side player_side
)
{
  const game_coordinate cursor_pos{get_cursor_pos(c, player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  return square(cursor_pos);
}

const physical_controller& game_controller::get_physical_controller(const side player_side) const noexcept
{
  return physical_controllers::get().get_controller(player_side);
}

const physical_controller& get_physical_controller(
  const game_controller& c,
  const side player
) noexcept
{
  return c.get_physical_controller(player);
}

physical_controller_type get_physical_controller_type(
  const game_controller& c,
  const side player
) noexcept
{
  return get_physical_controller(c, player).get_type();
}

std::optional<piece_action_type> get_piece_action(
  const game& g,
  const game_controller& c,
  const action_number& n,
  const side player_side
) noexcept
{
  const auto controller_type{c.get_physical_controller(player_side).get_type()};

  // Stub
  assert(controller_type == physical_controller_type::keyboard);

  if (has_selected_pieces(g, player_side))
  {
    assert(has_selected_pieces(g, player_side));

    assert(get_selected_pieces(g, player_side).size() == 1);
    const auto selected_piece{get_selected_pieces(g, player_side)[0]};

    if (can_promote(selected_piece))
    {
      switch (n.get_number())
      {
        case 1: return piece_action_type::promote_to_queen;
        case 2: return piece_action_type::promote_to_rook;
        case 3: return piece_action_type::promote_to_bishop;
        default:
        case 4:
          assert(n.get_number() == 4);
          return piece_action_type::promote_to_knight;
      }
    }

    const game_coordinate cursor_pos{get_cursor_pos(c, player_side)};
    if (!is_coordinat_on_board(cursor_pos))
    {
      return {};
    }
    const auto cursor_square{square(cursor_pos)};
    const chess_color player_color{get_player_color(player_side)};

    if (
      is_piece_at(g, cursor_square)
      && get_piece_at(g, cursor_square).get_color() == player_color
    )
    {
      const piece& p{get_piece_at(g, cursor_square)};
      if (p.is_selected())
      {
        switch (n.get_number())
        {
          case 1: return piece_action_type::unselect;
          case 2: return {};
          case 3: return {};
          default:
          case 4:
            assert(n.get_number() == 4);
            return {};
        }
      }
      else
      {
        assert(!p.is_selected());
        switch (n.get_number())
        {
          case 1: return piece_action_type::select;
          case 2: return {};
          case 3: return {};
          default:
          case 4:
            assert(n.get_number() == 4);
            return {};
        }
      }
    }
    else
    {
      // No own piece at cursor, maybe can move/castle/en-passant there?
      if (can_do(g, selected_piece, piece_action_type::attack_en_passant, cursor_square, player_side))
      {
        return piece_action_type::attack_en_passant;
      }

      // Castling
      const bool can_castle_kingside{can_do(g, selected_piece, piece_action_type::castle_kingside, cursor_square, player_side)};
      if (can_castle_kingside && n == action_number(1)) return piece_action_type::castle_kingside;
      const bool can_castle_queenside{can_do(g, selected_piece, piece_action_type::castle_queenside, cursor_square, player_side)};
      if (can_castle_queenside && n == action_number(1)) return piece_action_type::castle_queenside;
      if (can_castle_kingside && can_castle_queenside && n == action_number(2)) return piece_action_type::castle_queenside;

      if (can_do(g, selected_piece, piece_action_type::move, cursor_square, player_side))
      {
        switch (n.get_number())
        {
          case 1: return piece_action_type::move;
          case 2: return {};
          case 3: return {};
          default:
          case 4:
            assert(n.get_number() == 4);
            return {};
        }
      }
      if (can_do(g, selected_piece, piece_action_type::attack, cursor_square, player_side))
      {
        switch (n.get_number())
        {
          case 1: return piece_action_type::attack;
          case 2: return {};
          case 3: return {};
          default:
          case 4:
            assert(n.get_number() == 4);
            return {};
        }
      }
    }
  }
  else
  {
    // Has no selected pieces
    const auto cursor_pos{get_cursor_pos(c, player_side)};
    if (is_coordinat_on_board(cursor_pos) && is_piece_at(g, square(cursor_pos)))
    {
      const auto p{get_piece_at(g, square(cursor_pos))};
      const auto player_color{get_player_color(player_side)};
      if (p.get_color() == player_color)
      {
        if (!p.is_selected())
        {
          switch (n.get_number())
          {
            case 1: return piece_action_type::select;
            case 2: return {};
            case 3: return {};
            default:
            case 4:
              assert(n.get_number() == 4);
              return {};
          }
        }
      }
    }
  }
  return std::optional<piece_action_type>();
}

std::map<action_number, std::optional<piece_action_type>> get_piece_actions(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept
{
  std::map<action_number, std::optional<piece_action_type>> m;
  for (const auto n: get_all_action_numbers())
  {
    m[n] = get_piece_action(g, c, n, player_side);
  }
  return m;
}


user_input get_user_input_to_do_action_1(
  const game_controller& c,
  const side player_side
)
{
  if (get_physical_controller_type(c, player_side) == physical_controller_type::keyboard)
  {
    return create_press_action_1(player_side);
  }
  else
  {
    return create_press_lmb_action(player_side);
  }
}

const user_inputs& get_user_inputs(const game_controller& c) noexcept
{
 return c.get_user_inputs();
}

user_inputs get_user_inputs_to_move_cursor_from_to(
  const game_controller& c,
  const square& from,
  const square& to,
  const side player_side
)
{
  if (get_physical_controller_type(c, player_side) == physical_controller_type::mouse)
  {
    // A mouse user 'just' moves its mouse at the correct position,
    // regardless of the current cursors' position
    return get_user_inputs_to_move_cursor_to(c, to, player_side);
  }
  else
  {
    assert(
      get_physical_controller_type(c, player_side)
      == physical_controller_type::keyboard
    );
    const int n_right{(to.get_x() - from.get_x() + 8) % 8};
    const int n_down{(to.get_y() - from.get_y() + 8) % 8};
    std::vector<user_input> inputs;
    for (int i{0}; i!=n_right; ++i)
    {
      inputs.push_back(create_press_right_action(player_side));
    }
    for (int i{0}; i!=n_down; ++i)
    {
      inputs.push_back(create_press_down_action(player_side));
    }
    return inputs;
  }
}

user_inputs get_user_inputs_to_move_cursor_to(
  const game_controller& c,
  const square& to,
  const side player_side
)
{
  if (get_physical_controller_type(c, player_side) == physical_controller_type::mouse)
  {
    // A mouse user 'just' moves its mouse at the correct position,
    // regardless of the current cursors' position
    return user_inputs(
      {
        create_mouse_move_action(
          to_coordinat(to),
          player_side
        )
      }
    );
  }
  else
  {
    assert(get_physical_controller_type(c, player_side) == physical_controller_type::keyboard);
    // A keyboard user must move the cursor from its existing position
    const square from{get_cursor_pos(c, player_side)};
    return get_user_inputs_to_move_cursor_from_to(
      c,
      from,
      to,
      player_side
    );
  }
}

user_input get_user_input_to_select(
  const game_controller& c,
  const side player_side
)
{
  if (get_physical_controller_type(c, player_side) == physical_controller_type::keyboard)
  {
    return create_press_action_1(player_side);
  }
  else
  {
    return create_press_lmb_action(player_side);
  }
}

bool has_keyboard_controller(const game_controller& c)
{
  return
       c.get_physical_controller(side::lhs).get_type() == physical_controller_type::keyboard
    || c.get_physical_controller(side::rhs).get_type() == physical_controller_type::keyboard
  ;
}

bool has_mouse_controller(const game_controller& c)
{
  return
       c.get_physical_controller(side::lhs).get_type() == physical_controller_type::mouse
    || c.get_physical_controller(side::rhs).get_type() == physical_controller_type::mouse
  ;
}


bool is_mouse_user(const game_controller& c, const side player_side) noexcept
{
  return c.get_physical_controller(player_side).get_type()
    == physical_controller_type::mouse
  ;
}


void move_cursor_to(
  game_controller& c,
  const std::string& square_str,
  const side player_side
)
{
  move_cursor_to(c, square(square_str), player_side);
}

void move_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
)
{
  c.set_cursor_pos(to_coordinat(s), player_side);
}

void move_keyboard_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
)
{
  assert(
    get_physical_controller_type(c, player_side)
    == physical_controller_type::keyboard
  );
  c.set_cursor_pos(to_coordinat(s), player_side);
  assert(s
    == square(get_cursor_pos(c, player_side))
  );
}

void set_cursor_pos(
  game_controller& c,
  const game_coordinate& pos,
  const side player_side
) noexcept
{
  c.set_cursor_pos(pos, player_side);
}

void set_cursor_pos(
  game_controller& c,
  const square& s,
  const side player_side
) noexcept
{
  set_cursor_pos(c, to_coordinat(s), player_side);
}

void game_controller::set_mouse_user_selector(const action_number& number)
{
  assert(m_mouse_user_selector.has_value());
  m_mouse_user_selector = number;
}

void game_controller::set_cursor_pos(
  const game_coordinate& pos,
  const side player_side) noexcept
{
  if (player_side == side::lhs)
  {
    m_lhs_cursor_pos = pos;
  }
  else
  {
    assert(player_side == side::rhs);
    m_rhs_cursor_pos = pos;
  }
}

void test_game_controller() //!OCLINT tests may be many
{
  #ifndef NDEBUG // no tests in release
  // game::add_user_input
  {
    game_controller c{
      create_game_controller_with_keyboard_mouse()
    };
    assert(is_empty(get_user_inputs(c)));
    add_user_input(c, create_press_action_1(side::lhs));
    assert(!is_empty(get_user_inputs(c)));
  }
  // collect_all_user_inputses
  {
    // #define FIX_ISSUE_34
    #ifdef FIX_ISSUE_34
    // default start
    {
      const game g{create_game_with_standard_starting_position()};
      const game_controller c(create_game_controller_with_two_keyboards());
      const auto user_inputs{collect_all_user_inputses(g, c)};
      assert(!user_inputs.empty());
      const auto piece_actions{collect_all_piece_actions(g)};
      assert(user_inputs.size() == piece_actions.size());
    }
    #endif // FIX_ISSUE_34
  }
  // has_mouse_controller
  {
    const game_controller g(
      create_game_controller_with_two_keyboards()
    );
    assert(!has_mouse_controller(g));
  }
  // 55: move_cursor_to
  {
    game_controller c{
      create_game_controller_with_keyboard_mouse()
    };
    move_cursor_to(c, "d1", side::lhs);
    assert(get_cursor_pos(c, side::lhs) == to_coordinat(square("d1")));
    move_cursor_to(c, "f6", side::rhs);
    assert(get_cursor_pos(c, side::rhs) == to_coordinat(square("f6")));
  }
  // Keyboard: select white king
  {
    game g{create_game_with_standard_starting_position()};
    game_controller c{
      create_game_controller_with_keyboard_mouse()
    };
    const auto white_king{find_pieces(g, piece_type::king, chess_color::white).at(0)};
    set_cursor_pos(c, to_coordinat(white_king.get_current_square()), side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
  }
  // 60: selectedness is transferred, for white
  {
    game g{create_game_with_standard_starting_position()};
    game_controller c{
      create_game_controller_with_keyboard_mouse()
    };
    move_cursor_to(c, "e1", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::white) == 1);
    move_cursor_to(c, "d1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::white) != 2);
    assert(count_selected_units(g, chess_color::white) != 0);
    assert(count_selected_units(g, chess_color::white) == 1);
  }
  // 60: selectedness is transferred, for black
  {
    game g{create_game_with_standard_starting_position()};
    game_controller c{
      create_game_controller_with_two_keyboards()
    };
    move_cursor_to(c, "e8", side::rhs);
    assert(get_cursor_square(c, side::rhs) == square("e8"));
    assert(count_selected_units(g, chess_color::black) == 0);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::black) == 1);
    assert(get_physical_controller_type(c, side::rhs) == physical_controller_type::keyboard);
    move_cursor_to(c, "d8", side::rhs);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::black) != 2);
    assert(count_selected_units(g, chess_color::black) != 0);
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Selecting a unit twice with action 1 selects and unselects it
  {
    game g{create_game_with_standard_starting_position()};
    game_controller c{
      create_game_controller_with_keyboard_mouse()
    };
    assert(count_selected_units(g, chess_color::white) == 0);
    move_cursor_to(c, "e1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::white) != 2);
    assert(count_selected_units(g, chess_color::white) != 0);
    assert(count_selected_units(g, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 0);
  }
  // Cannot select two units
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "d2", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 1);
    do_select(g, c, "e2", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 1);
  }
  // Keyboard: can move pawn forward
  {
    game g{create_game_with_standard_starting_position()};
    game_controller c{
      create_game_controller_with_keyboard_mouse()
    };
    move_cursor_to(c, "e2", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "e4", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    g.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    g.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    g.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(get_closest_piece_to(g, to_coordinat("e3")).get_type() == piece_type::pawn);
    const auto messages{collect_messages(g)};
    const auto message{messages.at(1)};
    assert(message.get_message_type() == message_type::start_move);
  }
  // Keyboard: cannot move pawn backward
  {
    game g = create_game_with_starting_position(starting_position_type::pawn_all_out_assault);
    game_controller c{create_game_controller_with_keyboard_mouse()};
    move_cursor_to(c, "e4", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "e3", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01)); // Ignores invalid action, adds sound effect

    // No effect, so unit stays selected
    //assert(count_selected_units(g, chess_color::white) == 0);

    assert(get_closest_piece_to(g, to_coordinat("e4")).get_type() == piece_type::pawn);

    // Cannot be done, so no message
    // assert(collect_messages(g).at(1).get_message_type() == message_type::cannot);
  }
  // 3: white castles kingside
  {
    game g = create_game_with_starting_position(starting_position_type::ready_to_castle);
    game_controller c{
      create_game_controller_with_keyboard_mouse()
    };
    move_cursor_to(c, "e1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "g1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(get_closest_piece_to(g, to_coordinat("e4")).get_type() == piece_type::pawn);
    assert(collect_messages(g).at(1).get_message_type() == message_type::start_castling_kingside);
  }
  // has_mouse_controller
  {
    const game_controller g(
      create_game_controller_with_mouse_keyboard()
    );
    assert(has_mouse_controller(g));
  }
  // set_mouse_user_selector
  {
    game_controller g(
      create_game_controller_with_keyboard_mouse()
    );
    const action_number n1(1);
    g.set_mouse_user_selector(n1);
    assert(g.get_mouse_user_selector() == n1);

    const action_number n2(3);
    g.set_mouse_user_selector(n2);
    assert(g.get_mouse_user_selector() == n2);
  }
  // Clicking a unit once with LMB selects it
  {
    game g{create_game_with_standard_starting_position()};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(count_selected_units(g, chess_color::black) == 0);
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Clicking a unit twice with LMB selects and unselects it
  {
    game g{create_game_with_standard_starting_position()};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(count_selected_units(g, chess_color::black) == 0);
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 0);
  }
  // 60: selectedness is transferred
  // Clicking a unit with LMB,
  // then another unit with LMB, only the last unit is selected
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(count_selected_units(g, chess_color::black) == 0);
    move_cursor_to(c, "d8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) != 2);
    assert(count_selected_units(g, chess_color::black) != 0);
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Ke8e7 works by LMB, LMB
  {
    game g{get_kings_only_game()};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 0);
    move_cursor_to(c, "e7", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01));
    assert(count_piece_actions(g, chess_color::black) >= 1);
  }
  // 53: nothing selected, cursor at empty square -> no action
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d4", side::lhs);
    move_cursor_to(c, "d5", side::rhs);
    assert(!get_piece_action(g, c, action_number(1), side::lhs));
    assert(!get_piece_action(g, c, action_number(1), side::rhs));
    assert(!get_piece_action(g, c, action_number(2), side::lhs));
    assert(!get_piece_action(g, c, action_number(2), side::rhs));
    assert(!get_piece_action(g, c, action_number(3), side::lhs));
    assert(!get_piece_action(g, c, action_number(3), side::rhs));
    assert(!get_piece_action(g, c, action_number(4), side::lhs));
    assert(!get_piece_action(g, c, action_number(4), side::rhs));
    assert(!can_do(g, c, piece_action_type::select, side::lhs));
  }
  // 53: nothing selected, cursor at square with opponent piece -> no action
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d8", side::lhs);
    move_cursor_to(c, "d1", side::rhs);
    assert(!get_piece_action(g, c, action_number(1), side::lhs));
    assert(!get_piece_action(g, c, action_number(1), side::rhs));
    assert(!get_piece_action(g, c, action_number(2), side::lhs));
    assert(!get_piece_action(g, c, action_number(2), side::rhs));
    assert(!get_piece_action(g, c, action_number(3), side::lhs));
    assert(!get_piece_action(g, c, action_number(3), side::rhs));
    assert(!get_piece_action(g, c, action_number(4), side::lhs));
    assert(!get_piece_action(g, c, action_number(4), side::rhs));
  }
  // 53: nothing selected, cursor at square of own color -> select
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d1", side::lhs);
    move_cursor_to(c, "d8", side::rhs);
    assert(get_piece_action(g, c, action_number(1), side::lhs));
    assert(get_piece_action(g, c, action_number(1), side::rhs));
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() == piece_action_type::select);
    assert(get_piece_action(g, c, action_number(1), side::rhs).value() == piece_action_type::select);
    assert(!get_piece_action(g, c, action_number(2), side::lhs));
    assert(!get_piece_action(g, c, action_number(2), side::rhs));
    assert(!get_piece_action(g, c, action_number(3), side::lhs));
    assert(!get_piece_action(g, c, action_number(3), side::rhs));
    assert(!get_piece_action(g, c, action_number(4), side::lhs));
    assert(!get_piece_action(g, c, action_number(4), side::rhs));
  }
  // 53: selected piece, cursor still there -> unselect
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d1", side::lhs);
    move_cursor_to(c, "d8", side::rhs);
    add_user_input(c, create_press_action_1(side::lhs));
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() == piece_action_type::unselect);
    assert(get_piece_action(g, c, action_number(1), side::rhs).value() == piece_action_type::unselect);

    assert(!get_piece_action(g, c, action_number(2), side::lhs));
    assert(!get_piece_action(g, c, action_number(2), side::rhs));
    assert(!get_piece_action(g, c, action_number(3), side::lhs));
    assert(!get_piece_action(g, c, action_number(3), side::rhs));
    assert(!get_piece_action(g, c, action_number(4), side::lhs));
    assert(!get_piece_action(g, c, action_number(4), side::rhs));

  }
  // 53: Piece selected, cursor at valid other selection target
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "d2", side::lhs);
    do_select(g, c, "d7", side::rhs);
    move_cursor_to(c, "e2", side::lhs);
    move_cursor_to(c, "e7", side::rhs);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() == piece_action_type::select);
    assert(get_piece_action(g, c, action_number(1), side::rhs).value() == piece_action_type::select);
    assert(!get_piece_action(g, c, action_number(2), side::lhs));
    assert(!get_piece_action(g, c, action_number(2), side::rhs));
    assert(!get_piece_action(g, c, action_number(3), side::lhs));
    assert(!get_piece_action(g, c, action_number(3), side::rhs));
    assert(!get_piece_action(g, c, action_number(4), side::lhs));
    assert(!get_piece_action(g, c, action_number(4), side::rhs));
  }
  // 53: Piece selected, cursor at valid target square -> move
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "d2", side::lhs);
    do_select(g, c, "d7", side::rhs);
    move_cursor_to(c, "d3", side::lhs);
    move_cursor_to(c, "d5", side::rhs);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() == piece_action_type::move);
    assert(get_piece_action(g, c, action_number(1), side::rhs).value() == piece_action_type::move);
    assert(!get_piece_action(g, c, action_number(2), side::lhs));
    assert(!get_piece_action(g, c, action_number(2), side::rhs));
    assert(!get_piece_action(g, c, action_number(3), side::lhs));
    assert(!get_piece_action(g, c, action_number(3), side::rhs));
    assert(!get_piece_action(g, c, action_number(4), side::lhs));
    assert(!get_piece_action(g, c, action_number(4), side::rhs));
  }
  // 53: Piece selected, opponent at target square -> attack
  {
    game g{
      create_game_with_starting_position(starting_position_type::queen_end_game)
    };
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "d1", side::lhs);
    do_select(g, c, "d8", side::rhs);
    move_cursor_to(c, "d8", side::lhs);
    move_cursor_to(c, "d1", side::rhs);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() != piece_action_type::move);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() == piece_action_type::attack);
    assert(get_piece_action(g, c, action_number(1), side::rhs).value() == piece_action_type::attack);
  }
  // 53: King selected, cursor at valid king-side castling square -> king-side castle
  {
    game g{
      create_game_with_starting_position(starting_position_type::ready_to_castle)
    };
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "e1", side::lhs);
    do_select(g, c, "e8", side::rhs);
    move_cursor_to(c, "g1", side::lhs);
    move_cursor_to(c, "g8", side::rhs);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() != piece_action_type::move);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() == piece_action_type::castle_kingside);
    assert(get_piece_action(g, c, action_number(1), side::rhs).value() == piece_action_type::castle_kingside);
  }
  // 53: King selected, queen-side castle
  {
    game g{
      create_game_with_starting_position(starting_position_type::ready_to_castle)
    };
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "e1", side::lhs);
    do_select(g, c, "e8", side::rhs);
    move_cursor_to(c, "c1", side::lhs);
    move_cursor_to(c, "c8", side::rhs);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() != piece_action_type::move);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() == piece_action_type::castle_kingside);
    assert(get_piece_action(g, c, action_number(2), side::lhs).value() == piece_action_type::castle_queenside);
    assert(get_piece_action(g, c, action_number(1), side::rhs).value() == piece_action_type::castle_kingside);
    assert(get_piece_action(g, c, action_number(2), side::rhs).value() == piece_action_type::castle_queenside);
  }
  // 53: Pawns move to the square where they are promoted -> move
  {
    game g{
      create_game_with_starting_position(starting_position_type::pawns_near_promotion)
    };
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "a7", side::lhs);
    do_select(g, c, "h2", side::rhs);
    move_cursor_to(c, "a8", side::lhs);
    move_cursor_to(c, "h1", side::rhs);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() == piece_action_type::move);
    assert(get_piece_action(g, c, action_number(1), side::rhs).value() == piece_action_type::move);
  }
  // 53: Pawns are selected at a promotion square (cursor is there too) -> promote
  {
    game g{
      create_game_with_starting_position(starting_position_type::pawns_at_promotion)
    };
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "a8", side::lhs);
    do_select(g, c, "h1", side::rhs);
    move_cursor_to(c, "a8", side::lhs);
    move_cursor_to(c, "h1", side::rhs);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() == piece_action_type::promote_to_queen);
    assert(get_piece_action(g, c, action_number(1), side::rhs).value() == piece_action_type::promote_to_queen);
    assert(get_piece_action(g, c, action_number(2), side::lhs).value() == piece_action_type::promote_to_rook);
    assert(get_piece_action(g, c, action_number(2), side::rhs).value() == piece_action_type::promote_to_rook);
    assert(get_piece_action(g, c, action_number(3), side::lhs).value() == piece_action_type::promote_to_bishop);
    assert(get_piece_action(g, c, action_number(3), side::rhs).value() == piece_action_type::promote_to_bishop);
    assert(get_piece_action(g, c, action_number(4), side::lhs).value() == piece_action_type::promote_to_knight);
    assert(get_piece_action(g, c, action_number(4), side::rhs).value() == piece_action_type::promote_to_knight);
  }
  // 53: Pawns are selected at a promotion square (cursor is there too) -> promote
  {
    game g{
      create_game_with_starting_position(starting_position_type::pawns_at_promotion)
    };
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "a8", side::lhs);
    do_select(g, c, "h1", side::rhs);
    move_cursor_to(c, "a7", side::lhs);
    move_cursor_to(c, "h2", side::rhs);
    assert(get_piece_action(g, c, action_number(1), side::lhs).value() == piece_action_type::promote_to_queen);
    assert(get_piece_action(g, c, action_number(1), side::rhs).value() == piece_action_type::promote_to_queen);
    assert(get_piece_action(g, c, action_number(2), side::lhs).value() == piece_action_type::promote_to_rook);
    assert(get_piece_action(g, c, action_number(2), side::rhs).value() == piece_action_type::promote_to_rook);
    assert(get_piece_action(g, c, action_number(3), side::lhs).value() == piece_action_type::promote_to_bishop);
    assert(get_piece_action(g, c, action_number(3), side::rhs).value() == piece_action_type::promote_to_bishop);
    assert(get_piece_action(g, c, action_number(4), side::lhs).value() == piece_action_type::promote_to_knight);
    assert(get_piece_action(g, c, action_number(4), side::rhs).value() == piece_action_type::promote_to_knight);
  }
  // get_cursor_pos
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(get_cursor_pos(c, chess_color::white) != get_cursor_pos(c, chess_color::black));
  }
  // get_mouse_player_pos
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    const auto pos_before{get_cursor_pos(c, side::rhs)};
    const auto pos{get_cursor_pos(c, side::rhs)};
    set_cursor_pos(c, pos + game_coordinate(0.1, 0.1), side::rhs);
    const auto pos_after{get_cursor_pos(c, side::rhs)};
    assert(pos_before != pos_after);
  }
  // #27: a2-a3 takes 1 time unit
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("a2")));
    assert(!is_piece_at(g, square("a3")));
    assert(count_selected_units(g, chess_color::white) == 0);

    do_select(g, c, "a2", side::lhs);
    move_cursor_to(c, "a3", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 1);

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    assert(is_piece_at(g, square("a2")));
    assert(!is_piece_at(g, square("a3")));
    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      g.tick(delta_t(0.25));
    }
    assert(!is_piece_at(g, square("a2")));
    assert(is_piece_at(g, square("a3")));
  }
  // #27: a2-a4 takes 1 time unit, after which the pawn is en-passantable
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("a2")));
    assert(is_piece_at(g, square("b2")));
    assert(!is_piece_at(g, square("a4")));
    assert(!is_piece_at(g, square("a3")));
    assert(!is_piece_at(g, square("b3")));
    assert(count_selected_units(g, chess_color::white) == 0);

    do_select(g, c, "a2", side::lhs);
    move_cursor_to(c, "a4", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 1);

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    assert(is_piece_at(g, square("a2")));
    assert(!is_piece_at(g, square("a4")));
    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      g.tick(delta_t(0.25));
    }
    assert(!is_piece_at(g, square("a2")));
    assert(is_piece_at(g, square("a4")));

    // Has just double moved
    assert(has_just_double_moved(get_piece_at(g, "a4"), g.get_in_game_time()));
    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      g.tick(delta_t(0.25));
      assert(has_just_double_moved(get_piece_at(g, "a4"), g.get_in_game_time()));
    }
    // Rounding off error?
    g.tick(delta_t(0.00001));
    assert(!has_just_double_moved(get_piece_at(g, "a4"), g.get_in_game_time()));
  }
  // #27: a2-a5 does nothing
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("a2")));
    assert(!is_piece_at(g, square("a5")));
    assert(count_selected_units(g, chess_color::white) == 0);

    do_select(g, c, "a2", side::lhs);
    move_cursor_to(c, "a5", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 1);

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    assert(is_piece_at(g, square("a2")));
    assert(!is_piece_at(g, square("a5")));
    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      g.tick(delta_t(0.25));
    }
    assert(is_piece_at(g, square("a2")));
    assert(!is_piece_at(g, square("a5")));
  }
#ifdef FIX_CASTLING_KINGSIDE
  // castling kingside
  {
    game g{create_game_with_starting_position(starting_position_type::ready_to_castle)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    auto& white_king{get_piece_at(g, "e1")};
    assert(white_king.get_messages().size() == 0);
    auto& white_rook{get_piece_at(g, "h1")};
    assert(white_rook.get_messages().size() == 0);

    do_select(g, c, "e1", side::lhs);
    assert(white_king.is_selected());

    move_cursor_to(c, "a4", side::lhs); // Square is irrelevant

    assert(white_king.get_messages().size() == 1); // Selected
    assert(white_rook.get_messages().size() == 0); // No need to be selected

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    assert(white_king.get_messages().size() == 2); // Castling started
    assert(white_rook.get_messages().size() == 1); // Castling started

    assert(is_piece_at(g, square("e1")));  // K
    assert(!is_piece_at(g, square("f1"))); // Empty
    assert(!is_piece_at(g, square("g1"))); // Empty
    assert(is_piece_at(g, square("h1")));  // Rook
    // Should take 1 time unit
    for (int i{0}; i!=5; ++i)
    {
      g.tick(delta_t(0.25));
    }
    assert(!is_piece_at(g, square("e1"))); // Empty
    assert(is_piece_at(g, square("f1")));  // Rook
    assert(is_piece_at(g, square("g1")));  // K
    assert(!is_piece_at(g, square("h1"))); // Empty

  }
#endif // FIX_CASTLING_KINGSIDE
#ifdef FIX_CASTLING_QUEENSIDE
  // castling queenside
  {
    game g{create_game_with_starting_position(starting_position_type::ready_to_castle)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    do_select(g, c, "e1", side::lhs);
    move_cursor_to(c, "a4", side::lhs); // Square is irrelevant
    assert(count_selected_units(g, chess_color::white) == 1);

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    assert(is_piece_at(g, square("a1")));  // Rook
    assert(!is_piece_at(g, square("b1"))); // Empty
    assert(!is_piece_at(g, square("c1"))); // Empty
    assert(!is_piece_at(g, square("d1"))); // Empty
    assert(is_piece_at(g, square("e1")));  // K
    // Should take 1 time unit
    for (int i{0}; i!=5; ++i)
    {
      g.tick(delta_t(0.25));
    }
    assert(!is_piece_at(g, square("a1"))); // Empty
    assert(!is_piece_at(g, square("b1"))); // Empty
    assert(is_piece_at(g, square("c1")));  // K
    assert(is_piece_at(g, square("d1")));  // Rook
    assert(!is_piece_at(g, square("e1"))); // Empty

  }
#endif // FIX_CASTLING_QUEENSIDE
  // WHen two pieces want to move to the same square, one will go back
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    // Start c2-c3
    do_select(g, c, "c2", side::lhs);
    move_cursor_to(c, "c3", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    // Get that pawn moving
    g.tick(delta_t(0.25));

    // Start Nb1-c3
    do_select(g, c, "b1", side::lhs);
    move_cursor_to(c, "c3", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      g.tick(delta_t(0.25));
    }
    assert(is_piece_at(g, square("c3")));
    assert(is_piece_at(g, square("b1")));
    assert(!is_piece_at(g, square("c2"))); // The pawn got there first

    const auto knight_messages{get_piece_at(g, "b1").get_messages()};
    assert(knight_messages.size() == 5);
    assert(knight_messages[0] == message_type::select);
    assert(knight_messages[1] == message_type::start_move);
    assert(knight_messages[2] == message_type::start_move); // ?Why twice
    assert(knight_messages[3] == message_type::cannot);
    assert(knight_messages[4] == message_type::done);
  }
  // A piece under attack must have decreasing health
  {
    game_controller c{create_game_controller_with_keyboard_mouse()};
    game g{create_game_with_starting_position(starting_position_type::bishop_and_knight_end_game)};

    const double health_before{get_piece_at(g, square("d2")).get_health()};
    // Let the white knight at c4 attack the black king at d2
    assert(get_piece_at(g, square("d2")).get_color() == chess_color::black);

    do_select(g, c, "c4", side::lhs);
    move_cursor_to(c, "d2", side::lhs);

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    assert(get_piece_at(g, square("d2")).get_color() == chess_color::black);
    g.tick(delta_t(0.1));
    assert(get_piece_at(g, square("d2")).get_color() == chess_color::black);
    const double health_after{get_piece_at(g, square("d2")).get_health()};
    assert(health_after < health_before);
  }
  // Cannot attack a piece of one's own color
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    const double health_before{get_piece_at(g, square("e1")).get_health()};

    assert(get_piece_at(g, square("d2")).get_action_history().get_timed_actions().empty());

    do_select(g, c, "d1", side::lhs);
    move_cursor_to(c, "e1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    g.tick(delta_t(0.1));
    const double health_after{get_piece_at(g, square("d2")).get_health()};
    assert(health_after == health_before);
    assert(get_piece_at(g, square("d2")).get_action_history().get_timed_actions().empty());
  }
  // When a piece is killed, the queen attacker moves to that square
  {
    game_controller c{create_game_controller_with_keyboard_mouse()};
    game g{create_game_with_starting_position(starting_position_type::before_scholars_mate)};

    // The attacker
    assert(is_piece_at(g, square("h5")));
    assert(get_piece_at(g, square("h5")).get_type() == piece_type::queen);
    assert(get_piece_at(g, square("h5")).get_color() == chess_color::white);

    // The piece under attack
    assert(is_piece_at(g, square("f7")));
    assert(get_piece_at(g, square("f7")).get_type() == piece_type::pawn);
    assert(get_piece_at(g, square("f7")).get_color() == chess_color::black);


    do_select(g, c, "h5", side::lhs);
    move_cursor_to(c, "f7", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    int cnt{0};
    while (get_piece_at(g, square("f7")).get_color() == chess_color::black)
    {
      assert(is_piece_at(g, square("f7")));
      g.tick(delta_t(0.25));
      assert(cnt < 1000);
    }
    // Must be captured
    assert(get_piece_at(g, square("f7")).get_color() == chess_color::white);
  }
  // #20: A queen cannot attack over pieces
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("d1")));

    do_select(g, c, "d1", side::lhs);
    move_cursor_to(c, "d8", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    assert(is_piece_at(g, square("d1")));
    const auto white_queen_id{get_piece_at(g, square("d1")).get_id()};
    for (int i{0}; i!=5; ++i)
    {
      g.tick(delta_t(0.25));
    }
    const piece& p{get_piece_with_id(g, white_queen_id)};
    assert(is_piece_at(g, square("d1")));
    assert(p.get_messages().back() == message_type::cannot);
    const auto messages{get_piece_at(g, square("d1")).get_messages()};
    assert(messages.back() == message_type::cannot);
  }
  // operator<<
  {
    std::stringstream s;
    game_controller c{create_game_controller_with_keyboard_mouse()};
    s << c;
    assert(!s.str().empty());
  }
  // to_pgn
  {
    game g{create_game_with_standard_starting_position()};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(to_pgn(g).empty());
    const piece_action e2e4(
      chess_color::white,
      piece_type::pawn,
      piece_action_type::move,
      square("e2"),
      square("e4")
    );

    do_select(g, c, "e2", side::lhs);
    move_cursor_to(c, "e4", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    tick_until_idle(g);
    const std::string pgn{to_pgn(g)};
    assert(!pgn.empty());
    assert(pgn == "0.00: white pawn move from e2 to e4");
  }
  #endif // NDEBUG // no tests in release
}

std::ostream& operator<<(std::ostream& os, const game_controller& g) noexcept
{

  os
    << "LHS cursor position: " << g.get_cursor_pos(side::lhs) << '\n'
    << "RHS cursor position: " << g.get_cursor_pos(side::rhs) << '\n'
    << "LHS player physical controller: " << get_physical_controller(g, side::lhs) << '\n'
    << "RHS player physical controller: " << get_physical_controller(g, side::rhs) << '\n'
  ;
  if (g.get_mouse_user_selector())
  {
    os << "Mouse user selector: " << g.get_mouse_user_selector().value() << '\n';
  }
  else
  {
    os << "Mouse user selector: " << "{}" << '\n';
  }
  os
    << "User inputs: " << g.get_user_inputs() << '\n'
  ;
  return os;
}
