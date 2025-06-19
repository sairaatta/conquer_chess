#ifndef LOBBY_VIEW_H
#define LOBBY_VIEW_H

#ifndef LOGIC_ONLY

#include "controls_bar.h"
#include "program_state.h"
#include "lobby_view_layout.h"
#include "lobby_view_item.h"
#include "view.h"

#include <SFML/Graphics.hpp>
#include <optional>

/// The lobby screen.
class lobby_view : public view
{
public:
  lobby_view();

  /// Show the menu on-screen
  void draw_impl() override;

  bool get_dog_mode() const noexcept { return m_dog_mode; }

  /// Get the selected item of a player
  lobby_view_item get_selected(const side player_side) const noexcept;

  /// Is the player ready?
  bool get_start(const side player_side) const noexcept;

  const auto& get_layout() const noexcept { return m_layout; }


  /// Process all events
  /// @return if the user wants to quit
  bool process_event_impl(sf::Event& event) override;

  void process_resize_event_impl(sf::Event& event) override;

  /// Sets the selected item.
  /// If the selected item changes, play sound
  void set_selected(const lobby_view_item i);

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

  void stop_impl() override;

  void start_impl() override;

  /// Does the countdown
  void tick_impl(const delta_t dt) override;

private:

  /// Will be empty when players are chosing.
  /// When both are ready, the countdown will start
  std::optional<sf::Clock> m_clock;

  controls_bar m_controls_bar;

  /// How long the countdown will take
  const int m_countdown_secs{3};

  /// Yes, dog mode
  bool m_dog_mode{false};

  /// The layout of this window
  lobby_view_layout m_layout;

  /// The selected item for LHS
  lobby_view_item m_lhs_cursor;

  /// Is LHS ready to start?
  bool m_lhs_start;

  /// The selected item for RHS
  lobby_view_item m_rhs_cursor;

  /// Is RHS ready to start?
  bool m_rhs_start;
};

/// The background image
void draw_background(lobby_view& v);

/// Show the side/color the players have picked
void draw_color_panel(lobby_view& v, const side player_side);

/// Show the countdown
void draw_countdown(lobby_view& v, const int n_left_secs);

/// Show the portraits of the kings of the race the players use
void draw_king_portrait(lobby_view& v, const side player_side);

/// Show where the panels will be drawn
void draw_layout_panels(lobby_view& v);

/// Show the races the players have picked
void draw_race_panel(lobby_view& v, const side player_side);

/// Show if the players are ready
void draw_ready_panel(lobby_view& v, const side player_side);

/// Draw the cursor on the selected panel
void draw_selected_panel(lobby_view& v, const side player_side);

/// The image at the top
void draw_lobby_title(lobby_view& v);


#endif // LOGIC_ONLY

#endif // LOBBY_VIEW_H
