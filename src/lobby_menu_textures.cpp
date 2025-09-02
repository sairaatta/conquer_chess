#include "lobby_menu_textures.h"

#ifndef LOGIC_ONLY



#include <cassert>
#include <sstream>

lobby_menu_textures::lobby_menu_textures()
{
  for (const auto r: get_all_races())
  {
    const std::string filename_str{get_head_filename(r)};
    const auto filename{
      std::string("resources/textures/lobby_menu/")
      + filename_str.c_str()
    };
    if (!m_heads[r].loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }

  for (const auto r: get_all_chess_colors())
  {
    const std::string filename_str{get_color_filename(r)};
    const auto filename{
      std::string("resources/textures/lobby_menu/")
      + filename_str.c_str()
    };
    if (!m_color[r].loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }

  for (const auto b: {true, false})
  {
    const std::string filename_str{get_ready_filename(b)};
    const auto filename{std::string("resources/textures/lobby_menu/")
      + filename_str.c_str()
    };
    if (!m_ready[b].loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }
}

sf::Texture& lobby_menu_textures::get_color(
  const chess_color c
) noexcept
{
  return m_color[c];
}

std::string lobby_menu_textures::get_color_filename(
  const chess_color c
) const noexcept
{
  std::stringstream s;
  s << c << "_chess_color.jpg";
  return s.str();
}

std::string lobby_menu_textures::get_head_filename(
  const race r
) const noexcept
{
  std::stringstream s;
  s << r << "_head.jpg";
  return s.str();
}

sf::Texture& lobby_menu_textures::get_ready(
  const bool b
) noexcept
{
  return m_ready[b];
}

std::string lobby_menu_textures::get_ready_filename(
  const bool is_ready
) const noexcept
{
  if (is_ready) return "ready_yes.jpg";
  return "ready_no.jpg";
}

sf::Texture& lobby_menu_textures::get_head(
  const race r
) noexcept
{
  return m_heads[r];
}

#endif // LOGIC_ONLY
