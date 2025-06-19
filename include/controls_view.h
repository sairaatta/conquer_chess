#ifndef CONTROLS_VIEW_H
#define CONTROLS_VIEW_H

#ifndef LOGIC_ONLY

#include "controls_view_layout.h"
#include "controls_view_item.h"
#include "program_state.h"
#include "side.h"
#include "controls_bar.h"
#include "chess_color.h"
#include "view.h"

#include <SFML/Graphics.hpp>

#include <optional>

/// The Controls dialog.
///
/// The Controls dialog is where the user
/// determines which controllers (e.g. keyboard
/// or mouse) are used.
class controls_view : public view
{
public:
  controls_view(const side& player_side);

  /// Show the menu on-screen
  void draw() override;


  const auto& get_layout() const noexcept { return m_layout; }

  /// The player we are setting up the controls of
  side get_player_side() const { return m_player_side; }

  const auto& get_selected() const noexcept { return m_selected; }

  /// Process all events
  /// @return if the user wants to quit
  bool process_event(sf::Event& event) override;

  /// Resize
  void process_resize_event(sf::Event& event) override;

  /// Set a uniform text style
  void set_text_style(sf::Text& text);

  void start() override;

  void stop() override;

  void tick(const delta_t dt) override;

private:

  controls_bar m_controls_bar;

  /// The layout of this window
  controls_view_layout m_layout;

  /// The player we are setting up the controls of
  side m_player_side;

  /// The selected item
  controls_view_item m_selected;

  /// Change the selected item, from spacebar or LMB click
  void change_selected();

};

/// @param is_active if the panel is active:
///   if true, draw with the real color,
///   else use a greyed out color
void draw_panel(
  const screen_rect& panel_position,
  const std::string panel_text,
  const chess_color color,
  const bool is_active
);


sf::Keyboard::Key get_key_for_action_1(const controls_view& v);
sf::Keyboard::Key get_key_for_action_2(const controls_view& v);
sf::Keyboard::Key get_key_for_action_3(const controls_view& v);
sf::Keyboard::Key get_key_for_action_4(const controls_view& v);
sf::Mouse::Button get_button_for_do(const controls_view& v);
sf::Keyboard::Key get_key_for_move_down(const controls_view& v);
sf::Keyboard::Key get_key_for_move_left(const controls_view& v);
sf::Keyboard::Key get_key_for_move_right(const controls_view& v);
sf::Keyboard::Key get_key_for_move_up(const controls_view& v);
sf::Mouse::Button get_button_for_next(const controls_view& v);

void draw_background(controls_view& v);

void draw_keyboard_panel(controls_view& v);

void draw_mouse_panel(controls_view& v);

/// Show where the panels will be drawn
void draw_layout_panels(controls_view& v);

void draw_selected_panel(controls_view& v);

void draw_type_panel(controls_view& v);


#endif // LOGIC_ONLY

#endif // CONTROLS_VIEW_H
