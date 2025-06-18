#include "draw.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"
#include "render_window.h"
#include "game_options.h"
#include "sfml_helper.h"
#include "piece.h"
#include "game_coordinate.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>
#include <sstream>

void draw_about_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::white), sr);
  draw_big_text(sf::String("About"), sr);
}

void draw_big_text(const sf::String& s, const screen_rect& sr)
{
  draw_text(s, sr, 48);
}

void draw_chessboard_strip_texture(const chess_color c, const screen_rect& sr)
{
  draw_texture(
    get_strip_texture(c),
    sr
  );
}

void draw_controls_label(const screen_rect& sr)
{
  sf::RectangleShape rectangle;

  set_rect(rectangle, sr);
  rectangle.setTexture(
    &get_game_option_icon(
      options_view_item::left_controls
    )
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

  draw_normal_fancy_text("Controls", sr);
}

void draw_fancy_physical_controller_label(const physical_controller_type& t, const screen_rect& sr)
{
  draw_fancy_physical_controller_type(t, sr);
  draw_normal_fancy_text(to_str(t), sr);
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

void draw_fancy_text(
  const sf::String& s,
  const screen_rect& sr,
  const int outer_character_size,
  const int inner_character_size
)
{
  assert(inner_character_size < outer_character_size);
  draw_text(s, sr, outer_character_size, sf::Color::Black);
  draw_text(s, sr, inner_character_size, sf::Color::White);
}

void draw_game_speed_icon(const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setTexture(
    &get_game_option_icon(
      options_view_item::game_speed
      )
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

  draw_normal_fancy_text("Game speed", sr);
}

void draw_game_speed_value(const screen_rect& sr)
{
  draw_chessboard_strip_texture(chess_color::black, sr);
  draw_normal_text(to_human_str(game_options::get().get_game_speed()), sr);

}

void draw_huge_fancy_text(const sf::String& s, const screen_rect& sr)
{
  draw_text(s, sr, 512, sf::Color::Black);
  draw_text(s, sr, 500, sf::Color::White);
}

void draw_music_volume_label(const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  rectangle.setTexture(
    &get_game_option_icon(
      options_view_item::music_volume
      )
    );
  set_rect(rectangle, sr);
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

  draw_normal_fancy_text("Music volume", sr);
}

void draw_music_volume_value(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::white), sr);
  std::stringstream s;
  s << get_music_volume() << " %";
  draw_normal_text(s.str(), sr);
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

void draw_outline(const screen_rect& sr, const sf::Color& outline_color)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineColor(outline_color);
  rectangle.setOutlineThickness(5);
  get_render_window().draw(rectangle);
}

void draw_physical_controller_symbol(const physical_controller_type& t, const screen_rect& sr)
{
  draw_texture(
    game_resources::get().get_physical_controller_textures().get_symbol(t),
    sr
  );
}

void draw_pieces(
  const std::vector<piece>& pieces,
  const screen_rect& rect,
  const bool show_selected
)
{
  auto& window{get_render_window()};
  const int square_width{1 + get_width(rect) / 8};
  const int square_height{1 + get_height(rect) / 8};
  for (const auto& piece: pieces)
  {
    sf::RectangleShape sprite;
    sprite.setSize(sf::Vector2f(0.9 * square_width, 0.9 * square_height));
    sprite.setTexture(
      &get_piece_texture(
        piece.get_race(),
        piece.get_color(),
        piece.get_type()
      )
    );
    // Transparency effect when moving
    if (!piece.get_actions().empty()
      && piece.get_actions()[0].get_action_type() == piece_action_type::move
    )
    {
      const double f{piece.get_current_action_progress().get()};
      int alpha{0};
      if (f < 0.5)
      {
        alpha = 255 - static_cast<int>(f * 255.0);
      }
      else
      {
        alpha = static_cast<int>(f * 255.0);
      }
      sprite.setFillColor(sf::Color(255, 255, 255, alpha));
    }
    else
    {
      //sprite.setFillColor(sf::Color(0, 0, 0, 255));
    }
    if (show_selected && piece.is_selected())
    {
      sprite.setOutlineColor(sf::Color(255, 0, 0));
      sprite.setOutlineThickness(2);
    }
    sprite.setOrigin(sf::Vector2f(0.45 * square_width, 0.45 * square_height));
    const game_coordinate game_pos{
      to_coordinat(piece.get_current_square()) + game_coordinate(0.0, 0.1)
    };
    const screen_coordinate screen_position{
      rect.get_tl().get_x() + static_cast<int>(game_pos.get_x() * square_width),
      rect.get_tl().get_y() + static_cast<int>(game_pos.get_y() * square_height),
    };
    sprite.setPosition(
      screen_position.get_x(),
      screen_position.get_y()
    );
    window.draw(sprite);
  }
}

void draw_quit_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::black), sr);
  draw_big_text(sf::String("Quit"), sr);
}

