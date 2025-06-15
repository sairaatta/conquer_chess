#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "ccfwd.h"
#include "sleep_scheduler.h"
#include "about_view.h"
#include "physical_controllers.h"
#include "program_state.h"
#include "loading_view.h"
#include "menu_view.h"
#include "options_view.h"

/// The single main window.
class main_window
{
public:
  main_window(
    const physical_controllers& cs
  );

  /// Run the window, until the user quits
  void exec();

private:

  about_view m_about_view;

  /// The initial physical controllers, as can be set in the command-line
  physical_controllers m_physical_controllers;

  /// Sleeps to achieve a frame rate of 50-60 frames per second
  sleep_scheduler m_sleep_scheduler;

  program_state m_program_state{program_state::loading};

  /// Show the debug info
  bool m_show_debug_info{true};

  /// The loading screen
  loading_view m_loading_view;

  /// The main menu screen
  menu_view m_menu_view;

  /// The options screen
  options_view m_options_view;

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Process an event
  /// @return if the user wants to quit
  bool process_event(sf::Event& event);

  void process_resize_event(sf::Event& event);

  /// Show the current state
  void show();

  /// Draw the debug info over the current state
  void show_debug_info();

  /// Go to the next state (if any).
  ///
  /// Makes the screen do its thing,
  /// for example, load textures, move pieces, etc.
  void tick();

  /// The tick method, for the about screen
  void tick_about();

  /// The tick method, for the game screen
  void tick_game();

  /// The tick method, for the loading screen
  void tick_loading();

  /// The tick method, for the main menu screen
  void tick_main_menu();

  /// The tick method, for the options screen
  void tick_options();

  /// The tick method, for the replay screen
  void tick_replay();
};

#endif // LOGIC_ONLY

#endif // MAIN_WINDOW_H
