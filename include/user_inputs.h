#ifndef USER_INPUTS_H
#define USER_INPUTS_H

#include "ccfwd.h"

#include "chess_color.h"
#include "user_input.h"
#include "piece_action.h"
#include "physical_controller_type.h"
#include <iostream>
#include <vector>

/// A collection of user inputs.
///
/// The actions in a game, with two types:
///  * The control_actions are what the user wants.
///    These actions are immediate, processed each tick, and
///    possibly converted to piece_actions
///  * The piece_actions are ongoing piece_actions
///    e.g. move from e2 to e3, then from e3 to e4
/// The control_actions processes user actions
/// and passes the into the pieces it concerns, as 'piece_actions'.
/// There, the pieces take over.
///
/// Goal: SFML events converted to user inputs
class user_inputs
{
public:
  user_inputs(
    const std::vector<user_input>& user_inputs = {}
  );

  /// Add a new user input
  void add(const user_input& input);

  const auto& get_user_inputs() const noexcept { return m_user_inputs; }

private:

  std::vector<user_input> m_user_inputs;
};

/// Add new user inputs
void add(user_inputs& current, const user_inputs& to_be_added);

/// Count the total number of control actions to be done by the game,
/// which should be zero after each tick
int count_user_inputs(const user_inputs& a);

/// Count the total number of piece actions to be done by the game
int count_piece_actions(const user_inputs& a);

/// Select a piece
void do_select(
  game_controller& c,
  const std::string& square_str,
  const side player_side
);

/// Select a piece and move it to a target square
void do_select_and_move_piece(
  game_controller& c,
  const std::string& from_square_str,
  const std::string& to_square_str,
  const side player_side,
  const physical_controller_type t
);


/// See if the \link{user_inputs} holds zero elements
bool is_empty(const user_inputs& inputs) noexcept;

/// Process an A or right-mouse-button down
void start_attack(
  game& g,
  game_controller& c,
  const game_coordinate& coordinat,
  const chess_color player_color
);

void start_en_passant_attack(
  game& g,
  game_controller& /* c */,
  const game_coordinate& coordinat,
  const chess_color player_color
);

/// Process an M or right-mouse-button down
void start_move_unit(
  game& g,
  game_controller& c,
  const game_coordinate& coordinat,
  const chess_color player_color
);

/// Test this class and its free functions
void test_user_inputs();

std::ostream& operator<<(std::ostream& os, const user_inputs& actions) noexcept;
bool operator==(const user_inputs& lhs, const user_inputs& rhs) noexcept;

#endif // USER_INPUTS_H
