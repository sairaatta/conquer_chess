#include "about.h"

std::wstring get_author_name() noexcept
{
  return L"Richèl Bilderbeek";
}

std::string get_homepage_url() noexcept
{
  return "https://richelbilderbeek.github.io/conquer_chess/";
}

std::vector<sf::String> get_contributor_names() noexcept
{
  return {
    sf::String(L"Johanna Söderström"),
    sf::String(L"Leonid Rutkowski")
  };
}
