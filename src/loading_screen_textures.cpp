#include "loading_screen_textures.h"

#ifndef LOGIC_ONLY

//#include <functional>
#include <cassert>
#include <sstream>

loading_screen_textures::loading_screen_textures()
{
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
  /*
  const std::vector<std::pair<std::reference_wrapper<sf::Texture>, std::string>> v = {
    std::make_pair(std::ref(m_all_races_1), "all_races_1.jpg"),
    std::make_pair(std::ref(m_all_races_2), "all_races_2.jpg"),
    std::make_pair(std::ref(m_all_races_3), "all_races_3.jpg"),
    std::make_pair(std::ref(m_all_races_4), "all_races_4.jpg")
  };
  for (const auto& p: v)
  {
    const QString filename{p.second.c_str()};
    QFile f(":/resources/textures/artwork/" + filename);
    f.copy(filename);
    if (!p.first.get().loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  */
}

sf::Texture& loading_screen_textures::get_all_races(
  const int i
)
{
  return m_all_races.at(i);
}

std::string loading_screen_textures::get_filename(
  const int i
) const noexcept
{
  std::stringstream s;
  s << "all_races_" << i << ".jpg";
  return s.str();
}


#endif // LOGIC_ONLY
