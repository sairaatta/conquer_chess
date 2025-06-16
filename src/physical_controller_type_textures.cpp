#include "physical_controller_type_textures.h"

#ifndef LOGIC_ONLY

#include <QFile>

#include <cassert>
#include <sstream>

physical_controller_type_textures::physical_controller_type_textures()
{
  for (const auto r: get_all_physical_controller_types())
  {
    const std::string filename_str{get_filename(r)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/physical_controller_types/" + filename);
    f.copy(filename);
    if (!m_types[r].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}

std::string physical_controller_type_textures::get_filename(
  const physical_controller_type t
) const noexcept
{
  std::stringstream s;
  s << t << ".png";
  return s.str();
}

sf::Texture& physical_controller_type_textures::get_type(
  const physical_controller_type t
) noexcept
{
  return m_types[t];
}

#endif // LOGIC_ONLY
