#include "themba_textures.h"

#ifndef LOGIC_ONLY

#include <cassert>

themba_textures::themba_textures()
{
  for (int i=1; i!=6; ++i)
  {
    const std::string filename_str{get_filename(i)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/themba/" + filename);
    f.copy(filename);
    if (!m_textures[i].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}

std::string themba_textures::get_filename(
  const int item
) const noexcept
{
  assert(item >= 1);
  assert(item < 6);
  return "themba_" + std::to_string(item) + ".jpg";
}

sf::Texture& themba_textures::get_texture(
  const int item
)
{
  assert(item >= 1);
  assert(item < 6);
  return m_textures[item];
}

#endif // LOGIC_ONLY
