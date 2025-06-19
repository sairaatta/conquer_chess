#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#ifndef LOGIC_ONLY

#include "ccfwd.h"
#include "sleep_scheduler.h"
#include "about_view.h"
#include "program_state.h"
#include "loading_view.h"
#include "lobby_view.h"
#include "game_view.h"
#include "menu_view.h"
#include "controls_view.h"
#include "options_view.h"
#include "loading_view.h"
#include "view.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <map>

/// The single main window.
class main_window
{
public:
  main_window();

  /// Run the window, until the user quits
  void exec();

private:

  std::map<program_state, std::unique_ptr<view>> m_views;

  //about_view m_about_view;

  /// Sleeps to achieve a frame rate of 50-60 frames per second
  sleep_scheduler m_sleep_scheduler;

  program_state m_program_state{program_state::loading};

  /// Process all events
  /// @return if the user wants to quit
  bool process_events();

  /// Process an event
  /// @return if the user wants to quit
  bool process_event(sf::Event& event);

  void process_resize_event(sf::Event& event);

  /// The program reaches a new state.
  ///
  /// Stop the current window and start the new
  void set_new_state(const program_state s);

  /// Show the current state
  void show();

  /// Draw the debug info over the current state
  void show_debug_info();

  /// Go to the next state (if any).
  ///
  /// Makes the screen do its thing,
  /// for example, load textures, move pieces, etc.
  void tick();

  /*
  void tick_about();
  void tick_game();
  void tick_left_controls();
  void tick_loading();
  void tick_lobby();
  void tick_main_menu();
  void tick_options();
  void tick_replay();
  void tick_right_controls();
  */
};

#endif // LOGIC_ONLY

#endif // MAIN_WINDOW_H
