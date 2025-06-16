/// @mainpage Conquer Chess
///
/// Welcome to Conquer Chess
///
/// - [Conquer Chess homepage](https://richelbilderbeek.github.io/conquer_chess/)
///
/// This game has some singletons:
///
/// - \link{get_render_window}: to get the one-and-only SFML render window
/// - \link{get_resources}: to get the one-and-only collection of resources

// Use LOGIC_ONLY to be able to run on GHA
#include "about_view_layout.h"
#include "action_history.h"
#include "board_to_text_options.h"
#include "chess_move.h"
#include "played_game_view_layout.h"
#include "controls_view_item.h"
#include "controls_view_layout.h"
#include "physical_controller.h"
#include "physical_controllers.h"
#include "fps_clock.h"
#include "game_controller.h"
#include "game_log.h"
#include "game_options.h"
#include "game_rect.h"
#include "game_view_layout.h"
#include "helper.h"
#include "id.h"
#include "key_bindings.h"
#include "loading_view.h"
#include "lobby_options.h"
#include "lobby_view_item.h"
#include "lobby_view_layout.h"
#include "menu_view_item.h"
#include "menu_view_layout.h"
#include "options_view_layout.h"
#include "pgn_string.h"
#include "piece_actions.h"
#include "pieces.h"
#include "race.h"
#include "sfml_helper.h"
#include "read_only.h"
#include "replay.h"
#include "screen_coordinate.h"
#include "sleep_scheduler.h"
#include "test_game.h"
#include "main_window.h"

#include <SFML/Graphics.hpp>

#include <cassert>

/// All tests are called from here, only in debug mode
void test()
{
#ifndef NDEBUG

  test_sleep_scheduler();


  test_action_number();
  test_about_view_layout();
  test_action_history();
  test_board_to_text_options();
  test_chess_color();
  test_chess_move();
  test_user_input();
  test_control_action_type();
  test_user_inputs();
  test_controller();
  test_physical_controller_type();
  test_physical_controllers();
  test_controls_view_item();
  test_controls_view_layout();
  test_delta_t();
  test_fps_clock();
  test_game();
  test_game_controller();
  test_game_coordinate();
  test_game_options();
  test_game_rect();
  test_game_speed();
  test_game_view_layout();
  test_helper();
  test_id();
  test_key_bindings();
  test_lobby_options();
  test_lobby_view_item();
  test_lobby_view_layout();
  test_log();
  test_menu_view_item();
  test_menu_view_layout();
  test_message();
  test_message_type();
  test_options_view_item();
  test_options_view_layout();
  test_pgn_string();
  test_piece();
  test_piece_action();
  test_piece_actions();
  test_piece_action_type();
  test_piece_type();
  test_pieces();
  test_played_game_view_layout();
  test_race();
  test_read_only();
  test_replay();
  test_replayer();
  test_screen_coordinate();
  test_screen_rect();
  test_side();
  test_sfml_helper();
  test_sleep_scheduler();
  test_square();
  test_starting_position_type();
  test_volume();

#ifndef LOGIC_ONLY
  test_loading_view();
  test_resource_loader();
#endif // LOGIC_ONLY

#endif // NDEBUG
}

std::vector<std::string> collect_args(int argc, char **argv) {
  std::vector<std::string> v(argv, argv + argc);
  return v;
}

int main(int argc, char **argv) //!OCLINT tests may be long
{
  #ifndef NDEBUG
  test();
  #endif
  const auto args = collect_args(argc, argv);
  if (args.size() == 1)
  {
    #define USE_TWO_KEYBOARDS
    #ifdef USE_TWO_KEYBOARDS
    use_two_keyboard_controllers();
    #else
    use_keyboard_mouse_controllers();
    #endif
    #ifndef LOGIC_ONLY
    main_window v;
    v.exec();
    #endif // LOGIC_ONLY
  }
}
