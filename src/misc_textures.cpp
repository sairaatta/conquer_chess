#include "misc_textures.h"

#ifndef LOGIC_ONLY

#include <cassert>

misc_textures::misc_textures()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Texture>, std::string>> v = {
    std::make_pair(std::ref(m_discord_logo), "discord_logo.png")
  };

  for (const auto& p: v)
  {
    const auto filename{
      std::string("resources/textures/misc/") + p.second.c_str()
    };
    if (!p.first.get().loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }
}

#endif // LOGIC_ONLY
