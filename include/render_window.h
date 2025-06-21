#ifndef RENDER_WINDOW_H
#define RENDER_WINDOW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics/RenderWindow.hpp>

sf::RenderWindow& get_render_window() noexcept;

#endif // LOGIC_ONLY

#endif // RENDER_WINDOW_H
