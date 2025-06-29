#include "game_controller.h"

#include "chess_move.h"
#include "game.h"
#include "game_options.h"
#include "physical_controllers.h"
#include "piece.h"
#include "pieces.h"
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
  std::map<side, std::vector<piece_action_type>> actions;
  actions[side::lhs] = get_piece_actions(g, *this, side::lhs);
  actions[side::rhs] = get_piece_actions(g, *this, side::rhs);

  std::map<side, std::vector<user_input>> user_inputs;
  user_inputs[side::lhs] = {};
  user_inputs[side::rhs] = {};
  for (const auto& user_input: m_user_inputs.get_user_inputs())
  {
    user_inputs[user_input.get_player()].push_back(user_input);
  }

  for (const side s: get_all_sides())
  {
    for (const auto& user_input: user_inputs.at(s))
    {
      switch(user_input.get_user_input_type())
      {
        case user_input_type::press_down:
        {
          const auto pos{get_cursor_pos(user_input.get_player())};
          set_cursor_pos(get_below(pos), user_input.get_player());
        }
        break;
        case user_input_type::press_left:
        {
          const auto pos{get_cursor_pos(user_input.get_player())};
          set_cursor_pos(get_left(pos), user_input.get_player());
        }
        break;
        case user_input_type::press_right:
        {
          const auto pos{get_cursor_pos(user_input.get_player())};
          set_cursor_pos(get_right(pos), user_input.get_player());
        }
        break;
        case user_input_type::press_up:
        {
          const auto pos{get_cursor_pos(user_input.get_player())};
          set_cursor_pos(get_above(pos), user_input.get_player());
        }
        break;
        case user_input_type::mouse_move:
        {
          if (has_mouse_controller(*this))
          {
            assert(user_input.get_coordinat());
            set_cursor_pos(user_input.get_coordinat().value(), user_input.get_player());
            assert(get_cursor_pos(user_input.get_player()) == user_input.get_coordinat().value());
            assert(square(get_cursor_pos(user_input.get_player())) == square(user_input.get_coordinat().value()));
          }
        }
        break;
        case user_input_type::rmb_down:
        {
          const auto maybe_index{
            get_mouse_user_selector()
          };
          assert(maybe_index);
          set_mouse_user_selector(
            get_next(maybe_index.value())
          );
        }
        break;
        case user_input_type::lmb_down:
        {
          if (actions[s].size() < 1) continue;
          apply_action_type_to_game(g, actions[s][0], s);
        }
        break;
        case user_input_type::press_action_1:
        {
          if (actions[s].size() < 1) continue;
          apply_action_type_to_game(g, actions[s][0], s);
        }
        break;
        case user_input_type::press_action_2:
        {
          if (actions[s].size() < 2) continue;
          apply_action_type_to_game(g, actions[s][1], s);
        }
        break;
        case user_input_type::press_action_3:
        {
          if (actions[s].size() < 3) continue;
          apply_action_type_to_game(g, actions[s][2], s);
        }
        break;
        case user_input_type::press_action_4:
        {
          if (actions[s].size() < 4) continue;
          apply_action_type_to_game(g, actions[s][3], s);
        }
        break;
      }
    }
  }
  m_user_inputs = std::vector<user_input>();
}

void game_controller::apply_action_type_to_game(game& g, const piece_action_type t, const side s)
{
  switch (t)
  {
    case piece_action_type::attack: apply_action_type_attack_to_game(g, s); break;
    case piece_action_type::attack_en_passant: apply_action_type_attack_en_passant_to_game(g, s); break;
    case piece_action_type::castle_kingside: apply_action_type_castle_kingside_to_game(g, s); break;
    case piece_action_type::castle_queenside: apply_action_type_castle_queenside_to_game(g, s); break;
    case piece_action_type::move:
      assert(has_selected_pieces(g, s)); // ???
      apply_action_type_move_to_game(g, s);
      break;
    case piece_action_type::promote_to_bishop: apply_action_type_promote_to_game(g, piece_type::bishop, s); break;
    case piece_action_type::promote_to_knight: apply_action_type_promote_to_game(g, piece_type::knight, s); break;
    case piece_action_type::promote_to_queen: apply_action_type_promote_to_game(g, piece_type::queen, s); break;
    case piece_action_type::promote_to_rook: apply_action_type_promote_to_game(g, piece_type::rook, s); break;
    case piece_action_type::select: apply_action_type_select_to_game(g, s); break;
    case piece_action_type::unselect: apply_action_type_unselect_to_game(g, s); break;

  }
}

