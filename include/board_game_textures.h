#ifndef BOARD_GAME_TEXTURES_H
#define BOARD_GAME_TEXTURES_H

#ifndef LOGIC_ONLY

#include "game_info_statistic.h"
#include "chess_color.h"

#include <SFML/Graphics.hpp>

/// The textures for the game maps.
class board_game_textures
{
public:
  board_game_textures();

  /// Get the texture for the symbol
  sf::Texture& get_symbol(const game_info_statistic s);

  /// Get the color for the outline of the bars
  sf::Color get_outline_color(const game_info_statistic s);

  /// Get the color for the symbol
  sf::Color get_symbol_color(const game_info_statistic s);

  /// Get the color for the bar of the statistic
  sf::Color get_bar_color(const game_info_statistic s, const chess_color c);

  int get_n_textures() const noexcept
  {
    return sizeof(*this)
      / sizeof(m_dice_shield)
    ;
  }

  sf::Texture& get_dice_shield() { return m_dice_shield; }
  sf::Texture& get_dice_skull() { return m_dice_skull; }
  sf::Texture& get_dice_sword() { return m_dice_sword; }
  sf::Texture& get_dollar() { return m_dollar; }
  sf::Texture& get_flag_triangle() { return m_flag_triangle; }
  sf::Texture& get_flag_square() { return m_flag_square; }
  sf::Texture& get_hourglass() { return m_hourglass; }
  sf::Texture& get_pawn_up() { return m_pawn_up; }
  sf::Texture& get_shield() { return m_shield; }
  sf::Texture& get_skull() { return m_skull; }
  sf::Texture& get_sword() { return m_sword; }

private:

  sf::Texture m_dice_shield;
  sf::Texture m_dice_skull;
  sf::Texture m_dice_sword;
  sf::Texture m_dollar;
  sf::Texture m_flag_square;
  sf::Texture m_flag_triangle;
  sf::Texture m_hourglass;
  sf::Texture m_pawn_up;
  sf::Texture m_shield;
  sf::Texture m_skull;
  sf::Texture m_sword;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
