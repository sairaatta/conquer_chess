#ifndef ABOUT_H
#define ABOUT_H

/// @file The information about the game

#include <vector>
#include <SFML/System/String.hpp>

/// Get the URL of the homepage.
std::string get_homepage_url() noexcept;

/// Get the name of the author.
std::wstring get_author_name() noexcept;

/// Get the names of the contributors.
///
/// Use `sf::String`, because of contributors with non-ASCII characters
/// in their name.
std::vector<sf::String> get_contributor_names() noexcept;

#endif // ABOUT_H
