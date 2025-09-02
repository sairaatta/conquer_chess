#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "ccfwd.h"
#include "piece_action_type.h"
#include "game_coordinate.h"
#include "game.h"
#include "lobby_options.h"
#include "side.h"
#include "user_inputs.h"

#include <map>
#include <iosfwd>

/// The class that acts as a controller for \link{game}.
///
/// This can be thought of as a virtual person that plays the game for you.
///
///
/// This class does:
///
/// - deals with \link{user_input}
/// - keeps track of the cursors
/// - moves the pieces of a \link{game}
///
/// The most important member function is \link{apply_user_inputs_to_game},
/// where the user inputs are converted to actions, which are then applied.
/// @param g a (state of a) \link{game} to play on
/// @param lo a \link{lobby_options} to know who is who, e.g.. who is white
class game_controller
{
public:
  explicit game_controller(
    const game& g = game(),
    const lobby_options& lo = lobby_options()
  );

  /// Add a user input. These will be processed in 'game::tick'
  void add_user_input(const user_input& a);

  /// Process all actions and apply these on the game.
  void apply_user_inputs_to_game();

  /// Get the a player's cursor position
  const game_coordinate& get_cursor_pos(const side player_side) const noexcept;

  /// Get the game
  const game& get_game() const noexcept { return m_game; }

  /// Get the game
  game& get_game() noexcept { return m_game; }

  /// Get the game
  const auto& get_lobby_options() const noexcept { return m_lobby_options; }

  /// Get the selected squares, if any
  const std::optional<piece_id>& get_selected_piece_id(const side s) const noexcept;

  /// Get the game users' inputs
  const auto& get_user_inputs() const noexcept { return m_user_inputs; }

  /// Get a player's physical controller
  const physical_controller& get_physical_controller(const side player_side) const noexcept;

  /// Set a player's cursor's position
  void set_cursor_pos(const game_coordinate& pos, const side player_side) noexcept;

  /// Set the selected square, if any
  void set_selected_piece_id(const side s, const std::optional<piece_id>& selected_piece_id) noexcept;

  /// Make the game go forward
  void tick(const delta_t& dt = delta_t(1.0));

private:

  /// The in-game coordinat of the LHS user's cursor
  std::map<side, game_coordinate> m_cursor_pos;

  /// The game it controls
  game m_game;

  /// The options as set in the lobby: sides, colors, races
  lobby_options m_lobby_options;

  /// The selected squares, if any.
  ///
  /// Use \link{check_selected_pieces_exist} if the pieces that
  /// are selected actually exist
  std::map<side, std::optional<piece_id>> m_selected_piece_id;

  /// The user inputs that need to be processed
  user_inputs m_user_inputs;

  /// Apply an action type to a game
  void apply_action_type_to_game(game& g, const piece_action_type t, const side s);

  /// Apply a specific action type to a game
  void apply_action_type_attack_to_game(game& g, const side s);
  void apply_action_type_attack_en_passant_to_game(game& g, const side s);
  void apply_action_type_castle_kingside_to_game(game& g, const side s);
  void apply_action_type_castle_queenside_to_game(game& g, const side s);
  void apply_action_type_move_to_game(game& g, const side s);
  void apply_action_type_promote_to_game(game& g, const piece_type promote_to_type, const side s);
  void apply_action_type_select_to_game(game& g, const side s);
  void apply_action_type_unselect_to_game(game& g, const side s);

  /// Check that the pieces that are selected (by m_selected_piece_id)
  /// actually exist in m_game
  void check_selected_pieces_exist();

  /// When pieces die in m_game, this class needs to update the
  /// selected pieces
  void update_selectedness_due_to_captures();

  friend class game_view;
};

/// Add a user input. These will be processed in 'game::tick'
void add_user_input(game_controller& c, const user_input& input);

/// Add a user_inputs. These will be processed in 'game::tick'
void add_user_inputs(game_controller& c, const user_inputs& input);


/// Can the player attack?
bool can_attack(
  const game_controller& c,
  const side player_side
) noexcept;

/// Can the player do an en-passant attack?
bool can_attack_en_passant(
  const game_controller& c,
  const side player_side
) noexcept;

/// Can the player castle kingside?
bool can_castle_kingside(
  const game_controller& c,
  const side player_side
) noexcept;

/// Can the player castle queenside?
bool can_castle_queenside(
  const game_controller& c,
  const side player_side
) noexcept;

bool can_move(
  const game_controller& c,
  const side player_side
) noexcept;

/// Can the player select a piece at the current cursor position?
bool can_player_select_piece_at_cursor_pos(
  const game_controller& c,
  const chess_color cursor_color
);

