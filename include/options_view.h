#ifndef OPTIONS_VIEW_H
#define OPTIONS_VIEW_H

#ifndef LOGIC_ONLY

#include "game_options.h"
#include "options_view_layout.h"
#include "options_view_item.h"
#include "game_resources.h"
#include "physical_controllers.h"
#include <SFML/Graphics.hpp>

/// The Options dialog.
class options_view
{
public:
  explicit options_view();

  const auto& get_layout() const noexcept { return m_layout; }

  const auto& get_physical_controllers() const noexcept { return m_physical_controllers; }

  options_view_item get_selected() const noexcept { return m_selected; }

  /// Select an item.
  /// Play a sound when the selected item changes
  void set_selected(const options_view_item i);

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

private:

  options_view_layout m_layout;

  physical_controllers m_physical_controllers;

  /// The selected item
  options_view_item m_selected;

  /// Increase the value in the selected square,
  /// can be done by leftarrow key
  void decrease_selected();

  /// Increase the value in the selected square,
  /// can be done by LMB, space and right arrow key
  void increase_selected();

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

void draw_panel(
  options_view& v,
  const screen_rect& panel_position,
  const std::string panel_text,
  const chess_color color
);

physical_controller_type get_physical_controller_type(const options_view& v, const side player);

/// Get the starting position
starting_position_type get_starting_position(const options_view& v) noexcept;

/// Show the bottom part
void show_bottom(options_view& v);

/// Show the header of the bottom part,
/// i.e. 'Side' and 'Controller'
void show_bottom_header(options_view& v);

/// Show the left player's settings of the bottom part,
/// i.e. 'Left' and the LHS controller
void show_bottom_row(options_view& v, const side player_side);

/// Show the game speed label and value
void show_game_speed(options_view& v);

/// Show the music volume label and value
void show_music_volume(options_view& v);

/// Show all the layout panels
void show_layout_panels(options_view& v);

/// Show all the pieces in the initial position
void show_pieces(options_view& view);

/// Show the sound effects volume label and value
void show_sound_effects_volume(options_view& v);

/// Show the starting position label and value
void show_starting_position(options_view& v);

/// Show the selected panel
void show_selected_panel(options_view& v);

/// Show the top part
void show_top(options_view& v);

#endif // LOGIC_ONLY

#endif // OPTIONS_VIEW_H
