#ifndef LOADING_VIEW_H
#define LOADING_VIEW_H

#ifndef LOGIC_ONLY

#include "program_state.h"
#include "resource_loader.h"

#include <SFML/Graphics.hpp>

#include <optional>

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

  /// The next state to go to, if any.
  ///
  /// Will be empty if the user should remain in this screen
  const auto& get_next_state() const noexcept { return m_next_state; }

  /// Is this window active?
  ///
  /// It can be activated by 'start' and deactivated by 'stop'
  bool is_active() const noexcept { return m_is_active; }

  /// Go to the next state (if any).
  ///
  /// For this screen, the textures are loaded.
  void tick();

  /// Process an event
  /// @return true if the user wants to quit the program
  bool process_event(sf::Event& event);

  void process_resize_event(sf::Event& event);

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

  void start();

  /// End displaying this class
  void stop();

private:

  bool m_is_active{false};

  /// The next state to go to, if any
  std::optional<program_state> m_next_state;

  /// For loading the resources in steps
  resource_loader m_resource_loader;

};

sf::Text get_loading_screen_text() noexcept;

/// Test this class
void test_loading_view();

#endif // LOGIC_ONLY

#endif // LOADING_VIEW_H
