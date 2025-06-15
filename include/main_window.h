#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "ccfwd.h"
#include "sleep_scheduler.h"
#include "game_options.h"
#include "physical_controllers.h"

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

  auto& get_window() noexcept { return m_window; }

private:

  /// The initial game options, as can be set in the command-line
  game_options m_game_options;

  /// The initial physical controllers, as can be set in the command-line
  physical_controllers m_physical_controllers;

  /// Sleeps to achieve a frame rate of 50-60 frames per second
  sleep_scheduler m_sleep_scheduler;

  /// The window to draw to
  sf::RenderWindow m_window;

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Show the menu on-screen
  void show();
};

#endif // LOGIC_ONLY

#endif // MAIN_WINDOW_H
