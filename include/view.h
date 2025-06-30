#ifndef VIEW_H
#define VIEW_H

#ifndef LOGIC_ONLY

#include "delta_t.h"
#include "program_state.h"

#include <SFML/Window/Event.hpp>

#include <optional>

/// The abstract base class of all other views
///
/// Class name      |Descripion
/// ----------------|-----------------
/// about_view      |Shows contributors
/// controls_view   |Setup the controls
/// game_view       |Playing the game
/// loading_view    |Loading the game
/// lobby_view      |The lobby
/// menu_view       |The main menu
/// options_view    |Game settings
/// player_game_view|View a played game
class view
{
public:
  view();
  virtual ~view();

  /// Clear the next state
  void clear_next_state();

  /// Draw the menu on the main window
  void draw();

  /// The next state to go to, if any
  const auto& get_next_state() const noexcept { return m_next_state; }

  /// Is this window active?
  ///
  /// It can be activated by 'start' and deactivated by 'stop'
  bool is_active() const noexcept { return m_is_active; }

  /// Process an event
  bool process_event(sf::Event& e);

  /// Process a resize event
  void process_resize_event(sf::Event& event);

  /// Set if this window is active.
  void set_is_active(const bool is_active);

  /// Set the next state to go to.
  ///
  /// Use \link{clear_next_state} to clear the next state
  void set_next_state(const program_state next_state);

  /// Prepare this window for being shown
  void start();

  /// Clean up after this window has been shown
  void stop();

  /// Run the game, until the user quits
  void tick(const delta_t dt);

private:

  bool m_is_active{false};

  /// The next state to go to, if any
  std::optional<program_state> m_next_state;

  /// Draw the menu on the main window
  virtual void draw_impl() = 0;

  /// Process an event
  virtual bool process_event_impl(sf::Event& e) = 0;

  /// Process a resize event
  virtual void process_resize_event_impl(sf::Event& event) = 0;

  /// Prepare this window for being shown
  virtual void start_impl() = 0;

  /// Clean up after this window has been shown
  virtual void stop_impl() = 0;

  /// Run the game, until the user quits
  virtual void tick_impl(const delta_t dt) = 0;

};

#endif // LOGIC_ONLY

#endif // VIEW_H
