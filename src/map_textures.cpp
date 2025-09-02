#include "map_textures.h"

#ifndef LOGIC_ONLY

#include <cassert>
#include <sstream>

map_textures::map_textures()
{
  for (const auto r: get_all_races())
  {
    const std::string filename{get_filename(r)};
    if (!m_textures[r].loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }
}

std::string map_textures::get_filename(
  const race r
) const noexcept
{
  std::stringstream s;
  s << "resources/textures/maps/" << r << "_map.jpg";
  return s.str();
}

sf::Texture& map_textures::get_map(
  const race race
)
{
  return m_textures[race];
}

#endif // LOGIC_ONLY
