#include "piece_portrait_textures.h"

#ifndef LOGIC_ONLY


#include <cassert>
#include <sstream>

piece_portrait_textures::piece_portrait_textures()
{
  for (const auto r: get_all_races())
  {
    for (const auto c: get_all_chess_colors())
    {
      for (const auto p: get_all_piece_types())
      {
        const std::string filename_str{get_filename(r, c, p)};
        const auto filename{
          std::string("resources/textures/portraits/")
           + filename_str.c_str()
        };
        if (!m_textures[r][c][p].loadFromFile(filename))
        {
          auto msg{"Cannot find image file '" + filename + "'"};
          throw std::runtime_error(msg);
        }
      }
    }
  }
}

std::string piece_portrait_textures::get_filename(
  const race r,
  const chess_color color,
  const piece_type type
) const noexcept
{
  std::stringstream s;
  s << r << "_" << color << "_" << type << "_portrait.jpg";
  return s.str();
}

sf::Texture& piece_portrait_textures::get_portrait(
  const race race,
  const chess_color color,
  const piece_type type
)
{
  return m_textures[race][color][type];
}

#endif // LOGIC_ONLY
