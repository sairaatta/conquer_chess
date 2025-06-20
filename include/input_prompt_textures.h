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

  /// Get an inverted texture from an SFML key
  sf::Texture& get_inverted_texture(const sf::Keyboard::Key& k);

  /// Get a texture from a std::string.
  ///
  /// Prefer to use `get_texture` with a SFML key,
  /// if there is a key available, as these are tested.
  /// However, some textures, e.g. 'mouse_move'
  /// have no key associated with it and can only
  /// be obtained with this method.
  sf::Texture& get_texture(const std::string& s);

  /// Get a texture from an SFML key
  sf::Texture& get_texture(const sf::Keyboard::Key& k);

  /// Get a texture from an SFML mouse button
  sf::Texture& get_texture(const sf::Mouse::Button& b);

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
