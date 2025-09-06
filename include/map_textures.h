#ifndef MAP_TEXTURES_H
#define MAP_TEXTURES_H

#ifndef LOGIC_ONLY

#include "race.h"

#include <SFML/Graphics.hpp>

#include <map>

/// The textures for the game maps.
class map_textures
{
public:
  map_textures();

  std::string get_filename(
    const race race
  ) const noexcept;

  int get_n_textures() const noexcept
  {
    return sizeof(*this)
      / sizeof(m_textures.at(race::classic))
    ;
  }

  /// Get texture of a piece
  sf::Texture& get_map(
    const race race
  );

private:

  std::map<race, sf::Texture> m_textures;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
