#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#ifndef LOGIC_ONLY

#include "ccfwd.h"
#include "cc_cli_options.h"
#include "sleep_scheduler.h"
#include "program_state.h"
#include "game_options.h"
#include "lobby_options.h"
#include "replay.h"
#include "view.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <map>

/// The single main window.
class main_window
{
public:
  main_window(const cc_cli_options& options);

  /// Run the window, until the user quits
  void exec();

  const auto& get_program_state() const noexcept { return m_program_state; }

private:
  /// The command-line options at startup
  cc_cli_options m_cli_options;

  std::map<program_state, std::unique_ptr<view>> m_views;

  game_options m_game_options;

  /// The lobby options used throughout the game.
  ///
  /// Will only be updated by the lobby screen
  lobby_options m_lobby_options;

  /// The replay of the last game
  replay m_replay;

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

};

void test_main_window();

#endif // LOGIC_ONLY

#endif // MAIN_WINDOW_H
