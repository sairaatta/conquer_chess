#include "board_game_textures.h"

#ifndef LOGIC_ONLY

#include <QFile>
#include <cassert>

board_game_textures::board_game_textures()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Texture>, std::string>> v = {
    std::make_pair(std::ref(m_dice_shield), "dice_shield.png"),
    std::make_pair(std::ref(m_dice_skull), "dice_skull.png"),
    std::make_pair(std::ref(m_dice_sword), "dice_sword.png"),
    std::make_pair(std::ref(m_dollar), "dollar.png"),
    std::make_pair(std::ref(m_flag_square), "flag_square.png"),
    std::make_pair(std::ref(m_flag_triangle), "flag_triangle.png"),
    std::make_pair(std::ref(m_hourglass), "hourglass.png"),
    std::make_pair(std::ref(m_pawn_up), "pawn_up.png"),
    std::make_pair(std::ref(m_shield), "shield.png"),
    std::make_pair(std::ref(m_skull), "skull.png"),
    std::make_pair(std::ref(m_sword), "sword.png")
  };

  for (const auto& p: v)
  {
    const QString filename{p.second.c_str()};
    QFile f(":/resources/textures/board_game/" + filename);
    f.copy(filename);
    if (!p.first.get().loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }

}

#endif // LOGIC_ONLY
