#include "main_window.h"

#include "chess_move.h"

main_window::main_window(
  const game_options& go,
  const physical_controllers& pcs
) : m_game_options{go},
    m_physical_controllers{pcs}
{

}

void main_window::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_main_menu_screen_size().get_x(),
      get_default_main_menu_screen_size().get_y()
    ),
    "Conquer Chess",
    sf::Style::Fullscreen
  );

  while (m_window.isOpen())
  {
    // Process user input and play game until instructed to exit
    const bool must_quit{process_events()};
    if (must_quit) return;

    // Show the new state
    show();
  }
}

bool main_window::process_events()
{
  // User interaction
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    if (event.type == sf::Event::Resized)
    {
      // From https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized
      const sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
      m_window.setView(sf::View(visible_area));

      // From https://stackoverflow.com/a/38486530
      if (1 == 2 && "centering would really work")
      {
        m_window.setView(
          sf::View(
            m_window.getView().getCenter(),
            sf::Vector2f(event.size.width, event.size.height)
          )
        );
      }

      /*
      m_layout = menu_view_layout(
        screen_coordinate(event.size.width, event.size.height),
        get_default_margin_width()
      );
      */
    }
    else if (event.type == sf::Event::Closed)
    {
      m_window.close();
      return true; // Game is done
    }
    else if (event.type == sf::Event::KeyPressed)
    {
      sf::Keyboard::Key key_pressed = event.key.code;
      if (key_pressed == sf::Keyboard::Key::Escape)
      {
        m_window.close();
        return true;
      }
    }
  }
  return false; // Do not close the window :-)
}

void main_window::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();


  // Display all shapes
  m_window.display();

  m_sleep_scheduler.tick();
}


