#ifndef INPUT_PROMPT_TEXTURES_H
#define INPUT_PROMPT_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>

#include <map>

/// The textures used in the Lobby dialog.
class input_prompt_textures
{
public:
  input_prompt_textures();

  /// Determine if a texture is present
  bool has_texture(const std::string& s) const noexcept;

  /// Get a texture
  sf::Texture& get_texture(const std::string& s);

  int get_n_textures() const noexcept
  {
    return sizeof(*this) / sizeof(*m_textures.begin());
  }

private:

  std::map<std::string, sf::Texture> m_textures;

};

std::vector<std::string> read_texture_names(const std::string& filename);

#endif // LOGIC_ONLY

#endif // TEXTURES_H
