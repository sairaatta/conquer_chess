#ifndef MISC_TEXTURES_H
#define MISC_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>

/// The textures for the game maps.
class misc_textures
{
public:
  misc_textures();


  int get_n_textures() const noexcept
  {
    return sizeof(*this)
      / sizeof(m_discord_logo)
    ;
  }

  /// Get the texture for the Discord logo
  sf::Texture& get_discord_logo() { return m_discord_logo; }

private:

  sf::Texture m_discord_logo;
};

#endif // LOGIC_ONLY

#endif // MISC_TEXTURES_H
