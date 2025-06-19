#ifndef VIEW_H
#define VIEW_H

#include "delta_t.h"
#include "program_state.h"

#include <SFML/Window/Event.hpp>

#include <optional>

/// The abstract base class of all other views
class view
{
public:
  view();
  virtual ~view();

  /// Clear the next state
  void clear_next_state();

  /// Draw the menu on the main window
  virtual void draw() = 0;

  /// The next state to go to, if any
  const auto& get_next_state() const noexcept { return m_next_state; }

  /// Is this window active?
  ///
  /// It can be activated by 'start' and deactivated by 'stop'
  bool is_active() const noexcept { return m_is_active; }

  /// Process an event
  virtual bool process_event(sf::Event& e) = 0;

  /// Process a resize event
  virtual void process_resize_event(sf::Event& event) = 0;

  /// Set if this window is active.
  void set_is_active(const bool is_active);

  /// Set the next state to go to.
  ///
  /// Use \link{clear_next_state} to clear the next state
  void set_next_state(const program_state next_state);

  virtual void start() = 0;

  virtual void stop() = 0;

  /// Run the game, until the user quits
  virtual void tick(const delta_t dt) = 0;

private:

  bool m_is_active{false};

  /// The next state to go to, if any
  std::optional<program_state> m_next_state;

};

#endif // VIEW_H
