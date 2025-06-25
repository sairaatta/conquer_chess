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

sf::Texture& board_game_textures::get_game_info_statistic(const game_info_statistic s)
{
  switch (s)
  {
    case game_info_statistic::activity: return get_flag_square();
    case game_info_statistic::protectedness: return get_shield();
    case game_info_statistic::time: return get_hourglass();
    default:
    case game_info_statistic::value:
      assert(s == game_info_statistic::value);
      return get_dollar();
  }

}

sf::Color board_game_textures::get_game_info_statistic_color(const game_info_statistic s)
{
  switch (s)
  {
    case game_info_statistic::activity: return sf::Color(0, 255, 0, 128);
    case game_info_statistic::protectedness: return sf::Color(0, 0, 255, 128);
    case game_info_statistic::time: return sf::Color(255, 255, 255, 128);
    default:
    case game_info_statistic::value:
      assert(s == game_info_statistic::value);
      return sf::Color(255, 0, 0, 128);
  }
}



#endif // LOGIC_ONLY
