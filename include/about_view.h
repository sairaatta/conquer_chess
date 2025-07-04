#ifndef ABOUT_VIEW_H
#define ABOUT_VIEW_H

#ifndef LOGIC_ONLY

#include "view.h"
#include "about_view_layout.h"
#include "controls_bar.h"
#include "view.h"

#include <SFML/Graphics.hpp>

/// The About screen.
class about_view : public view
{
public:
  about_view();

  /// Show the menu on-screen
  void draw_impl() override;

  const auto& get_layout() const noexcept { return m_layout; }

  /// Process all events
  /// @return if the user wants to quit
  bool process_event_impl(sf::Event& event) override;

  /// Process a resize event
  void process_resize_event_impl(sf::Event& event) override;

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

  /// Prepare for showing this dialog
  void start_impl() override;

  /// End showing this dialog
  void stop_impl() override;

  /// Run the menu, until the user quits
  void tick_impl(const delta_t dt) override;

private:

  controls_bar m_controls_bar;

  /// The layout of this window
  about_view_layout m_layout;
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
