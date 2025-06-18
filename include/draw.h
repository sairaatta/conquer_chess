#ifndef DRAW_H
#define DRAW_H

#ifndef LOGIC_ONLY

///@file Functions to draw things on the main screen
///
/// The main function is \link{draw_texture},
/// the rest are convenience functions

#include "ccfwd.h"
#include "chess_color.h"
#include "physical_controller_type.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

/// Draw an About menu button on the main window
void draw_about_button(const screen_rect& sr);

/// Draw the text on the main window.
///
/// Use Ariel font, 48 pixels, black letters and bold face
void draw_big_text(const sf::String& s, const screen_rect& r);

/// Draw a chessboard strip texture
void draw_chessboard_strip_texture(const chess_color c, const screen_rect& sr);

/// A fancy label that shows the text 'Controls'
void draw_controls_label(const screen_rect& sr);

/// Draw the fancy physical controller texture with a text on it
void draw_fancy_physical_controller_label(const physical_controller_type& t, const screen_rect& sr);

/// Draw the fancy physical controller texture
void draw_fancy_physical_controller_type(const physical_controller_type& t, const screen_rect& sr);

/// Draw fancy text at any custom size
void draw_fancy_text(
  const sf::String& s,
  const screen_rect& sr,
  const int outer_character_size,
  const int inner_character_size
);

/// The button that shows the text 'Game Speed'
void draw_game_speed_icon(const screen_rect& sr);

/// The button that shows the value for the game speed
void draw_game_speed_value(const screen_rect& sr);

/// Draw fancy text at a huge size
void draw_huge_fancy_text(const sf::String& s, const screen_rect& sr);

/// Draw a symbol for a keyboard input prompt.
///
/// For example, a symbol for the key A
void draw_input_prompt_symbol(
  const sf::Keyboard::Key& k,
  const screen_rect& r
);

/// Draw a symbol for a mouse input prompt.
///
/// For example, a symbol for the key A
void draw_input_prompt_symbol(
  const sf::Mouse::Button& k,
  const screen_rect& r
);

/// The button that shows the text 'Music volume'
void draw_music_volume_label(const screen_rect& sr);

/// The button that shows the value of the music
void draw_music_volume_value(const screen_rect& sr);

/// Draw fancy text at a normal size
void draw_normal_fancy_text(const sf::String& s, const screen_rect& sr);

/// Draw the text on the main window.
///
/// Use Ariel font, 32 pixels, black letters and bold face
void draw_normal_text(const sf::String& s, const screen_rect& r);

/// Draw the texts on the main window,
/// using one row per element.
///
/// Use Ariel font, 32 pixels, black letters and bold face
void draw_normal_texts(
  const std::vector<sf::String>& s, const screen_rect& r
);

/// Draw an Options menu button on the main window
void draw_options_button(const screen_rect& sr);

/// Draw a thick, red outline at the edges of this rectangle
void draw_outline(const screen_rect& sr, const sf::Color& outline_color = sf::Color::Red);

/// Draw the fancy physical controller symbol
void draw_physical_controller_symbol(const physical_controller_type& t, const screen_rect& sr);

/// Draw the pieces
void draw_pieces(
  const std::vector<piece>& pieces,
  const screen_rect& rect,
  const bool show_selected
);

/// Draw a Quit menu button on the main window
void draw_quit_button(const screen_rect& sr);

/// The button that shows the text 'Sound effects volume'
void draw_sound_effects_volume_label(const screen_rect& sr);

/// The button that shows the value of the music
void draw_sound_effects_volume_value(const screen_rect& sr);

/// Draw the squares of a chessboard at the window target rectangle's location
void draw_squares(
  const screen_rect& rect,
  const bool semi_transparent
);

/// Draw a Start menu button on the main window
void draw_start_button(const screen_rect& sr);

/// A fancy label that shows the text 'Starting position'
void draw_starting_position_label(const screen_rect& sr);

/// A fancy label that shows the value of the starting position
void draw_starting_position_value(const screen_rect& sr);


/// Draw the text on the main window.
///
/// Use Ariel font, black letters and bold face
void draw_text(
  const sf::String& s,
  const screen_rect& r,
  const int character_size,
  const sf::Color fill_color = sf::Color::Black
);

/// Draw the texts on the main window,
/// using one row per element.
void draw_texts(
  const std::vector<sf::String>& s, const screen_rect& r, const int character_size
);

/// Draw the texture on the main window scaled to that size
void draw_texture(sf::Texture& t, const screen_rect& r);

constexpr int get_normal_character_size() noexcept { return 32; }

#endif // LOGIC_ONLY

#endif // DRAW_H
