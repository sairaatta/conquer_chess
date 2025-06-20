#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

#include "ccfwd.h"
#include "screen_coordinate.h"
#include "starting_position_type.h"
#include "volume.h"
#include "game_speed.h"

#include <iosfwd>

/// Options for the game.
///
/// Options for the game, such as speed.
/// Some options are set by the user in the \link{options_view},
/// some are variables obtained by tuning the game.
class game_options
{
public:
  /// This class is a Singleton
  game_options(const game_options&) = delete;
  game_options& operator=(const game_options&) = delete;
  game_options(game_options &&) = delete;
  game_options& operator=(game_options&&) = delete;
  static auto& get() {
    static game_options r;
    return r;
  }

  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  auto get_click_distance() const noexcept { return m_click_distance; }

  /// Get the damage per chess move that all pieces deal
  auto get_damage_per_chess_move() const noexcept { return 1.0; }

  /// Get the game speed
  auto get_game_speed() const noexcept { return m_game_speed; }

  /// Get the width of the margin in pixels
  auto get_margin_width() const noexcept { return m_margin_width; }

  /// How long log messages are displayed
  double get_message_display_time_secs() const noexcept { return 5.0; }

  /// Get the size of the screen in pixels
  const auto& get_screen_size() const noexcept { return m_screen_size; }

  /// Show the debug info
  auto get_show_debug_info() const noexcept { return m_show_debug_info; }

  /// Show the squares that are actually occupied by the piecs?
  auto get_show_occupied() const noexcept { return false; }

  /// Are selected units highlighted?
  auto get_show_selected() const noexcept { return false; }

  /// Get the starting position
  starting_position_type get_starting_position() const noexcept { return m_starting_position; }

  /// Get the music volume, as a percentage
  const auto& get_music_volume() const noexcept { return m_music_volume; }

  /// Get the sound effects volume
  const volume& get_sound_effects_volume() const noexcept { return m_sound_effects_volume; }

  /// Reset all values to defaults
  void reset();

  /// Set the game speed
  void set_game_speed(const game_speed speed) noexcept { m_game_speed = speed; }

  /// Set the volume, as a percentage
  void set_music_volume(const volume& v) noexcept { m_music_volume = v; }

  /// Set showing the debug info
  void set_show_debug_info(const bool b) noexcept { m_show_debug_info = b; }

  /// Set the starting position
  void set_starting_position(const starting_position_type starting_position) noexcept { m_starting_position = starting_position; }

  /// Set the sound effects volume, as a percentage
  void set_sound_effects_volume(const volume& v) noexcept { m_sound_effects_volume = v; }

private:

  game_options();

  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  double m_click_distance;

  /// The game speed
  game_speed m_game_speed;

  /// The width of the margin in pixels
  int m_margin_width;

  /// The size of the screen in pixels
  screen_coordinate m_screen_size;

  /// The starting position
  starting_position_type m_starting_position;

  /// Music volume
  volume m_music_volume;

  /// Show debug info
  #ifndef NDEBUG
  bool m_show_debug_info{true};
  #else
  bool m_show_debug_info{false};
  #endif

  /// Sound effects volume
  volume m_sound_effects_volume;

};


/// Decrease the game speed
void decrease_game_speed() noexcept;

/// Decrease the music volume
void decrease_music_volume() noexcept;

/// Decrease the sound effects volume
void decrease_sound_effects_volume() noexcept;

/// Decrease the starting position.
///
/// Or: get the previous one.
void decrease_starting_position() noexcept;

/// Are selected squares shown on-screen?
bool do_show_selected() noexcept;

/// The default pixels distance for the mouse
/// for a click to connect to a piece.
constexpr double get_default_click_distance() noexcept { return 0.5; }

/// The default music volume.
///
/// In percent, i.e. 20 denotes 20% of the maximum.
volume get_default_music_volume() noexcept;

/// The default sound effects volume.
///
/// In percent, i.e. 20 denotes 20% of the maximum.
volume get_default_sound_effects_volume() noexcept;

/// Get the music volume
const volume& get_music_volume() noexcept;

/// Get the music volume as a percentage
double get_music_volume_as_percentage() noexcept;

/// Get the sound effects volume
const volume& get_sound_effects_volume() noexcept;

/// Get the starting position
starting_position_type get_starting_position() noexcept;

/// Increase the game speed
void increase_game_speed() noexcept;

/// Increase the music volume
void increase_music_volume() noexcept;

/// Increase the sound effects volume
void increase_sound_effects_volume() noexcept;

/// Increase the starting position.
///
/// Or: get the next one.
void increase_starting_position() noexcept;

/// Test this class and its free functions
void test_game_options();

std::ostream& operator<<(std::ostream& os, const game_options& options) noexcept;

#endif // GAME_OPTIONS_H
