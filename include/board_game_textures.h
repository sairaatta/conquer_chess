#ifndef BOARD_GAME_TEXTURES_H
#define BOARD_GAME_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>

/// The textures for the game maps.
class board_game_textures
{
public:
  board_game_textures();

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
  sf::Texture& get_pawn_up() { return m_pawn_up; }
  sf::Texture& get_shield() { return m_shield; }
  sf::Texture& get_skull() { return m_skull; }
  sf::Texture& get_sword() { return m_sword; }

private:

  sf::Texture m_dice_shield;
  sf::Texture m_dice_skull;
  sf::Texture m_dice_sword;
  sf::Texture m_dollar;
  sf::Texture m_pawn_up;
  sf::Texture m_shield;
  sf::Texture m_skull;
  sf::Texture m_sword;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
