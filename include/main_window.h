#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "ccfwd.h"
#include "sleep_scheduler.h"
#include "game_options.h"
#include "physical_controllers.h"
#include "active_window_type.h"
#include "loading_view.h"
#include "menu_view.h"

/// The single main window.
class main_window
{
public:
  main_window(
    const game_options& go,
    const physical_controllers& cs
  );

  /// Run the window, until the user quits
  void exec();

  /// Get the number of frames per seconds this class is displayed at
  int get_fps() const noexcept { return m_sleep_scheduler.get_fps(); }

private:

  /// The initial game options, as can be set in the command-line
  game_options m_game_options;

  /// The initial physical controllers, as can be set in the command-line
  physical_controllers m_physical_controllers;

  /// Sleeps to achieve a frame rate of 50-60 frames per second
  sleep_scheduler m_sleep_scheduler;

  active_window_type m_active_window_type{active_window_type::loading};

  /// Show the debug info
  bool m_show_debug_info{true};

  /// The loading screen
  loading_view m_loading_view;

  /// The loading screen
  menu_view m_menu_view;

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

};

#endif // LOGIC_ONLY

#endif // MAIN_WINDOW_H