/// Can the player select a piece at the current cursor position?
bool can_player_select_piece_at_cursor_pos(
  const game_controller& c,
  const side player_side
);

bool can_promote(
  const game_controller& c,
  const side player_side
) noexcept;


bool can_select(
  const game_controller& c,
  const side player_side
) noexcept;

bool can_unselect(
  const game_controller& c,
  const side player_side
) noexcept;

/// Get all the sound effects to be processed
std::vector<message> collect_messages(const game_controller& g) noexcept;

///Collect all the piece IDs of the selected pieces, if any
std::vector<piece_id> collect_selected_piece_ids(const game_controller& c);

/// Convert a chess move, e.g. e4,
/// to the right user inputs
user_inputs convert_move_to_user_inputs(
  const game_controller& c,
  const chess_move& move,
  const physical_controller_type t
);

/// Count the total number of actions to be done by pieces of both players
int count_piece_actions(const game_controller& g);

/// Count the total number of actions to be done by pieces of a player
int count_piece_actions(
  const game_controller& g,
  const chess_color player
);

/// Count the number of selected units.
///
/// As one can only select one unit, this is either zero or one.
int count_selected_units(
  const game_controller& c,
  const chess_color player_color
);

/// Count the total number of \link{user_input}s
/// to be done by the \link{game_controller}.
int count_user_inputs(const game_controller& c) noexcept;

chess_color get_color(
  const game_controller& c,
  const side s
);

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

const in_game_time& get_in_game_time(
  const game_controller& c
) noexcept;

/// Get the types of actions, if any, a player could by pressing an action key.
///
/// For example, action 1 typically un-/selects pieces, where
/// action 4 is only used to promote to a knight.
std::vector<piece_action_type> get_piece_actions(
  const game_controller& c,
  const side player_side
) noexcept;

piece& get_piece_at(game_controller& c, const std::string& square_str);
piece& get_piece_at(game_controller& c, const square& s);

/// Get the possible moves for a player's selected pieces
/// Will be empty if no pieces are selected
std::vector<square> get_possible_moves(
  const game_controller& c,
  const side player
);

/// Get all the selected pieces
/// @param g a game
/// @param player the color of the player, which is white for player 1
/// @see use 'has_selected_piece' to see if there is at least 1 piece selected
std::vector<piece> get_selected_pieces(
  const game_controller& c,
  const chess_color player
);

/// Get all the selected pieces
/// @param g a game
/// @param side the side of the player, which is white for player 1
/// @see use 'has_selected_piece' to see if there is at least 1 piece selected
std::vector<piece> get_selected_pieces(
  const game_controller& c,
  const side player
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
  const side player_side,
  const physical_controller_type t
);

/// Create the user inputs to move the cursor to a target square
user_inputs get_user_inputs_to_move_cursor_to(
  const game_controller& c,
  const square& to,
  const side player_side,
  const physical_controller_type t
);

const std::optional<chess_color>& get_winner(const game_controller& c) noexcept;

/// See if there is at least 1 piece selected
/// @param g a game
/// @param player the color of the player, which is white for player 1
/// @see use 'get_selected_pieces' to get all the selected pieces
bool has_selected_pieces(
  const game_controller& c,
  const chess_color player_color
);

/// See if there is at least 1 piece selected
bool has_selected_pieces(
  const game_controller& c,
  const side player_side
);

/// Does the game have a winner?
bool has_winner(const game_controller& c) noexcept;

bool is_cursor_on_selected_piece(
  const game_controller& c,
  const side player_side
) noexcept;

/// Determine if the game is a draw
bool is_draw(const game_controller& c);

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const game_controller& g,
  const square& coordinat
);

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const game_controller& g,
  const std::string& square_str
);

/// Determine if the piece is selected,
/// based on its action history
bool is_selected(
  const piece& p,
  const game_controller& c
);

/// Determine if the square is attacked by (another) piece of a certain color.
///
/// If the square is empty, it will determine if an enemy piece
/// is looking at this square.
///
/// If the square is occupied, it will determine if an enemy piece
/// is looking at that piece.
/// That piece can be of any color.
bool is_square_attacked(
  const game_controller& c,
  const square& s,
  const chess_color attacker_color
);

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

/// Play a random game, used for profiling
game play_random_game(const int n_turns = 2, const int seed = 42);

/// Set the cursor's position to the target position
void set_cursor_pos(
  game_controller& c,
  const game_coordinate& pos,
  const side player_side
) noexcept;

/// Test this class and its free functions
void test_game_controller();

std::string to_board_str(
  const game_controller& c,
  const board_to_text_options& options = board_to_text_options()
) noexcept;

std::ostream& operator<<(std::ostream& os, const game_controller& g) noexcept;

#endif // GAME_CONTROLLER_H
