#include "input_prompt_textures.h"

#ifndef LOGIC_ONLY

#include "helper.h"
#include "sfml_helper.h"


#include <cassert>
#include <regex>

input_prompt_textures::input_prompt_textures()
{
  // The description of what is in the files
  std::vector<std::string> texture_names;
  {
    const std::string filename{"resources/textures/input_prompts/input_prompt_textures.xml"};
    texture_names = read_texture_names(filename);
  }
  assert(!texture_names.empty());

  for (const std::string& texture_name: texture_names)
  {
    const std::string filename{
      std::string("resources/textures/input_prompts/")
      + texture_name + ".png"
    };
    if (!m_textures[texture_name].loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }

  // Test if all the SFML keys are mapped to a resource
  for (const auto k: get_all_sfml_keys())
  {
    const std::string resource_name{to_resource_name(k)};
    assert(has_texture(resource_name));
  }
  // Test if all the inverted SFML keys are mapped to a resource
  for (const auto k: get_all_sfml_keys())
  {
    const std::string resource_name{to_inverted_resource_name(k)};
    assert(has_texture(resource_name));
  }

  // Test if all the SFML keys are mapped to a resource
  for (const auto k: get_all_sfml_buttons())
  {
    const std::string resource_name{to_resource_name(k)};
    assert(has_texture(resource_name));
  }
  assert(has_texture("mouse_move"));
}

bool input_prompt_textures::has_texture(const std::string& s) const noexcept
{
  return m_textures.find(s) != std::end(m_textures);
}

sf::Texture& input_prompt_textures::get_inverted_texture(const sf::Keyboard::Key& s)
{
  return get_texture(to_inverted_resource_name(s));
}

sf::Texture& input_prompt_textures::get_texture(const sf::Keyboard::Key& s)
{
  return get_texture(to_resource_name(s));
}

sf::Texture& input_prompt_textures::get_texture(const sf::Mouse::Button& s)
{
  return get_texture(to_resource_name(s));
}

sf::Texture& input_prompt_textures::get_texture(const std::string& s)
{
  assert(has_texture(s));
  return m_textures[s];
}

std::vector<std::string> read_texture_names(const std::string& filename)
{
  const auto lines{read_lines(filename)};
  //   <SubTexture name="keyboard" x="0" y="0" width="64" height="64" />
  const std::regex re{"name=\"([a-z0-9_]+)\" x="};

  std::vector<std::string> texture_names;
  for (const auto& sentence: lines)
  {
    std::smatch match;
    std::regex_search(sentence, match, re);
    if (match.empty()) continue;
    const std::string t{match[1]};
    assert(!t.empty());
    assert(t[0] != '\'');
    assert(t[0] != '\"');
    assert(t.back() != '\'');
    assert(t.back() != '\"');
    texture_names.push_back(t);
  }
  assert(!texture_names.empty());
  return texture_names;
}


#endif // LOGIC_ONLY
