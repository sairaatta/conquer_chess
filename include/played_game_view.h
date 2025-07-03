#ifndef PLAYED_GAME_VIEW_H
#define PLAYED_GAME_VIEW_H

#ifndef LOGIC_ONLY

#include "played_game_view_layout.h"
#include "game_controller.h"
#include "controls_bar.h"
#include "view.h"

#include <SFML/Graphics.hpp>


/// Dialog to view a played Conquer Chess game
class played_game_view : public view
{
public:
  played_game_view();

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

  const auto& get_game() const noexcept { return m_game_controller.get_game(); }

  const auto& get_layout() const noexcept { return m_layout; }

private:

  controls_bar m_controls_bar;

  /// The played game
  game_controller m_game_controller;

  /// The layout of this window
  played_game_view_layout m_layout;
};

/// Show where the panels will be drawn
void draw_layout_panels(played_game_view& v);

/// Show where the panels will be drawn
void show_text_panel(played_game_view& v);

#endif // LOGIC_ONLY

#endif // PLAYED_GAME_VIEW_H
