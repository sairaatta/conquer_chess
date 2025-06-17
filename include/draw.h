#ifndef DRAW_H
#define DRAW_H

#ifndef LOGIC_ONLY

///@file Functions to draw things on the main screen
///
/// The main function is \link{draw_texture},
/// the rest are convenience functions

#include "ccfwd.h"
#include "physical_controller_type.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>

/// Draw an About menu button on the main window
void draw_about_button(const screen_rect& sr);

/// Draw the text on the main window.
///
/// Use Ariel font, 48 pixels, black letters and bold face
void draw_big_text(const sf::String& s, const screen_rect& r);

/// A fancy label that shows the text 'Controls'
void draw_controls_label(const screen_rect& sr);

/// Draw the fancy physical controller texture
void draw_fancy_physical_controller_type(const physical_controller_type& t, const screen_rect& sr);

/// The button that shows the text 'Game Speed'
void draw_game_speed_icon(const screen_rect& sr);

/// The button that shows the value for the game speed
void draw_game_speed_value(const screen_rect& sr);

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

/// Draw a thick, red outline at the edges of this rectengle
void draw_outline(const screen_rect& sr);

/// Draw a Quit menu button on the main window
void draw_quit_button(const screen_rect& sr);

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

/// Draw the texture on the main window
void draw_texture(sf::Texture& t, const screen_rect& r);

#endif // LOGIC_ONLY

#endif // DRAW_H
