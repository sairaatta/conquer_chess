#ifndef LOADING_VIEW_H
#define LOADING_VIEW_H

#ifndef LOGIC_ONLY

#include "program_state.h"
#include "resource_loader.h"
#include "view.h"

#include <SFML/Graphics.hpp>

#include <optional>

/// The Loading screen.
///
/// The Loading screen loads the game resources
/// and shows the progress in doing so.
class loading_view : public view
{
public:
  loading_view();

  /// Displays this window
  void draw() override;

  /// Go to the next state (if any).
  ///
  /// For this screen, the textures are loaded.
  void tick(const delta_t dt) override;

  /// Process an event
  /// @return true if the user wants to quit the program
  bool process_event(sf::Event& event) override;

  void process_resize_event(sf::Event& event) override;

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

  void start() override;

  /// End displaying this class
  void stop() override;

private:

  /// For loading the resources in steps
  resource_loader m_resource_loader;

};

sf::Text get_loading_screen_text() noexcept;

/// Test this class
void test_loading_view();

#endif // LOGIC_ONLY

#endif // LOADING_VIEW_H