void game_controller::apply_action_type_attack_to_game(game& g, const side s)
{
  const chess_color player_color{get_player_color(s)};
  const game_coordinate cursor_pos{get_cursor_pos(s)};
  assert(is_coordinat_on_board(cursor_pos));
  const square cursor{square(cursor_pos)};
  const bool is_cursor_on_piece{is_piece_at(g, cursor)};
  const bool is_cursor_on_enemy_piece{
    is_cursor_on_piece && get_piece_at(g, cursor).get_color() != player_color
  };

  assert(is_cursor_on_enemy_piece);

  assert(has_selected_pieces(g, s));
  assert(has_selected_pieces(g, player_color));
  assert(!get_selected_pieces(g, s).empty());
  assert(get_selected_pieces(g, s).size() == 1);
  const auto selected_piece{get_selected_pieces(g, s)[0]};
  const auto selected_piece_type{selected_piece.get_type()};
  const auto selected_piece_square{selected_piece.get_current_square()};

  assert(
    can_attack_on_empty_board(
      player_color,
      selected_piece_type,
      selected_piece_square,
      cursor
    )
  );

  start_attack(
    g,
    *this,
    cursor_pos,
    player_color
  );
}

void game_controller::apply_action_type_attack_en_passant_to_game(game& g, const side s)
{
  assert(!g.get_pieces().empty());
  assert(!to_str(s).empty());
  assert(!"TODO");
}


void game_controller::apply_action_type_castle_kingside_to_game(game& g, const side player_side)
{
  const chess_color player_color{get_player_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos(player_side)};
  assert(is_coordinat_on_board(cursor_pos));

  const square king_square{get_initial_king_square(player_color)};

  const bool is_castle_kingside{
       is_piece_at(g, king_square)
    && get_piece_at(g, king_square).get_color() == player_color
    && get_piece_at(g, king_square).is_selected()
    && can_castle_kingside(get_piece_at(g, king_square), g)
  };
  assert(is_castle_kingside);

  // Unselect all pieces
  unselect_all_pieces(g, player_color);

  // King starts promoting

  const square king_target_square{get_king_target_square(player_color, piece_action_type::castle_kingside)};
  get_piece_at(g, king_square).add_action(
    piece_action(
      player_color,
      piece_type::king,
      piece_action_type::castle_kingside,
      king_square,
      king_target_square
    )
  );

  // Rook starts promoting
  const square rook_target_square{get_rook_target_square(player_color, piece_action_type::castle_kingside)};
  get_piece_at(g, get_initial_rook_square(player_color, castling_type::king_side)).add_action(
    piece_action(
      player_color,
      piece_type::rook,
      piece_action_type::castle_kingside,
      get_initial_rook_square(player_color, castling_type::king_side),
      rook_target_square
    )
  );
}


void game_controller::apply_action_type_castle_queenside_to_game(game& g, const side player_side)
{
  const chess_color player_color{get_player_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos(player_side)};
  assert(is_coordinat_on_board(cursor_pos));

  const square king_square{get_initial_king_square(player_color)};

  const bool is_castle_queenside{
       is_piece_at(g, king_square)
    && get_piece_at(g, king_square).get_color() == player_color
    && get_piece_at(g, king_square).is_selected()
    && can_castle_queenside(get_piece_at(g, king_square), g)
  };
  assert(is_castle_queenside);

  // Unselect all pieces
  unselect_all_pieces(g, player_color);

  // King start castling
  const square king_target_square{get_king_target_square(player_color, piece_action_type::castle_queenside)};
  get_piece_at(g, get_initial_king_square(player_color)).add_action(
    piece_action(
      player_color,
      piece_type::king,
      piece_action_type::castle_queenside,
      get_initial_king_square(player_color),
      king_target_square
    )
  );
  // Rook starts castling
  const square rook_target_square{get_rook_target_square(player_color, piece_action_type::castle_queenside)};
  get_piece_at(g, get_initial_rook_square(player_color, castling_type::queen_side)).add_action(
    piece_action(
      player_color,
      piece_type::rook,
      piece_action_type::castle_queenside,
      get_initial_rook_square(player_color, castling_type::queen_side),
      rook_target_square
    )
  );
}


