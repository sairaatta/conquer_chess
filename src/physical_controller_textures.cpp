#include "physical_controller_textures.h"

#ifndef LOGIC_ONLY


#include <cassert>
#include <sstream>

physical_controller_textures::physical_controller_textures()
{
  // Symbols
  for (const auto r: get_all_physical_controller_types())
  {
    const std::string filename_str{get_filename(r)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/physical_controller/" + filename);
    f.copy(filename);
    if (!m_symbols[r].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  assert(m_symbols.size() == get_all_physical_controller_types().size());

  // Fancy
  for (const auto r: get_all_physical_controller_types())
  {
    const std::string filename_str{get_fancy_filename(r)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/physical_controller/" + filename);
    f.copy(filename);
    if (!m_fancy_textures[r].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  assert(m_fancy_textures.size() == get_all_physical_controller_types().size());
}

sf::Texture& physical_controller_textures::get_fancy(
  const physical_controller_type t
) noexcept
{
  return m_fancy_textures[t];
}

std::string physical_controller_textures::get_fancy_filename(
  const physical_controller_type t
) const noexcept
{
  return std::string("fancy_") + get_filename(t);
}

std::string physical_controller_textures::get_filename(
  const physical_controller_type t
) const noexcept
{
  std::stringstream s;
  s << t << ".png";
  return s.str();
}

sf::Texture& physical_controller_textures::get_symbol(
  const physical_controller_type t
) noexcept
{
  return m_symbols[t];
}

#endif // LOGIC_ONLY
