#ifndef LOBBY_OPTIONS_H
#define LOBBY_OPTIONS_H

#include "chess_color.h"
#include "race.h"
#include "side.h"

#include <iosfwd>
#include <map>

/// The options that are set in the lobby.
///
/// This is where the players decide on their colors
/// and races.
class lobby_options
{
public:
  lobby_options(
    const chess_color lhs_color = chess_color::white,
    const race lhs_race = race::classic,
    const race rhs_race = race::classic
  );

  /*
  /// This class is a Singleton
  lobby_options(const lobby_options&) = delete;
  lobby_options& operator=(const lobby_options&) = delete;
  lobby_options(lobby_options&&) = delete;
  lobby_options& operator=(lobby_options&&) = delete;
  static auto& get(){
    static lobby_options r;
    return r;
  }
  */

  /// Get the chess color of a player
  chess_color get_color(const side player_side) const noexcept;

  /// Get the race of a player color
  race get_race(const chess_color player_color) const noexcept;

  /// Get the race of a player side
  race get_race(const side player_side) const noexcept;

  /// Get the side of a player color
  side get_side(const chess_color player_color) const noexcept;

  /// Set the chess color of a player.
  void set_color(const chess_color color, const side player_side) noexcept;

  /// Get the chess color of a player
  void set_race(const race r, const side player_side) noexcept;

private:


  /// The selected colors.
  ///
  /// The LHS is given by the player, the RHS is concluded
  std::map<side, chess_color> m_color;

  /// The selected race, as given by the players
  std::map<side, race> m_race;

  /// To use the constructor
  friend void test_lobby_options();
};

/*
void use_default_lobby_options() noexcept;

chess_color get_color(const side player_side) noexcept;

/// Get the race of a color
race get_race_of_color(const chess_color c) noexcept;

/// Get the chess color of a player
race get_race_of_side(const side player_side) noexcept;

*/

/// Test this class and its free functions
void test_lobby_options();

std::ostream& operator<<(std::ostream& os, const lobby_options& options) noexcept;

#endif // LOBBY_OPTIONS_H
