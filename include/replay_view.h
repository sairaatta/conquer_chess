#ifndef REPLAY_VIEW_H
#define REPLAY_VIEW_H

#ifndef LOGIC_ONLY

#include "replay_view_layout.h"
#include "replay.h"
#include "controls_bar.h"
#include "view.h"

#include <SFML/Graphics.hpp>


/// Dialog to view a played Conquer Chess game
class replay_view : public view
{
public:
  replay_view();

  /// Draw the menu on the main window
  void draw_impl() override;

  /// Process an event
  bool process_event_impl(sf::Event& e) override;

  /// Process a resize event
  void process_resize_event_impl(sf::Event& event) override;

  void set_replay(const replay& r);

  /// Prepare this window for being shown
  void start_impl() override;

  /// Clean up after this window has been shown
  void stop_impl() override;

  /// Run the game, until the user quits
  void tick_impl(const delta_t dt) override;

  const auto& get_game() const noexcept { return m_replay.get_game(); }

  const auto& get_layout() const noexcept { return m_layout; }

  /// The played game
  const auto& get_replayer() const noexcept { return m_replay; }

  const auto& get_statistics() const noexcept { return m_statistics; }

private:

  controls_bar m_controls_bar;

  /// The played game
  replay m_replay;

  /// Costly to extract from replayer
  game_statistics_in_time m_statistics;

  /// The layout of this window
  replay_view_layout m_layout;
};

/// Draw the board
void draw_board(replay_view& v);

/// Draw the statistics
void draw_statistics(replay_view& v);


#endif // LOGIC_ONLY

#endif // REPLAY_VIEW_H
