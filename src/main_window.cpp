#include "main_window.h"

#ifndef LOGIC_ONLY

#include "screen_rect.h"
#include "sfml_helper.h"
#include "render_window.h"
#include "game_resources.h"

#include <cassert>
#include <cmath>

main_window::main_window()
  : m_left_controls_view(side::lhs),
    m_right_controls_view(side::rhs)
{
  game_resources::get().get_loading_screen_songs().get_heroes().setVolume(
    10
  );
  game_resources::get().get_loading_screen_songs().get_heroes().play();

}

void main_window::exec()
{
  while (get_render_window().isOpen())
  {
    // Process user input and play game until instructed to exit
    const bool must_quit{process_events()};
    if (must_quit) return;

    // Go to the next state
    tick();

    // Show the new state
    show();
  }
  get_render_window().close();
}

bool main_window::process_events()
{
  sf::Event event;
  while (get_render_window().pollEvent(event))
  {
    const bool stop_program{process_event(event)};
    if (stop_program == true) return true;
  }
  return false; // Do not close the window :-)
}

bool main_window::process_event(sf::Event& event)
{
  // General events
  if (event.type == sf::Event::Resized)
  {
    process_resize_event(event);
    return false; // Do not close the program
  }
  if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::F3)
    {
      m_show_debug_info = !m_show_debug_info;
      return false; // Done. Do not close the program
    }
  }

  // Specific
  switch(m_program_state)
  {
    case program_state::about: return m_about_view.process_event(event);
    case program_state::game: return m_game_view.process_event(event);
    case program_state::left_controls: return m_left_controls_view.process_event(event);
    case program_state::lobby: return m_lobby_view.process_event(event);
    case program_state::loading: return m_loading_view.process_event(event);
    case program_state::main_menu: return m_menu_view.process_event(event);
    case program_state::options: return m_options_view.process_event(event);
    //case program_state::replay: return m_replay_view.process_event(event);
    case program_state::right_controls: return m_right_controls_view.process_event(event);
  }
  return false; // Do not close the program
}
void main_window::process_resize_event(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);

  // From https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized
  const sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
  get_render_window().setView(sf::View(visible_area));

  // Resize all windows
  m_about_view.process_resize_event(event);
  m_game_view.process_resize_event(event);
  m_left_controls_view.process_resize_event(event);
  m_loading_view.process_resize_event(event);
  m_lobby_view.process_resize_event(event);
  m_options_view.process_resize_event(event);
  m_menu_view.process_resize_event(event);
  m_right_controls_view.process_resize_event(event);
}

void main_window::set_new_state(const program_state s)
{
  // Stop the current/old state
  switch (m_program_state)
  {
    case program_state::about: m_about_view.stop(); break;
    case program_state::game: m_game_view.stop(); break;
    case program_state::left_controls: m_left_controls_view.stop(); break;
    case program_state::loading: m_loading_view.stop(); break;
    case program_state::lobby: m_lobby_view.stop(); break;
    case program_state::main_menu: m_menu_view.stop(); break;
    case program_state::options: m_options_view.stop(); break;
    //case program_state::replay: m_replay_view.stop(); break;
    case program_state::right_controls: m_right_controls_view.stop(); break;
  }

  // Start the new state
  m_program_state = s;

  switch (m_program_state)
  {
    case program_state::about: m_about_view.start(); break;
    case program_state::game: m_game_view.start(); break;
    case program_state::left_controls: m_left_controls_view.start(); break;
    case program_state::loading: m_loading_view.start(); break;
    case program_state::lobby: m_lobby_view.start(); break;
    case program_state::main_menu: m_menu_view.start(); break;
    case program_state::options: m_options_view.start(); break;
    //case program_state::replay: m_replay_view.start(); break;
    case program_state::right_controls: m_right_controls_view.start(); break;
  }

}

