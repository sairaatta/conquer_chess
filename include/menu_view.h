#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "ccfwd.h"
#include "game_options.h"
#include "game_resources.h"
#include "menu_view_layout.h"
#include "menu_view_item.h"
#include "physical_controllers.h"
#include "sleeper.h"

/// The main Menu dialog.
class menu_view
{
public:
  menu_view(
    const game_options& options,
    const physical_controllers& controllers
  );

  /// Run the menu, until the user quits
  void exec();

  /// Get the index of the background image.
  ///
  /// There are multiple possible background images and the one
  /// displayed is chosen randomly.
  int get_background_image_index() const noexcept { return m_background_image_index; }

  /// Get the number of frames per seconds this class is displayed at
  int get_fps() const noexcept { return m_sleeper.get_fps(); }

  const auto& get_layout() const noexcept { return m_layout; }

  auto& get_resources() noexcept { return m_resources; }

  const auto& get_selected() const noexcept { return m_selected; }

  auto& get_window() noexcept { return m_window; }

  /// Sets the selected item.
  /// If the selected item changes, play sound
  void set_selected(const menu_view_item i);

  /// Set the text to a uniform style
  void set_text_style(sf::Text& t);

private:

  /// Index of the background image
  int m_background_image_index;

  /// Sleeps to achieve a frame rate of 50-60 frames per second
  sleeper m_sleeper;

  /// The layout of this window
  menu_view_layout m_layout;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// The game options
  game_options m_game_options;

  /// The current physical controllers
  physical_controllers m_physical_controllers;

  /// Resources
  game_resources m_resources;

  /// The selected item
  menu_view_item m_selected;

  /// Show the number of frames per second?
  bool m_show_fps;

  /// Run the about screen
  void exec_about();

  /// Run the game
  void exec_game();

  /// Run the lobby
  void exec_lobby();

  /// Run the options screen
  void exec_options();

  /// View the played game
  void exec_played_game();

  /// Run the game with a replay
  void exec_replay();

  /// Run the dialog under 'Start'
  void exec_start();

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

/// Create a random background image index
int create_random_background_image_index(
  std::default_random_engine& rng_engine
);

/// Create a random background image index
int create_seedless_random_background_image_index();

void draw_about_panel(menu_view& v);
void draw_background_image(menu_view& v);

/// Draw the frames per second on the menu
void draw_fps(menu_view& v);

void draw_menu_panel(menu_view& v);
void draw_options_panel(menu_view& v);

/// Show where the panels will be drawn
void draw_layout_panels(menu_view& v);

void draw_quit_panel(menu_view& v);
void draw_selected_panel(menu_view& v);
void draw_subtitle_panel(menu_view& v);
void draw_start_panel(menu_view& v);
void draw_title_panel(menu_view& v);


#endif // LOGIC_ONLY

#endif // MENU_VIEW_H
