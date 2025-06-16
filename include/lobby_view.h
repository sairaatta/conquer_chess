#ifndef LOBBY_VIEW_H
#define LOBBY_VIEW_H

#ifndef LOGIC_ONLY

#include "program_state.h"
#include "lobby_view_layout.h"
#include "lobby_view_item.h"

#include <SFML/Graphics.hpp>

#include <optional>

/// The lobby screen.
class lobby_view
{
public:
  lobby_view();

  /// Show the menu on-screen
  void draw();

  /// Get the selected item of a player
  lobby_view_item get_selected(const side player_side) const noexcept;

  /// Is the player ready?
  bool get_start(const side player_side) const noexcept;

  const auto& get_layout() const noexcept { return m_layout; }

  /// The next state to go to, if any.
  ///
  /// Will be empty if the user should remain in this screen
  const auto& get_next_state() const noexcept { return m_next_state; }


  /// Process all events
  /// @return if the user wants to quit
  bool process_event(sf::Event& event);

  void process_resize_event(sf::Event& event);

  /// Sets the selected item.
  /// If the selected item changes, play sound
  void set_selected(const lobby_view_item i);

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

  void stop();

  void start();

  /// Does the countdown
  void tick();

private:

  /// Will be empty when players are chosing.
  /// When both are ready, the countdown will start
  std::optional<sf::Clock> m_clock;

  /// How long the countdown will take
  const int m_countdown_secs{3};

  /// The layout of this window
  lobby_view_layout m_layout;

  /// The selected item for LHS
  lobby_view_item m_lhs_cursor;

  /// Is LHS ready to start?
  bool m_lhs_start;

  /// The next state to go to, if any
  std::optional<program_state> m_next_state;

  /// The selected item for RHS
  lobby_view_item m_rhs_cursor;

  /// Is RHS ready to start?
  bool m_rhs_start;
};

/// The background image
void draw_background(lobby_view& v);

/// Show the side/color the players have picked
void draw_color_panel(lobby_view& v, const side player_side);

/// Show the controllers the players use
void draw_controls_panel(lobby_view& v, const side player_side);

/// Show the countdown
void draw_countdown(lobby_view& v, const int n_left_secs);

/// Show where the panels will be drawn
void draw_layout_panels(lobby_view& v);

/// Show the races the players have picked
void draw_race_panel(lobby_view& v, const side player_side);

/// Show if the players are ready
void draw_ready_panel(lobby_view& v, const side player_side);

/// Draw the cursor on the selected panel
void draw_selected_panel(lobby_view& v, const side player_side);

/// The image at the top
void draw_title(lobby_view& v);


#endif // LOGIC_ONLY

#endif // LOBBY_VIEW_H