void main_window::show()
{
  // Start drawing the new frame, by clearing the screen
  get_render_window().clear();

  switch(m_program_state)
  {
    case program_state::about: m_about_view.draw(); break;
    case program_state::game: m_game_view.draw(); break;
    case program_state::left_controls: m_left_controls_view.draw(); break;
    case program_state::loading: m_loading_view.draw(); break;
    case program_state::lobby: m_lobby_view.draw(); break;
    case program_state::main_menu: m_menu_view.draw(); break;
    case program_state::options: m_options_view.draw(); break;
    //case program_state::replay: m_replay_view.draw(); break;
    case program_state::right_controls: m_right_controls_view.draw(); break;
  }

  if (m_show_debug_info) {
    show_debug_info();
  }

  // Display all shapes
  get_render_window().display();

  m_sleep_scheduler.tick();
}

void main_window::show_debug_info()
{
  const auto debug_rect = screen_rect(
    screen_coordinate(32, 32),
    screen_coordinate(100, 80)
  );
  sf::RectangleShape rectangle;
  set_rect(rectangle, debug_rect);
  rectangle.setFillColor(sf::Color::White);
  get_render_window().draw(rectangle);

  // Text
  sf::Text text;
  text.setFont(get_arial_font());
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(20);
  text.setFillColor(sf::Color::Black);

  const int fps{
    static_cast<int>(std::round(m_sleep_scheduler.get_fps()))
  };
  text.setString(sf::String(std::to_string(fps)));
  set_text_position(text, debug_rect);
  text.setCharacterSize(text.getCharacterSize() - 2);
  text.setFillColor(sf::Color::Black);
  get_render_window().draw(text);
}

void main_window::tick()
{
  switch (m_program_state) {
    case program_state::about: tick_about(); break;
    case program_state::game: tick_game(); break;
    case program_state::lobby: tick_lobby(); break;
    case program_state::left_controls: tick_left_controls(); break;
    case program_state::loading: tick_loading(); break;
    case program_state::main_menu: tick_main_menu(); break;
    case program_state::options: tick_options(); break;
    //case program_state::replay: tick_replay(); break;
    case program_state::right_controls: tick_right_controls(); break;
  }
}

void main_window::tick_about()
{
  m_about_view.tick();
  if (m_about_view.get_next_state().has_value()) {
    set_new_state(m_about_view.get_next_state().value());
  }
}

void main_window::tick_game()
{
  const delta_t dt{
      get_speed_multiplier(game_options::get().get_game_speed())
    / m_sleep_scheduler.get_fps()
  };

  m_game_view.tick(dt);
  if (m_game_view.get_next_state().has_value()) {
    set_new_state(m_game_view.get_next_state().value());
  }
}

void main_window::tick_left_controls()
{
  m_left_controls_view.tick();
  if (m_left_controls_view.get_next_state().has_value()) {
    set_new_state(m_left_controls_view.get_next_state().value());
  }
}

void main_window::tick_loading()
{
  m_loading_view.tick();
  if (m_loading_view.get_next_state().has_value()) {
    set_new_state(m_loading_view.get_next_state().value());
  }
}

void main_window::tick_lobby()
{
  m_lobby_view.tick();
  if (m_lobby_view.get_next_state().has_value()) {
    set_new_state(m_lobby_view.get_next_state().value());
  }
}

void main_window::tick_main_menu()
{
  m_menu_view.tick();
  if (m_menu_view.get_next_state().has_value()) {
    set_new_state(m_menu_view.get_next_state().value());
  }
}

void main_window::tick_options()
{
  m_options_view.tick();
  if (m_options_view.get_next_state().has_value()) {
    set_new_state(m_options_view.get_next_state().value());
  }
}

void main_window::tick_replay()
{
  assert(!"TODO");
}

void main_window::tick_right_controls()
{
  m_right_controls_view.tick();
  if (m_right_controls_view.get_next_state().has_value()) {
    set_new_state(m_right_controls_view.get_next_state().value());
  }
}

#endif // LOGIC_ONLY

