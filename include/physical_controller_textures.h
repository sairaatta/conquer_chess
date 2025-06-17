#ifndef PHYSICAL_CONTROLLER_TEXTURES_H
#define PHYSICAL_CONTROLLER_TEXTURES_H

#ifndef LOGIC_ONLY

#include "physical_controller_type.h"

#include <SFML/Graphics/Texture.hpp>

#include <map>

/// The textures used in the Lobby dialog.
class physical_controller_textures
{
public:
  physical_controller_textures();

  /// Get a fancy image for a controller type
  sf::Texture& get_fancy(const physical_controller_type t) noexcept;

  /// Get a symbol for a controller type
  sf::Texture& get_symbol(const physical_controller_type t) noexcept;


  std::string get_filename(const physical_controller_type t)  const noexcept;

  int get_n_textures() const noexcept
  {
    return sizeof(*this) / sizeof(m_symbols.at(physical_controller_type::mouse));
  }

private:


  std::map<physical_controller_type, sf::Texture> m_fancy_textures;
  std::map<physical_controller_type, sf::Texture> m_symbols;
};

#endif // LOGIC_ONLY

#endif // PHYSICAL_CONTROLLER_TEXTURES_H
