#ifndef PLAYED_GAME_VIEW_H
#define PLAYED_GAME_VIEW_H

#ifndef LOGIC_ONLY

#include "played_game_view_layout.h"
#include "game.h"
#include "controls_bar.h"
#include "view.h"

#include <SFML/Graphics.hpp>


/// Dialog to view a played Conquer Chess game
class played_game_view : public view
{
public:
  played_game_view(const game& g);

  /// Run the menu, until the user quits
  void exec();

  const auto& get_game() const noexcept { return m_game; }

  const auto& get_layout() const noexcept { return m_layout; }

private:

  controls_bar m_controls_bar;

  /// The played game
  game m_game;

  /// The layout of this window
  played_game_view_layout m_layout;

  /// Process all events
  /// @return if the user wants to quit
  bool process_event_impl(sf::Event& event) override;

  /// Show the menu on-screen
  void draw_impl() override;
};

/// Show where the panels will be drawn
void draw_layout_panels(played_game_view& v);

/// Show where the panels will be drawn
void show_text_panel(played_game_view& v);

#endif // LOGIC_ONLY

#endif // PLAYED_GAME_VIEW_H