void game_controller::apply_action_type_move_to_game(game& g, const side player_side)
{
  assert(has_selected_pieces(g, player_side)); // ???

  const chess_color player_color{get_player_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos(player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  const square cursor{square(cursor_pos)};
  const bool is_cursor_on_piece{is_piece_at(g, cursor)};
  const bool is_cursor_on_friendly_piece{
    is_cursor_on_piece && get_piece_at(g, cursor).get_color() == player_color
  };

  assert(!is_cursor_on_friendly_piece);
  assert(has_selected_pieces(g, player_side));
  assert(has_selected_pieces(g, player_color));
  assert(!get_selected_pieces(g, player_side).empty());
  assert(get_selected_pieces(g, player_side).size() == 1);
  const auto selected_piece{get_selected_pieces(g, player_side)[0]};
  const auto selected_piece_type{selected_piece.get_type()};
  const auto selected_piece_square{selected_piece.get_current_square()};

  assert(
    can_move(
      g.get_pieces(),
      player_color,
      selected_piece_type,
      selected_piece_square,
      cursor
    )
  );

  start_move_unit(
    g,
    *this,
    cursor_pos,
    player_color
  );
}


void game_controller::apply_action_type_promote_to_game(game& g, const piece_type promote_to_type, const side player_side)
{
  const chess_color player_color{get_player_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos(player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  const square cursor{square(cursor_pos)};

  // true for promotions, false for castling
  const bool is_cursor_on_piece{is_piece_at(g, cursor)};
  const bool is_cursor_on_friendly_piece{
    is_cursor_on_piece && get_piece_at(g, cursor).get_color() == player_color
  };
  const bool is_cursor_on_selected_piece_of_own_color{
    is_cursor_on_friendly_piece && get_piece_at(g, cursor).is_selected()
  };

  const bool can_selected_piece_promote{
    is_cursor_on_selected_piece_of_own_color
      && can_promote(get_piece_at(g, cursor))
  };
  const bool is_promotion{
    is_cursor_on_selected_piece_of_own_color && can_selected_piece_promote
  };

  assert(is_cursor_on_friendly_piece);
  const auto& p{get_piece_at(g, cursor)};
  assert(p.is_selected());
  assert (can_promote(p));

  piece_action_type pat{piece_action_type::attack_en_passant};
  switch (promote_to_type)
  {
    case piece_type::bishop: pat = piece_action_type::promote_to_bishop; break;
    case piece_type::knight: pat = piece_action_type::promote_to_knight; break;
    case piece_type::queen: pat = piece_action_type::promote_to_queen; break;
    default:
    case piece_type::rook:
      assert(promote_to_type == piece_type::rook);
      pat = piece_action_type::promote_to_rook;
      break;
  }

  assert(is_promotion);
  get_piece_at(g, cursor).add_action(
    piece_action(
      player_color,
      promote_to_type,
      pat,
      cursor,
      cursor
    )
  );
}

void game_controller::apply_action_type_select_to_game(game& g, const side player_side)
{
  const chess_color player_color{get_player_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos(player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  const square cursor{square(cursor_pos)};

  // true for promotions, false for castling
  const bool is_cursor_on_piece{is_piece_at(g, cursor)};
  const bool is_cursor_on_friendly_piece{
    is_cursor_on_piece && get_piece_at(g, cursor).get_color() == player_color
  };

  assert(is_cursor_on_friendly_piece);
  const auto& p{get_piece_at(g, cursor)};
  assert(!p.is_selected());

  unselect_all_pieces(g, player_color);
  get_piece_at(g, cursor).add_action(
    piece_action(
      player_color,
      p.get_type(),
      piece_action_type::select,
      cursor,
      cursor
    )
  );
}


void game_controller::apply_action_type_unselect_to_game(game& g, const side player_side)
{
  const chess_color player_color{get_player_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos( player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  const square cursor{square(cursor_pos)};

  const bool is_cursor_on_piece{is_piece_at(g, cursor)};
  const bool is_cursor_on_friendly_piece{
    is_cursor_on_piece && get_piece_at(g, cursor).get_color() == player_color
  };

  assert(is_cursor_on_friendly_piece);
  const auto& p{get_piece_at(g, cursor)};
  assert(p.is_selected());
  assert(!can_promote(p));

  get_piece_at(g, cursor).add_action(
    piece_action(
      player_color,
      p.get_type(),
      piece_action_type::unselect,
      cursor,
      cursor
    )
  );
}





bool can_attack(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(g, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};
  const square cursor_square(square(c.get_cursor_pos(player_side)));
  return can_do_attack(g, selected_piece, cursor_square, player_side);
}

bool can_attack_en_passant(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(g, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};
  const square cursor_square(square(c.get_cursor_pos(player_side)));
  return can_do_en_passant(g, selected_piece, cursor_square, player_side);
}

bool can_castle_kingside(
  const game& g,
  const game_controller& /* c */,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(g, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};

  // 'can_castle_kingside' will check if it is a king
  return can_castle_kingside(selected_piece, g);

}

bool can_castle_queenside(
  const game& g,
  const game_controller& /* c */,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(g, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};

  // 'can_castle_kingside' will check if it is a king
  return can_castle_queenside(selected_piece, g);
}

bool can_move(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(g, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};
  const square cursor_square(square(c.get_cursor_pos(player_side)));
  return can_do_move(g, selected_piece, cursor_square, player_side);
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

bool can_promote(
  const game& g,
  const game_controller& /* c */,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(g, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};

  // can_promote will check if it is a pawn
  return can_promote(selected_piece);
}

bool can_select(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept
{
  return can_player_select_piece_at_cursor_pos(g, c, player_side);
}

bool can_unselect(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept
{
  return is_cursor_on_selected_piece(g, c, player_side);
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

std::vector<piece_action_type> get_piece_actions(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept

{
  std::vector<piece_action_type> actions;
  if (can_select(g, c, player_side))
  {
    actions.push_back(piece_action_type::select);
  }
  if (can_unselect(g, c, player_side))
  {
    actions.push_back(piece_action_type::unselect);
  }
  if (can_attack(g, c, player_side))
  {
    actions.push_back(piece_action_type::attack);
  }
  if (can_attack_en_passant(g, c, player_side))
  {
    actions.push_back(piece_action_type::attack_en_passant);
  }
  if (can_move(g, c, player_side))
  {
    actions.push_back(piece_action_type::move);
  }
  if (can_castle_kingside(g, c, player_side))
  {
    actions.push_back(piece_action_type::castle_kingside);
  }
  if (can_castle_queenside(g, c, player_side))
  {
    actions.push_back(piece_action_type::castle_queenside);
  }
  if (can_promote(g, c, player_side))
  {
    actions.push_back(piece_action_type::promote_to_queen);
    actions.push_back(piece_action_type::promote_to_rook);
    actions.push_back(piece_action_type::promote_to_bishop);
    actions.push_back(piece_action_type::promote_to_knight);
  }
  return actions;

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


bool is_cursor_on_selected_piece(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept
{
  if (!has_selected_pieces(g, player_side)) return false;
  const auto& cursor_coordinate{c.get_cursor_pos(player_side)};
  const square cursor_square(cursor_coordinate);
  const auto selected_pieces{get_selected_pieces(g, player_side)};
  assert(selected_pieces.size() == 1);
  const auto piece_square{selected_pieces[0].get_current_square()};
  return cursor_square == piece_square;
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
  // count_user_inputs
  {
    game_controller c{
      create_game_controller_with_keyboard_mouse()
    };
    assert(count_user_inputs(c) == 0);
  }
  //#define FIX_ISSUE_34_A
  #ifdef FIX_ISSUE_34_A
  // collect_all_user_inputses
  {
    const game g{create_game_with_standard_starting_position()};
    use_two_keyboard_controllers();
    game_controller c{
      create_game_controller_with_two_keyboards()
    };
    assert(collect_all_user_inputses(g, c).empty());
    const auto piece_actions{collect_all_piece_actions(g)};
    assert(user_inputs.size() == piece_actions.size());
  }
  #endif // FIX_ISSUE_34
  // has_mouse_controller
  {
    const game_controller g(
      create_game_controller_with_two_keyboards()
    );
    assert(!has_mouse_controller(g));
  }
  // has_keyboard_controller
  {
    const game_controller g(
      create_game_controller_with_two_keyboards()
    );
    assert(has_keyboard_controller(g));

  }
  // has_mouse_controller
  {
    const game_controller c(
      create_game_controller_with_keyboard_mouse()
    );
    assert(!is_mouse_user(c, side::lhs));
    assert(is_mouse_user(c, side::rhs));
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

    assert(has_selected_pieces(g, side::lhs));
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
    game_controller c{create_game_controller_with_keyboard_mouse()};
    do_select(g, c, "e1", side::lhs);
    g.tick(delta_t(0.0));
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "g1", side::lhs); // Cursor must not be on king
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));
    assert(count_selected_units(g, chess_color::white) == 0);

    const auto msg{collect_messages(g)};
    assert(msg.at(1).get_message_type() == message_type::start_castling_kingside);
  }
  // 3: white castles queenside
  {
    game g = create_game_with_starting_position(starting_position_type::ready_to_castle);
    game_controller c{
      create_game_controller_with_keyboard_mouse()
    };
    do_select(g, c, "e1", side::lhs);

    g.tick(delta_t(0.0));
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "g1", side::lhs); // Cursor must not be on king

    add_user_input(c, create_press_action_2(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));

    assert(count_selected_units(g, chess_color::white) == 0);

    const auto msg{collect_messages(g)};
    assert(msg.at(0).get_message_type() == message_type::start_castling_queenside);
    assert(msg.at(2).get_message_type() == message_type::start_castling_queenside);
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
  // nothing selected, cursor at empty square -> no actions, lhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d4", side::lhs);
    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.empty());
  }
  // nothing selected, cursor at empty square -> no actions, rhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d5", side::rhs);
    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.empty());
  }
  // 53: nothing selected, cursor at square with opponent piece -> no actions, lhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d8", side::lhs);
    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.empty());
  }
  // 53: nothing selected, cursor at square with opponent piece -> no actions, rhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d1", side::rhs);
    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.empty());
  }
  // 53: nothing selected, cursor at square of own color -> select, lhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d1", side::lhs);
    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::select);
  }
  // 53: nothing selected, cursor at square of own color -> select, rhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d8", side::rhs);
    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::select);
  }
  // 53: selected piece, cursor still there -> unselect, lhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));

    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::unselect);
  }
  // 53: selected piece, cursor still there -> unselect, rhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    move_cursor_to(c, "d8", side::rhs);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));

    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::unselect);
  }

  // 53: Piece selected, cursor at valid other selection target, lhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "d2", side::lhs);
    move_cursor_to(c, "e2", side::lhs);
    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::select);
  }
  // 53: Piece selected, cursor at valid other selection target, rhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "d7", side::rhs);
    move_cursor_to(c, "e7", side::rhs);
    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::select);
  }
  // 53: Piece selected, cursor at valid target square -> move, lhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "d2", side::lhs);
    move_cursor_to(c, "d3", side::lhs);
    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::move);
  }
  // 53: Piece selected, cursor at valid target square -> move, rhs
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "d7", side::rhs);
    move_cursor_to(c, "d5", side::rhs);
    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::move);
  }
  // 53: Piece selected, opponent at target square -> attack, lhs
  {
    game g{create_game_with_starting_position(starting_position_type::queen_end_game)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "d1", side::lhs);
    move_cursor_to(c, "d8", side::lhs);
    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::attack);
  }
  // 53: Piece selected, opponent at target square -> attack, rhs
  {
    game g{create_game_with_starting_position(starting_position_type::queen_end_game)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "d8", side::rhs);
    move_cursor_to(c, "d1", side::rhs);
    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::attack);
  }
  // King-side castle, cursor on empty square, lhs
  {
    game g{create_game_with_starting_position(starting_position_type::ready_to_castle)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "e1", side::lhs);
    move_cursor_to(c, "e4", side::lhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.size() == 2);
    assert(actions[0] == piece_action_type::castle_kingside);
    assert(actions[1] == piece_action_type::castle_queenside);
  }
  // King-side castle, cursor on empty square, rhs
  {
    game g{create_game_with_starting_position(starting_position_type::ready_to_castle)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "e8", side::rhs);
    move_cursor_to(c, "e5", side::rhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.size() == 2);
    assert(actions[0] == piece_action_type::castle_kingside);
    assert(actions[1] == piece_action_type::castle_queenside);
  }

  // King-side castle, cursor on friendly square, lhs
  {
    game g{create_game_with_starting_position(starting_position_type::ready_to_castle)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "e1", side::lhs);
    move_cursor_to(c, "a1", side::lhs);
    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.size() == 3);
    assert(actions[0] == piece_action_type::select);
    assert(actions[1] == piece_action_type::castle_kingside);
    assert(actions[2] == piece_action_type::castle_queenside);
  }
  // King-side castle, cursor on friendly square, rhs
  {
    game g{create_game_with_starting_position(starting_position_type::ready_to_castle)};
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "e8", side::rhs);
    move_cursor_to(c, "a8", side::rhs);
    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.size() == 3);
    assert(actions[0] == piece_action_type::select);
    assert(actions[1] == piece_action_type::castle_kingside);
    assert(actions[2] == piece_action_type::castle_queenside);
  }
  // 53: Pawns move to the square where they are promoted -> move, lhs
  {
    game g{
      create_game_with_starting_position(starting_position_type::pawns_near_promotion)
    };
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "a7", side::lhs);
    move_cursor_to(c, "a8", side::lhs);
    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::move);
  }
  // 53: Pawns move to the square where they are promoted -> move, rhs
  {
    game g{
      create_game_with_starting_position(starting_position_type::pawns_near_promotion)
    };
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "h2", side::rhs);
    move_cursor_to(c, "h1", side::rhs);
    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::move);
  }
  // Selected pawns at a promotion square, empty square selected -> promote, lhs
  {
    game g{
      create_game_with_starting_position(starting_position_type::pawns_at_promotion)
    };
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "a8", side::lhs);
    move_cursor_to(c, "e4", side::lhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(g, c, side::lhs)};
    assert(actions.size() == 4);
    assert(actions[0] == piece_action_type::promote_to_queen);
    assert(actions[1] == piece_action_type::promote_to_rook);
    assert(actions[2] == piece_action_type::promote_to_bishop);
    assert(actions[3] == piece_action_type::promote_to_knight);
  }
  // Selected pawns at a promotion square, empty square selected -> promote, rhs
  {
    game g{
      create_game_with_starting_position(starting_position_type::pawns_at_promotion)
    };
    game_controller c{create_game_controller_with_two_keyboards()};
    do_select(g, c, "h1", side::rhs);
    move_cursor_to(c, "e5", side::rhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(g, c, side::rhs)};
    assert(actions.size() == 4);
    assert(actions[0] == piece_action_type::promote_to_queen);
    assert(actions[1] == piece_action_type::promote_to_rook);
    assert(actions[2] == piece_action_type::promote_to_bishop);
    assert(actions[3] == piece_action_type::promote_to_knight);
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
  // get_piece_actions
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    const auto m{get_piece_actions(g, c, side::lhs)};
    assert(!m.empty());
  }
  // a2-a3 takes 1 time unit
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
  // a2-a4 takes 1 time unit, after which the pawn is en-passantable
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("a2")));
    assert(!is_piece_at(g, square("a3")));
    assert(!is_piece_at(g, square("a4")));

    do_select(g, c, "a2", side::lhs);
    move_cursor_to(c, "a4", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    for (int i{0}; i!=4; ++i)
    {
      g.tick(delta_t(0.25));
    }
    assert(!is_piece_at(g, square("a2")));
    assert(is_piece_at(g, square("a4")));
  }
  // a2-a4 makes the pawn en-passantable
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("a2")));
    assert(!is_piece_at(g, square("a3")));
    assert(!is_piece_at(g, square("a4")));

    do_select(g, c, "a2", side::lhs);
    move_cursor_to(c, "a4", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    for (int i{0}; i!=4; ++i)
    {
      g.tick(delta_t(0.25));
    }
    assert(!is_piece_at(g, square("a2")));
    assert(is_piece_at(g, square("a4")));
    assert(has_just_double_moved(get_piece_at(g, "a4"), g.get_in_game_time()));
    assert(is_enpassantable(get_piece_at(g, "a4"), g.get_in_game_time()));
  }
  // a2-a5 does nothing
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
  #ifdef FIX_EN_PASSANT_CAPTURE
  // En-passant capture
  {
    game g{create_game_with_starting_position(starting_position_type::before_en_passant)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("g2"))); // White pawn to be captured
    assert(is_piece_at(g, square("h4"))); // Black pawn to capture
    assert(!is_piece_at(g, square("g3"))); // White pawn can move forward
    assert(!is_piece_at(g, square("g4"))); // White pawn can move forward
    assert(count_selected_units(g, chess_color::white) == 0);

    // g2-g4
    do_select(g, c, "g2", side::lhs); // White pawn
    move_cursor_to(c, "g4", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);

    for (int i{0}; i!=5; ++i)
    {
      g.tick(delta_t(0.25));
    }
    assert(!is_piece_at(g, square("g2")));
    assert(is_piece_at(g, square("g4")));
    assert(is_enpassantable(get_piece_at(g, "g4"), g.get_in_game_time()));

    // h4xg3 e.p.
    do_select(g, c, "h4", side::rhs); // Black pawn
    move_cursor_to(c, "g3", side::rhs);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game(g);

    const auto messages{collect_messages(g)};
    // Maybe type must be start_en_passant_attack?
    assert(messages.back().get_message_type() == message_type::start_attack);

    for (int i{0}; i!=4; ++i)
    {
      g.tick(delta_t(0.25));
    }
    g.tick(delta_t(0.25));
    assert(!is_piece_at(g, square("h4"))); // Black pawn has moved from here
    assert(!is_piece_at(g, square("g4"))); // White pawn is captured
    assert(is_piece_at(g, square("g3"))); // Black pawn has moved here
  }
