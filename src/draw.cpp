#include "draw.h"

#include "game_resources.h"
#include "render_window.h"
#include "sfml_helper.h"

#include <SFML/Graphics/RectangleShape.hpp>

void draw_texture(sf::Texture& t, const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setTexture(&t);
  get_render_window().draw(rectangle);
}

void draw_about_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::white), sr);
  draw_text("About", sr);
}

void draw_options_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::black), sr);
  draw_text("Options", sr);
}

void draw_outline(const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(5);
  get_render_window().draw(rectangle);
}

void draw_quit_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::black), sr);
  draw_text("Quit", sr);
}

void draw_start_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::white), sr);
  draw_text("Start", sr);
}

void draw_text(const std::string& s, const screen_rect& sr)
{
  /*
  const int vertical_font_size = 0.7
    * static_cast<double>(get_height(sr));
  const int horizonal_font_size = 0.7
    * static_cast<double>(get_width(sr))
    / static_cast<double>(s.size())
  ;
  const int font_size{std::min(horizonal_font_size, vertical_font_size)};
  */
  const int font_size{48};

  sf::Text text;
  text.setFont(get_arial_font());
  text.setStyle(sf::Text::Bold);
  text.setString(s);
  text.setCharacterSize(font_size);
  text.setFillColor(sf::Color::Black);
  set_text_position(text, sr);
  get_render_window().draw(text);
}