void draw_sound_effects_volume_label(const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  rectangle.setTexture(
    &get_game_option_icon(
      options_view_item::sound_effects_volume
    )
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
  set_rect(rectangle, sr);
  get_render_window().draw(rectangle);

  draw_normal_fancy_text("Sound effects volume", sr);
}

void draw_sound_effects_volume_value(const screen_rect& sr)
{
  draw_chessboard_strip_texture(chess_color::black, sr);

  std::stringstream s;
  s << get_sound_effects_volume() << " %";
  draw_normal_text(s.str(), sr);
}

void draw_squares(
  const screen_rect& rect,
  const bool semi_transparent
)
{
  auto& r{game_resources::get()};
  const int square_width{1 + (get_width(rect) / 8)};
  const int square_height{1 + (get_height(rect) / 8)};
  sf::RectangleShape black_square;
  sf::RectangleShape white_square;
  black_square.setSize(sf::Vector2f(square_width, square_height));
  black_square.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));
  white_square.setSize(sf::Vector2f(square_width, square_height));
  white_square.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));
  if (semi_transparent)
  {
    black_square.setTexture(&r.get_textures().get_semitransparent_square(chess_color::black));
    white_square.setTexture(&r.get_textures().get_semitransparent_square(chess_color::white));
  }
  else
  {
    black_square.setTexture(&r.get_textures().get_square(chess_color::black));
    white_square.setTexture(&r.get_textures().get_square(chess_color::white));
  }

  for (int x = 0; x != 8; ++x)
  {
    for (int y = 0; y != 8; ++y)
    {
      sf::RectangleShape& s = (x + y) % 2 == 0 ? black_square : white_square;
      const screen_coordinate square_pos{
        static_cast<int>(
          rect.get_tl().get_x() + ((0.5 + x) * square_width)
        ),
        static_cast<int>(
          rect.get_tl().get_y() + ((0.5 + y) * square_height)
        )
      };
      s.setPosition(square_pos.get_x(), square_pos.get_y());
      get_render_window().draw(s);
    }
  }
}

void draw_start_button(const screen_rect& sr)
{
  draw_texture(get_strip_texture(chess_color::white), sr);
  draw_big_text(sf::String("Start"), sr);
}

void draw_starting_position_label(const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  rectangle.setTexture(
    &get_game_option_icon(
      options_view_item::starting_position
    )
  );
  set_rect(rectangle, sr);
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

  draw_normal_fancy_text("Starting position", sr);
}

void draw_starting_position_value(const screen_rect& sr)
{
  draw_texture(
    get_strip_texture(chess_color::white),
    sr
  );
  draw_normal_text(to_human_str(get_starting_position()), sr);
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

void draw_texture(sf::Texture& t, const screen_rect& sr)
{
  sf::RectangleShape rectangle;
  set_rect(rectangle, sr);
  rectangle.setTexture(&t);
  get_render_window().draw(rectangle);
}

void draw_texture(sf::Texture& t, const screen_coordinate& c, const screen_coordinate& size)
{
  const int w = size.get_x();
  const int h = size.get_y();
  const screen_rect sr{
    screen_coordinate(c.get_x() - (w / 2), c.get_y() - (h / 2)),
    screen_coordinate(c.get_x() + (w / 2), c.get_y() + (h / 2))
  };
  draw_texture(t, sr);
}

#endif // LOGIC_ONLY

