#ifndef LOADING_VIEW_H
#define LOADING_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "resource_loader.h"

/// The Loading screen.
///
/// The Loading screen loads the game resources
/// and shows the progress in doing so.
class loading_view
{
public:
  loading_view();

  /// Displays this window
  void draw();

  /// Go to the next state (if any).
  ///
  /// For this screen, the textures are loaded.
  void tick();

  bool is_done() const noexcept { return m_resource_loader.is_done(); }

  /// Process an event
  /// @return true if the user wants to quit the program
  bool process_event(sf::Event& event);

  void process_resize_event(sf::Event& event);

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

  /// End displaying this class
  void stop();

private:

  /// For loading the resources in steps
  resource_loader m_resource_loader;
};

sf::Text get_loading_screen_text() noexcept;

#endif // LOGIC_ONLY

#endif // LOADING_VIEW_H
