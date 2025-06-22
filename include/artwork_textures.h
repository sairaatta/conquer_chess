#ifndef ARTWORK_TEXTURES_H
#define ARTWORK_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>

/// The textures for the game maps.
class artwork_textures
{
public:
  artwork_textures();

  int get_n_textures() const noexcept
  {
    return 0;
    // return sizeof(*this)
    //   / sizeof(m_all_races.at(1))
    // ;
  }

  /// Get texture of a artwork with all races
  ///
  /// This uses \link{loading_screen_textures}.
  sf::Texture& get_all_races(
    const int i
  );

private:


};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
