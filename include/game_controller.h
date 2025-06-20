#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "ccfwd.h"
#include "action_number.h"
#include "game_coordinate.h"
#include "side.h"
#include "user_inputs.h"
#include "physical_controller_type.h"

#include <map>
#include <iosfwd>

/// The class that acts as a controller for \link{game}.
///
/// This can be thought of as a virtual person that plays the game for you.
///
/// This class:
///
/// - deals with input from the \link{physical_controllers}
/// - keeps track of the cursors
/// - moves the pieces of a \link{game}
///
class game_controller
{
public:

  /// Add a user input. These will be processed in 'game::tick'
  void add_user_input(const user_input& a);

  /// Process all actions and apply these on the game
  void apply_user_inputs_to_game(game& g);

  /// Get the a player's cursor position
  const game_coordinate& get_cursor_pos(const side player_side) const noexcept;

  /// A mouse user selects one of the four actions by using RMB
  /// and scrollwheel. The mouse user selector stores
  /// which of the four actions is selected.
  /// (a keyboard user has different keys for that)
  const auto& get_mouse_user_selector() const noexcept { return m_mouse_user_selector; }

  /// Get the game users' inputs
  const auto& get_user_inputs() const noexcept { return m_user_inputs; }

  /// Get a player's physical controller
  const physical_controller& get_physical_controller(const side player_side) const noexcept;

  /// Set the selected action for the mouse user.
  /// Assumes there is a mouse user
  void set_mouse_user_selector(const action_number& number);

  /// Set a player's cursor's position
  void set_cursor_pos(const game_coordinate& pos, const side player_side) noexcept;

private:
  // Use a friend instead
  game_controller();

  /// The in-game coordinat of the LHS user's cursor
  game_coordinate m_lhs_cursor_pos;

  /// A mouse user selects one of the four actions
  /// (a keyboard user has different keys for that)
  std::optional<action_number> m_mouse_user_selector;

  /// The in-game coordinat of the RHS user's cursor
  game_coordinate m_rhs_cursor_pos;

  /// The user inputs that need to be processed
  user_inputs m_user_inputs;

  /// Force to pick a setup of physical_controllers
  friend game_controller create_game_controller_with_keyboard_mouse();
  friend game_controller create_game_controller_with_mouse_keyboard();
  friend game_controller create_game_controller_with_two_keyboards();
  friend class game_view;
};

/// Add a user input. These will be processed in 'game::tick'
void add_user_input(game_controller& c, const user_input& input);

/// Add a user_inputs. These will be processed in 'game::tick'
void add_user_inputs(game_controller& c, const user_inputs& input);

/// Can a player do a certain action?
bool can_do(
  const game& g,
  const game_controller& c,
  const piece_action_type action,
  const side player_side
);

/// Can the player select a piece at the current cursor position?
bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const game_controller& c,
  const chess_color cursor_color
);

/// Can the player select a piece at the current cursor position?
bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const game_controller& c,
  const side player_side
);

/// Collect all valid 'user_input' for all players.
/// Each 'user_inputs' equals one 'piece_action'
/// @see use 'collect_all_piece_actions'
/// to get all the 'piece_action's from a game
std::vector<user_inputs> collect_all_user_inputses(
  const game& g,
  const game_controller& c
);

/// Convert a chess move, e.g. e4,
/// to the right user inputs
user_inputs convert_move_to_user_inputs(
  const game& g,
  const game_controller& c,
  const chess_move& move
);

/// Count the total number of \link{user_input}s
/// to be done by the \link{game_controller}.
int count_user_inputs(const game_controller& c) noexcept;

/// Create a game controller when the players use a keyboard and a mouse
game_controller create_game_controller_with_keyboard_mouse();

/// Create a game controller when the players use a mouse and a keyboard
game_controller create_game_controller_with_mouse_keyboard();

/// Create a game controller when the players use two keyboards
game_controller create_game_controller_with_two_keyboards();

/// Get the a player's cursor position
const game_coordinate& get_cursor_pos(
  const game_controller& c,
  const side player_side
) noexcept;

/// Get the cursor position for a chess color
const game_coordinate& get_cursor_pos(
  const game_controller& c,
  const chess_color cursor_color
);

/// Get the a player's cursor position, as a \link{square}
square get_cursor_square(
  const game_controller& c,
  const side player_side
);

/// Get the physical controllers
const physical_controller& get_physical_controller(
  const game_controller& c,
  const side player_side
) noexcept;

/// Get the physical controller type
physical_controller_type get_physical_controller_type(
  const game_controller& c,
  const side player_side
) noexcept;

/// Get the type of action, if any, a player could by pressing an action key.
///
/// For example, pressing action 1 typically un-/selects pieces when
/// a cursor is on a piece of the own color.
///
/// @seealso See \link{get_piece_actions} to obtain these for all of the
/// action keys
std::optional<piece_action_type> get_piece_action(
  const game& g,
  const game_controller& c,
  const action_number& n,
  const side player_side
) noexcept;

/// Get the type of action, if any, a player could by pressing an action key.
///
/// For example, action 1 typically un-/selects pieces, where
/// action 4 is only used to promote to a knight
std::map<action_number, std::optional<piece_action_type>> get_piece_actions(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept;

/// Create the user inputs to do action_1 at the square at the cursor
user_input get_user_input_to_do_action_1(
  const game_controller& c,
  const side player_side
);

/// Get the game users' inputs
const user_inputs& get_user_inputs(const game_controller& c) noexcept;

/// Create the user inputs to move the cursor to a target square
/// knowing it will be at the 'from' square.
/// This is useful for creating future 'user_input's,
/// e.g. for white doing e4, the cursor must be moved to e2
/// to select a pawn, then to e4 to select the target.
user_inputs get_user_inputs_to_move_cursor_from_to(
  const game_controller& c,
  const square& from,
  const square& to,
  const side player_side
);

/// Create the user inputs to move the cursor to a target square
user_inputs get_user_inputs_to_move_cursor_to(
  const game_controller& c,
  const square& to,
  const side player_side
);

/// Create the user inputs to select the square at the cursor
user_input get_user_input_to_select(
  const game_controller& c,
  const side player_side
);

/// Is there a player that uses the keyboard?
bool has_keyboard_controller(const game_controller& c);

/// Is there a player that uses the mouse?
bool has_mouse_controller(const game_controller& c);

/// The the player at that side a mouse user?
bool is_mouse_user(const game_controller& c, const side player_side) noexcept;


/// Put the cursor (i.e. the selector, not the mouse pointer)
/// at the desired square
void move_cursor_to(
  game_controller& c,
  const std::string& square_str,
  const side player_side
);

/// Put the cursor (i.e. the selector, not the mouse pointer)
/// at the desired square.
/// Does not select the square where the cursor is.
void move_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
);

/// Put the cursor (i.e. the selector)
/// at the desired square
/// @see \link{move_cursor_to} is more general
void move_keyboard_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
);

/// Put the cursor (i.e. the selector, not the mouse pointer)
/// at the desired square
/// @see \link{move_cursor_to} is more general
void move_mouse_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
);

/// Set the cursor's position to the target position
void set_cursor_pos(
  game_controller& c,
  const game_coordinate& pos,
  const side player_side
) noexcept;

/// Set the cursor's position to the target square
void set_cursor_pos(
  game_controller& c,
  const square& s,
  const side player_side
) noexcept;

/// Test this class and its free functions
void test_game_controller();

std::ostream& operator<<(std::ostream& os, const game_controller& g) noexcept;

#endif // GAME_CONTROLLER_H
