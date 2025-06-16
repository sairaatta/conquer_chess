#ifndef ABOUT_H
#define ABOUT_H

/// @file The information about the game

#include <SFML/System/String.hpp>

#include <vector>

/// Get the name of the author.
std::wstring get_author_name() noexcept;

/// Get the URL of the homepage.
std::string get_homepage_url() noexcept;

/// Get the names of the contributors.
///
/// Use `sf::String`, because of contributors with non-ASCII characters
/// in their name.
std::vector<sf::String> get_contributor_names() noexcept;

/// Test these functions
void test_about();

#endif // ABOUT_H
