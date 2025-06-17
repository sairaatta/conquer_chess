#ifndef DRAW_H
#define DRAW_H

///@file Functions to draw things on the main screen
///
/// The main function is \link{draw_texture},
/// the rest are convenience functions

#include "ccfwd.h"
#include <SFML/Graphics/Texture.hpp>

/// Draw an About menu butto non the main window
void draw_about_button(const screen_rect& sr);

/// Draw the texture on the main window
void draw_texture(sf::Texture& t, const screen_rect& r);

/// Draw the text on the main window.
///
/// Use Ariel font, black letters and bold face.
void draw_text(const std::string& s, const screen_rect& r);

#endif // DRAW_H
