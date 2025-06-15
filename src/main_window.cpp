#include "main_window.h"
#include <cassert>
#include "screen_rect.h"
#include "sfml_helper.h"
#include "render_window.h"
#include "game_resources.h"

main_window::main_window(
  const physical_controllers& pcs
) : m_physical_controllers{pcs}
{
  game_resources::get().get_loading_screen_songs().get_heroes().setVolume(
    10
  );
  game_resources::get().get_loading_screen_songs().get_heroes().setLoop(true);
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
}

bool main_window::process_events()
{
  // User interaction
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
  switch(m_active_window_type)
  {
    case program_state::about:
      return m_about_view.process_event(event);
    case program_state::loading:
      return m_loading_view.process_event(event);
    case program_state::main_menu:
    default:
      // Fails when window is not implemented yet
      assert(m_active_window_type == program_state::main_menu);
      return m_menu_view.process_event(event);
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
  m_loading_view.process_resize_event(event);
  m_menu_view.process_resize_event(event);
}


void main_window::show()
{
  // Start drawing the new frame, by clearing the screen
  get_render_window().clear();

  switch(m_active_window_type)
  {
    case program_state::about:
      m_about_view.draw();
      break;
    case program_state::loading:
      m_loading_view.draw();
      break;
    case program_state::main_menu:
    default:
      // Fails when window is not implemented yet
      assert(m_active_window_type == program_state::main_menu);
      m_menu_view.draw();
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

  text.setString(sf::String(std::to_string(get_fps())));
  set_text_position(text, debug_rect);
  text.setCharacterSize(text.getCharacterSize() - 2);
  text.setFillColor(sf::Color::Black);
  get_render_window().draw(text);
}

void main_window::tick()
{
  switch (m_active_window_type) {
    case program_state::about:
      m_about_view.tick();
      if (m_about_view.get_next_state().has_value()) {
        m_active_window_type = m_about_view.get_next_state().value();
        m_about_view.stop();
      }
      break;
    case program_state::game:
      assert(!"TODO");
      break;
    case program_state::loading:
    {
      m_loading_view.tick();
      if (m_loading_view.is_done()) {

        m_loading_view.stop();
        m_active_window_type = program_state::main_menu;
        m_menu_view.start();
      }
    }
    break;
    case program_state::main_menu:
      m_menu_view.tick();
      if (m_menu_view.get_next_state().has_value()) {
        m_active_window_type = m_menu_view.get_next_state().value();
        m_menu_view.stop();
        switch (m_active_window_type)
        {
          case program_state::about:
            assert(m_active_window_type == program_state::about);
            m_about_view.start();
            break;
          case program_state::game:
            assert(!"TODO");
            break;
          case program_state::loading:
            assert(!"This should never happen");
            break;
          case program_state::main_menu:
            assert(!"This should never happen");
            break;
          case program_state::options:
            assert(!"TODO");
            break;
          case program_state::replay:
            assert(!"TODO");
            break;
        }
      }
      break;
    case program_state::options:
      assert(!"TODO");
      break;
    case program_state::replay:
      assert(!"TODO");
      break;
  }
}


