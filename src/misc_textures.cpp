#include "misc_textures.h"

#ifndef LOGIC_ONLY

#include <QFile>
#include <cassert>

misc_textures::misc_textures()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Texture>, std::string>> v = {
    std::make_pair(std::ref(m_discord_logo), "discord_logo.png")
  };

  for (const auto& p: v)
  {
    const QString filename{p.second.c_str()};
    QFile f(":/resources/textures/misc/" + filename);
    f.copy(filename);
    if (!p.first.get().loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}

#endif // LOGIC_ONLY
