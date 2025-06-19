#ifndef ABOUT_VIEW_H
#define ABOUT_VIEW_H

#ifndef LOGIC_ONLY

#include "about_view_layout.h"
#include "about_view_item.h"
#include "controls_bar.h"
#include "program_state.h"

#include <SFML/Graphics.hpp>

#include <optional>

/// The About screen.
class about_view
{
public:
  about_view();

  /// Show the menu on-screen
  void draw();

  const auto& get_layout() const noexcept { return m_layout; }

  /// The next state to go to, if any.
  ///
  /// Will be empty if the user should remain in this screen
  const auto& get_next_state() const noexcept { return m_next_state; }

  /// The rotation, in degrees
  double get_rotation() const noexcept { return m_rotation; };

  /// Process all events
  /// @return if the user wants to quit
  bool process_event(sf::Event& event);

  /// Process a resize event
  void process_resize_event(sf::Event& event);

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

  /// Prepare for showing this dialog
  void start();

  /// End showing this dialog
  void stop();

  /// Run the menu, until the user quits
  void tick();

private:

  /// The game clock, to measure the elapsed time
  sf::Clock m_clock;


  controls_bar m_controls_bar;

  /// The layout of this window
  about_view_layout m_layout;

  double m_rotation;


  /// The next state to go to, if any
  std::optional<program_state> m_next_state;

};

/// Show where the panels will be drawn
void draw_layout_panels(about_view& v);

void draw_background(about_view& v);
void draw_copyright(about_view& v);
void draw_panel(about_view& v);
void draw_subtitle(about_view& v);
void draw_contributors(about_view& v);
void draw_title(about_view& v);
void draw_url(about_view& v);


#endif // LOGIC_ONLY

#endif // ABOUT_VIEW_H
