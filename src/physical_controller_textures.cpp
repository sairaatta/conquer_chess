#include "physical_controller_textures.h"

#ifndef LOGIC_ONLY


#include <cassert>
#include <filesystem>
#include <sstream>

physical_controller_textures::physical_controller_textures()
{
  // Symbols
  for (const auto r: get_all_physical_controller_types())
  {
    const std::string filename{get_filename(r)};
    assert(std::filesystem::exists(filename));
    if (!m_symbols[r].loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }
  assert(m_symbols.size() == get_all_physical_controller_types().size());

  // Fancy
  for (const auto r: get_all_physical_controller_types())
  {
    const std::string filename{get_fancy_filename(r)};
    assert(std::filesystem::exists(filename));
    if (!m_fancy_textures[r].loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
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
  std::stringstream s;
  s << "resources/textures/physical_controller/fancy_" << t << ".png";
  return s.str();
}

std::string physical_controller_textures::get_filename(
  const physical_controller_type t
) const noexcept
{
  std::stringstream s;
  s << "resources/textures/physical_controller/" << t << ".png";
  return s.str();
}

sf::Texture& physical_controller_textures::get_symbol(
  const physical_controller_type t
) noexcept
{
  return m_symbols[t];
}

#endif // LOGIC_ONLY
