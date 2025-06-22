#include "artwork_textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <cassert>
#include <sstream>

artwork_textures::artwork_textures()
{
  /*
  for (int i=1 ; i!=5; ++i)
  {
    const std::string filename_str{get_filename(i)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/artwork/" + filename);
    f.copy(filename);
    if (!m_all_races[i].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  */
}


sf::Texture& artwork_textures::get_all_races(
  const int i
)
{
  return game_resources::get().get_loading_screen_textures().get_all_races(i);
}

#endif // LOGIC_ONLY
