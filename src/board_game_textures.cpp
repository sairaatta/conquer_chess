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

sf::Color board_game_textures::get_outline_color(const game_statistic_type s)
{
  const sf::Color c{get_symbol_color(s)};
  return sf::Color(c.r, c.g, c.b);
}

sf::Texture& board_game_textures::get_symbol(const game_statistic_type s)
{
  switch (s)
  {
    case game_statistic_type::activity: return get_flag_square();
    case game_statistic_type::protectedness: return get_shield();
    case game_statistic_type::time: return get_hourglass();
    default:
    case game_statistic_type::value:
      assert(s == game_statistic_type::value);
      return get_dollar();
  }

}

sf::Color board_game_textures::get_symbol_color(const game_statistic_type s)
{
  switch (s)
  {
    case game_statistic_type::activity: return sf::Color(0, 255, 0, 128);
    case game_statistic_type::protectedness: return sf::Color(0, 0, 255, 128);
    case game_statistic_type::time: return sf::Color(255, 255, 255, 128);
    default:
    case game_statistic_type::value:
      assert(s == game_statistic_type::value);
      return sf::Color(255, 0, 0, 128);
  }
}

sf::Color board_game_textures::get_bar_color(const game_statistic_type s, const chess_color c)
{
  const sf::Color base{get_symbol_color(s)};
  const int hue{c == chess_color::white ? 255 : 0};
  const int r{(base.r + hue) / 2};
  const int g{(base.g + hue) / 2};
  const int b{(base.b + hue) / 2};
  return sf::Color(r, g, b, 128 + 64);
}




#endif // LOGIC_ONLY
