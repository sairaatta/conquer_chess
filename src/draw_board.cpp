#include "draw_board.h"

#ifndef LOGIC_ONLY

#include "board_layout.h"
#include "draw.h"
#include "game.h"
#include "game_resources.h"
#include "sfml_helper.h"

void draw_pieces(
  const std::vector<piece>& pieces,
  const board_layout& layout
)
{
  for (const auto& piece: pieces)
  {
    const auto square_rect{
      layout.get_square(
        piece.get_current_square().get_x(),
        piece.get_current_square().get_y()
      )
    };
    /// +------------------+
    /// |+----------------+|
    /// || Health         ||
    /// |+----------------+|
    /// ||Player          ||
    /// ||                ||
    /// ||                ||
    /// ||                ||
    /// |+----------------+|
    /// +------------------+
    ///
    const auto piece_rect{square_layout(square_rect).get_piece()};
    // Transparency effect when moving
    sf::Color fill_color{sf::Color::Transparent};
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
      fill_color = sf::Color(255, 255, 255, alpha);
    }
    draw_texture(
      get_piece_texture(
        piece.get_race(),
        piece.get_color(),
        piece.get_type()
      ),
      piece_rect,
      fill_color
    );
  }
}

void draw_squares(
  const board_layout& layout,
  const bool semi_transparent
)
{
  for (int x = 0; x != 8; ++x)
  {
    for (int y = 0; y != 8; ++y)
    {
      const chess_color c{(x + y) % 2 == 0 ? chess_color::black : chess_color::white };
      const screen_rect square_rect{layout.get_square(x, y).get_square()};
      auto& t{game_resources::get().get_textures()};
      if (semi_transparent)
      {
        draw_texture(t.get_semitransparent_square(c), square_rect);
      }
      else
      {
        draw_texture(t.get_square(c), square_rect);
      }
    }
  }
}

void draw_unit_health_bars(
  const game& g,
  const board_layout& bl
)
{
  for (const auto& piece: g.get_pieces())
  {
    const auto& square_layout{
      bl.get_square(
        piece.get_current_square().get_x(),
        piece.get_current_square().get_y()
      )
    };
    const auto outline_rect{
      square_layout.get_health_bar_outline()
    };

    // Black background
    draw_rectangle(
      outline_rect,
      sf::Color::Black
    );

    try
    {
      const double f_health{get_f_health(piece)};

      // Will be lower for protoss
      const auto bar_rect{square_layout.get_health_bar_value(f_health, piece.get_race())};
      const sf::Color health_color{f_health_to_color(get_f_health(piece))};
      draw_rectangle(
        bar_rect,
        health_color
      );
    }
    catch (std::logic_error&)
    {
      // OK
    }
    if (piece.get_race() == race::protoss)
    {
      try
      {
        const double f_shield{get_f_shield(piece)};

        // Will be lower for protoss
        const auto bar_rect{square_layout.get_shield_bar_value(f_shield)};
        const sf::Color shield_color{f_shield_to_color(get_f_shield(piece))};
        draw_rectangle(
          bar_rect,
          shield_color
        );
      }
      catch (std::logic_error&)
      {
        // OK
      }

    }

  }
}

#endif // LOGIC_ONLY