#endif // FIX_EN_PASSANT_CAPTURE
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
    assert(!is_piece_at(g, square("h1"))); // Empty
    assert(is_piece_at(g, square("g1")));  // K
    assert(is_piece_at(g, square("f1")));  // Rook
  }
  // castling queenside
  {
    game g{create_game_with_starting_position(starting_position_type::ready_to_castle)};
    game_controller c{create_game_controller_with_keyboard_mouse()};

    do_select(g, c, "e1", side::lhs);
    move_cursor_to(c, "a4", side::lhs); // Square is irrelevant
    assert(count_selected_units(g, chess_color::white) == 1);

    add_user_input(c, create_press_action_2(side::lhs)); // Action 1 is a king-side castle
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
  // When two pieces want to move to the same square, one will go back
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
  // A queen cannot attack over pieces
  {
    game g{create_game_with_starting_position(starting_position_type::standard)};
    game_controller c{create_game_controller_with_keyboard_mouse()};
    assert(is_piece_at(g, square("d1")));

    do_select(g, c, "d1", side::lhs);
    move_cursor_to(c, "d8", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g); // Will do nothing

    assert(is_piece_at(g, square("d1")));
    const auto white_queen_id{get_piece_at(g, square("d1")).get_id()};
    for (int i{0}; i!=5; ++i)
    {
      g.tick(delta_t(0.25));
    }
    const piece& p{get_piece_with_id(g, white_queen_id)};
    assert(is_piece_at(g, square("d1")));
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
