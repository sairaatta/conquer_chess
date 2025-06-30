#ifndef GAME_STATISTICS_VIEW_H
#define GAME_STATISTICS_VIEW_H

#ifndef LOGIC_ONLY

#include "game_statistics_view_layout.h"
#include "game.h"
#include "game_statistics_in_time.h"
#include "controls_bar.h"
#include "view.h"

#include <SFML/Graphics.hpp>


/// Dialog to view a the statistics of a played Conquer Chess game
class game_statistics_view : public view
{
public:
  game_statistics_view();

  /// Draw the menu on the main window
  void draw_impl() override;

  /// Process an event
  bool process_event_impl(sf::Event& e) override;

  /// Process a resize event
  void process_resize_event_impl(sf::Event& event) override;

  /// Prepare this window for being shown
  void start_impl() override;

  /// Clean up after this window has been shown
  void stop_impl() override;

  /// Run the game, until the user quits
  void tick_impl(const delta_t dt) override;

  const auto& get_statistics() const noexcept { return m_statistics; }

  const auto& get_layout() const noexcept { return m_layout; }

private:

  controls_bar m_controls_bar;

  /// The played game
  game_statistics_in_time m_statistics;

  /// The layout of this window
  game_statistics_view_layout m_layout;
};

/// Show where the panels will be drawn
void draw_layout_panels(game_statistics_view& v);

/// Show where the panels will be drawn
void draw_plot_panel(game_statistics_view& v);

#endif // LOGIC_ONLY

#endif // GAME_STATISTICS_VIEW_H
