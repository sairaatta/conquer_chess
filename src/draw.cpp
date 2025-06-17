#include "draw.h"

#include "game_resources.h"
#include "render_window.h"
#include "sfml_helper.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>

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
  draw_big_text(sf::String("About"), sr);
}

void draw_big_text(const sf::String& s, const screen_rect& sr)
{
  draw_text(s, sr, 48);
}


void draw_fancy_physical_controller_type(const physical_controller_type& t, const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setTexture(
    &game_resources::get().get_physical_controller_textures().get_fancy(t)
  );
  // Zoom in
  rectangle.setTextureRect(
   sf::IntRect(
     0,
     rectangle.getTexture()->getSize().y / 4,
     rectangle.getTexture()->getSize().x,
     rectangle.getTexture()->getSize().y / 2
   )
  );
  get_render_window().draw(rectangle);
}

void draw_normal_text(const sf::String& s, const screen_rect& sr)
{
  draw_text(s, sr, 32);
}

void draw_normal_fancy_text(const sf::String& s, const screen_rect& sr)
{
  draw_text(s, sr, 32, sf::Color::Black);
  draw_text(s, sr, 30, sf::Color::White);
}

void draw_normal_texts(const std::vector<sf::String>& s, const screen_rect& r)
{
  draw_texts(s, r, 32);
}

void draw_options_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::black), sr);
  draw_big_text(sf::String("Options"), sr);
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
  draw_big_text(sf::String("Quit"), sr);
}

void draw_start_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::white), sr);
  draw_big_text(sf::String("Start"), sr);
}

void draw_text(
  const sf::String& s,
  const screen_rect& sr,
  const int character_size,
  const sf::Color fill_color
)
{
  assert(character_size > 7);
  sf::Text text;
  text.setFont(get_arial_font());
  text.setStyle(sf::Text::Bold);
  text.setString(s);
  text.setCharacterSize(character_size);
  text.setFillColor(fill_color);
  set_text_position(text, sr);
  get_render_window().draw(text);
}

void draw_texts(const std::vector<sf::String>& s, const screen_rect& r, const int character_size)
{
  const int n_lines = s.size();
  const double row_height{
    static_cast<double>(get_height(r))
    / static_cast<double>(s.size())
  };

  for (int i{0}; i!=n_lines; ++i)
  {
    const int x1{r.get_tl().get_x()};
    const int x2{r.get_br().get_x()};
    const int y1 = r.get_tl().get_y() + (row_height * i);
    const int y2 = r.get_tl().get_y() + (row_height * (i + 1));
    const screen_rect row_rect(
      screen_coordinate(x1, y1),
      screen_coordinate(x2, y2)
    );
    draw_text(s[i], row_rect, character_size);
  }

}
