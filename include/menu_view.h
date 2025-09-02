#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#ifndef LOGIC_ONLY

#include "ccfwd.h"
#include "menu_view_layout.h"
#include "menu_view_item.h"
#include "game_options.h"
#include "controls_bar.h"
#include "view.h"

#include <SFML/Graphics.hpp>

#include <random>

/// The main Menu dialog.
class menu_view : public view
{
public:
  menu_view();

  /// Show the menu on-screen
  void draw_impl() override;

  /// Get the index of the background image.
  ///
  /// There are multiple possible background images and the one
  /// displayed is chosen randomly.
  int get_background_image_index() const noexcept { return m_background_image_index; }

  const auto& get_layout() const noexcept { return m_layout; }

  const auto& get_selected() const noexcept { return m_selected; }

  /// Process an event
  /// @return true if the user wants to quit the program
  bool process_event_impl(sf::Event& event) override;

  /// Process a resize event
  void process_resize_event_impl(sf::Event& event) override;

  /// Sets the selected item.
  /// If the selected item changes, play sound
  void set_selected(const menu_view_item i);

  /// Start displaying this window
  void start_impl() override;

  void stop_impl() override;

  void tick_impl(const delta_t dt) override;


private:

  /// Index of the background image
  int m_background_image_index;

  controls_bar m_controls_bar;

  game_options m_game_options;

  /// The layout of this window
  menu_view_layout m_layout;

  /// The selected main menu item
  menu_view_item m_selected;
};

/// Create a random background image index
int create_random_background_image_index(
  std::default_random_engine& rng_engine
);

/// Create a random background image index.
///
/// Use \link{create_random_background_image_index}
/// to be able to supply a random seed.
int create_seedless_random_background_image_index();

void draw_about_panel(menu_view& v);
void draw_background_image(menu_view& v);

/// Show where the panels will be drawn
void draw_layout_panels(menu_view& v);

void draw_menu_outline(menu_view& v);

/// The 'Options' submenu, with its own multiple buttons
void draw_options_sub_menu(menu_view& v);

/// The button with 'Options' on it
void draw_options_panel(menu_view& v);


void draw_quit_panel(menu_view& v);
void draw_selected_panel(menu_view& v);
void draw_subtitle_panel(menu_view& v);
void draw_start_panel(menu_view& v);
void draw_title_panel(menu_view& v);


#endif // LOGIC_ONLY

#endif // MENU_VIEW_H
