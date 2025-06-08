#ifndef LOADING_SCREEN_TEXTURES_H
#define LOADING_SCREEN_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>

/// The games' loading_screen_textures
/// The raw game resources
class loading_screen_textures
{
public:
  loading_screen_textures();

  int get_n_textures() const noexcept { return sizeof(*this) / sizeof(m_all_races_1); };

  sf::Texture& get_all_races_1() noexcept { return m_all_races_1; }
  sf::Texture& get_all_races_2() noexcept { return m_all_races_2; }
  sf::Texture& get_all_races_3() noexcept { return m_all_races_3; }
  sf::Texture& get_all_races_4() noexcept { return m_all_races_4; }
private:

  sf::Texture m_all_races_1;
  sf::Texture m_all_races_2;
  sf::Texture m_all_races_3;
  sf::Texture m_all_races_4;
};

#endif // LOGIC_ONLY

#endif // LOADING_SCREEN_TEXTURES_H
